#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <StellarisWare/driverlib/gpio.h>
#include <stdint.h>
#include "PLL.h"
#include "User.h"
#include "TestSampling.h"
#include "ST7735.h"
#include <math.h>


tBoolean blink_on = true;
tBoolean initialized = false;

extern float leftValue;
extern float rightValue;
extern tADC* leftPot;
extern tADC* rightPot;
extern tMotor* left;
extern tMotor* right;
extern float leftMotor;
extern float rightMotor;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

void initialize(void){
  if (!initialized){
    leftPot = InitializeADC(PIN_B4);
    rightPot = InitializeADC(PIN_B5);
    left = InitializeServoMotor(PIN_E4, true);
    right = InitializeServoMotor(PIN_E5, false);
    SetMotor(left, 0);
    SetMotor(right, 0);
    initialized = true;
  }
}



// The 'main' function is the entry point of the program
int main(void) {
    InitializeGPIO();
    //PLL_Init(Bus80MHz);
		CallEvery(blink, 0, 0.5);
    initialize();
    TestSampling_Init();
    //Systick_Init();
    ST7735_InitR(INITR_REDTAB);
    User_Begin();
    
}
