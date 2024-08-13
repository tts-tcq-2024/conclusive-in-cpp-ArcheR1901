#include "typewise-alert.h"
#include <stdio.h>

// Define limits as constants to avoid magic numbers
const int PASSIVE_COOLING_UPPER_LIMIT = 35;
const int HI_ACTIVE_COOLING_UPPER_LIMIT = 45;
const int MED_ACTIVE_COOLING_UPPER_LIMIT = 40;

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if(value < lowerLimit) {
        return TOO_LOW;
    }
    if(value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    int upperLimit = 0;
    
    switch(coolingType) {
        case PASSIVE_COOLING:
            upperLimit = PASSIVE_COOLING_UPPER_LIMIT;
            break;
        case HI_ACTIVE_COOLING:
            upperLimit = HI_ACTIVE_COOLING_UPPER_LIMIT;
            break;
        case MED_ACTIVE_COOLING:
            upperLimit = MED_ACTIVE_COOLING_UPPER_LIMIT;
            break;
        default:
            // Handle unexpected cooling type
            return NORMAL;
    }
    
    return inferBreach(temperatureInC, 0, upperLimit);
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

    switch(alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
        default:
            // Handle unexpected alert target
            break;
    }
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recepient = "a.b@c.com";
    if(breachType != NORMAL) {
        printf("To: %s\n", recepient);
        const char* breachMessage = (breachType == TOO_LOW) ? 
                                     "Hi, the temperature is too low\n" : 
                                     "Hi, the temperature is too high\n";
        printf("%s", breachMessage);
    }
}
