#include "typewise-alert.h"
#include <stdio.h>

// Configuration table for cooling types and their limits
const TemperatureLimits limits[] = {
    {0, 35}, // PASSIVE_COOLING
    {0, 45}, // HI_ACTIVE_COOLING
    {0, 40}  // MED_ACTIVE_COOLING
};

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    TemperatureLimits tempLimits = limits[coolingType];
    return inferBreach(temperatureInC, tempLimits.lowerLimit, tempLimits.upperLimit);
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\nHi, the temperature is too low\n", recipient);
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\nHi, the temperature is too high\n", recipient);
    }
}

typedef void (*AlertFunction)(BreachType);

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    AlertFunction alertFuncs[] = {sendToController, sendToEmail};
    alertFuncs[alertTarget](breachType);
}
