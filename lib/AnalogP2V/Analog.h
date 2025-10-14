#pragma once
#include <Arduino.h>
#include <vector>
#include <functional>

class Analog
{
public:
    enum FilterType
    {
        NONE,
        MOVING_AVG,
        COMPLEMENTARY,
        MEDIAN
    };

    Analog(uint8_t pin);

    void update();
    float getValue() const { return filteredValue; }
    float getRaw() const { return rawValue; }

    // Filter setup
    void setFilterNone();
    void setFilterMovingAvg(size_t windowSize);
    void setFilterComplementary(float alpha);
    void setFilterMedian(size_t windowSize);

    // Scaling and inversion
    void setScale(float minIn, float maxIn, float minOut, float maxOut);
    void setInvert(bool invert);

    // Auto-calibration
    void setAutoCalibrate(bool enable);
    void resetCalibration();

    // Callback
    void onChange(std::function<void(float)> cb, float threshold = 0.01f);

private:
    uint8_t pin;
    FilterType filterType = NONE;

    // Values
    float rawValue = 0.0f;
    float filteredValue = 0.0f;
    float prevFiltered = 0.0f;

    // Moving average
    std::vector<float> window;
    size_t windowSize = 1;
    size_t index = 0;
    float sum = 0;

    // Complementary
    float alpha = 0.5f;

    // Median
    std::vector<float> medBuffer;

    // Scaling
    float minIn = 0, maxIn = 4095;
    float minOut = 0, maxOut = 1;
    bool invert = false;

    // Auto calibration
    bool autoCal = false;
    float autoMin = 4095, autoMax = 0;

    // Callback
    std::function<void(float)> callback = nullptr;
    float changeThreshold = 0.01f;

    float readRaw();
    float applyFilter(float val);
    float applyScaling(float val);
    float computeMedian(std::vector<float> &buf);
};
