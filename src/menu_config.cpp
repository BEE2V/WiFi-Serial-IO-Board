/*
#include "MenuSystem.h"
#include "VarBind.h"

float kp = 1.0, ki = 0.5, kd = 0.1;
float speedFast = 100, speedSlow = 50;

MenuPage rootMenu("Main Menu");
MenuPage pidMenu("PID Tune");
MenuPage modeMenu("SetMode");

void setupMenus(MenuSystem &menu)
{
    // PID page
    pidMenu.addItem(new VarBind("Kp", &kp, 'f', 0, 10, 0.1, TRIMMER_EDIT));
    pidMenu.addItem(new VarBind("Ki", &ki, 'f', 0, 10, 0.1, TRIMMER_EDIT));
    pidMenu.addItem(new VarBind("Kd", &kd, 'f', 0, 10, 0.1, TRIMMER_EDIT));

    // Mode page
    modeMenu.addItem(new VarBind("Fast Speed", &speedFast, 'f', 0, 200, 1, DIGIT_EDIT));
    modeMenu.addItem(new VarBind("Slow Speed", &speedSlow, 'f', 0, 200, 1, DIGIT_EDIT));

    // Root
    rootMenu.addItem(&pidMenu);
    rootMenu.addItem(&modeMenu);

    menu.setRoot(&rootMenu);
}
*/