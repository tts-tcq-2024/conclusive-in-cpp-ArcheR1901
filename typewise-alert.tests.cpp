#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(12, 20, 30), TOO_LOW);
    EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);
    EXPECT_EQ(inferBreach(35, 20, 30), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifyTemperatureBreach) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 30), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 40), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 40), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 30), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 45), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlert) {
    BatteryCharacter battery = {HI_ACTIVE_COOLING, "BrandA"};
    
    // Redirect stdout to capture print output for testing
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, battery, 50);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");

    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, battery, 50);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}
