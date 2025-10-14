#include "Analog.h"

Analog::Analog(uint8_t pin) : pin(pin)
{
    pinMode(pin, INPUT);
}

// ==== Main update ====
void Analog::update()
{
    rawValue = readRaw();

    if (autoCal)
    {
        if (rawValue < autoMin)
            autoMin = rawValue;
        if (rawValue > autoMax)
            autoMax = rawValue;
    }

    float scaled = applyScaling(rawValue);
    filteredValue = applyFilter(scaled);

    if (callback && fabs(filteredValue - prevFiltered) >= changeThreshold)
    {
        callback(filteredValue);
    }
    prevFiltered = filteredValue;
}

// ==== Filter selection ====
void Analog::setFilterNone()
{
    filterType = NONE;
}

void Analog::setFilterMovingAvg(size_t size)
{
    filterType = MOVING_AVG;
    windowSize = max((size_t)1, size);
    window.clear();
    sum = 0;
    index = 0;
}

void Analog::setFilterComplementary(float a)
{
    filterType = COMPLEMENTARY;
    alpha = constrain(a, 0.0f, 1.0f);
}

void Analog::setFilterMedian(size_t size)
{
    filterType = MEDIAN;
    windowSize = max((size_t)1, size);
    medBuffer.clear();
}

// ==== Scaling ====
void Analog::setScale(float minIn_, float maxIn_, float minOut_, float maxOut_)
{
    minIn = minIn_;
    maxIn = maxIn_;
    minOut = minOut_;
    maxOut = maxOut_;
}

void Analog::setInvert(bool inv)
{
    invert = inv;
}

// ==== Auto calibration ====
void Analog::setAutoCalibrate(bool enable)
{
    autoCal = enable;
    if (enable)
    {
        autoMin = 4095;
        autoMax = 0;
    }
}

void Analog::resetCalibration()
{
    autoMin = 4095;
    autoMax = 0;
}

// ==== Callback ====
void Analog::onChange(std::function<void(float)> cb, float threshold)
{
    callback = cb;
    changeThreshold = threshold;
}

// ==== Internal ====
float Analog::readRaw()
{
    return analogRead(pin);
}

float Analog::applyFilter(float val)
{
    switch (filterType)
    {
    case MOVING_AVG:
        if (window.size() < windowSize)
        {
            window.push_back(val);
            sum += val;
            return sum / window.size();
        }
        else
        {
            sum -= window[index];
            sum += val;
            window[index] = val;
            index = (index + 1) % windowSize;
            return sum / windowSize;
        }

    case COMPLEMENTARY:
        filteredValue = alpha * val + (1 - alpha) * filteredValue;
        return filteredValue;

    case MEDIAN:
        medBuffer.push_back(val);
        if (medBuffer.size() > windowSize)
            medBuffer.erase(medBuffer.begin());
        return computeMedian(medBuffer);

    default:
        return val;
    }
}

float Analog::computeMedian(std::vector<float> &buf)
{
    if (buf.empty())
        return 0;
    std::vector<float> sorted = buf;
    std::sort(sorted.begin(), sorted.end());
    size_t mid = sorted.size() / 2;
    if (sorted.size() % 2 == 0)
        return (sorted[mid - 1] + sorted[mid]) / 2.0f;
    else
        return sorted[mid];
}

float Analog::applyScaling(float val)
{
    float inMin = autoCal ? autoMin : minIn;
    float inMax = autoCal ? autoMax : maxIn;

    if (fabs(inMax - inMin) < 1e-6) // prevent divide-by-zero
        return filteredValue;       // return last known value or 0

    float scaled = (val - inMin) * (maxOut - minOut) / (inMax - inMin) + minOut;

    if (invert)
        scaled = maxOut - (scaled - minOut);

    return constrain(scaled, minOut, maxOut);
}
