#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <StellarisWare/driverlib/gpio.h>
#include <stdint.h>
#include "User.h"
#include "TestSampling.h"
#include "ST7735.h"
#include <math.h>
#include "PSX.h"
/*#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_sysctl.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/inc/hw_watchdog.h>
#include <StellarisWare/driverlib/watchdog.h>*/
#include <RASLib/inc/timeout.h>


tBoolean blink_on = true;
tBoolean initialized = false;
int tid;

extern float leftValue;
extern float rightValue;
extern tMotor* left;
extern tMotor* right;
extern float leftMotor;
extern float rightMotor;
extern tADC* leftPot;
extern tADC* rightPot;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

/*void WatchdogHandler(void) {
    WatchdogIntClear(WATCHDOG_BASE);
    WatchdogResetDisable(WATCHDOG_BASE);

   SetMotor(left, 0);
   SetMotor(right, 0);
 //   SetPin(PIN_F1, true);
}

void WatchDog_Init(void) {
//   SetPin(PIN_F1, false);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);  // Watchdog timer
    WatchdogIntClear(WATCHDOG_BASE);
    //WatchdogStallEnable(WATCHDOG_BASE);   // Enable for breakpoint debugging
    if(WatchdogLockState(WATCHDOG_BASE) == true) {
        WatchdogUnlock(WATCHDOG_BASE);
    }

    WatchdogIntRegister(WATCHDOG_BASE, WatchdogHandler);

    //Motor control node communicates at 10Hz, if no command for 20Hz halts
    WatchdogReloadSet(WATCHDOG_BASE, 25000000);   

    WatchdogIntEnable(WATCHDOG_BASE);
    WatchdogResetDisable(WATCHDOG_BASE);   //Second interrupt doesn't result processor

    WatchdogEnable(WATCHDOG_BASE);
}*/

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
	CallEvery(blink, 0, 0.5);
    initialize();
   // WatchDog_Init();
    //TestSampling_Init();
   InitializeSystemTimeout();
   int tid = CallOnTimeout(User_StopMotors, NULL, 0.1f);
    PSX_Initialize();
    ST7735_InitR(INITR_REDTAB);
    //User_Begin();
    User_FreeStyle();
    
}
