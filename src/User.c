// filename ******** User.c ************** 
//
// Source code file for functions to provide a user
// interface for our project
//
// Authors:          Shan and Santiago
//                   (we attribute this file to Prof Valvano)
// Date of Creation: October 30, 2016
// TA:               Dylan
// Unique number:    16620

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "User.h"
#include <math.h>
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <StellarisWare/driverlib/gpio.h>
//#include "PLL.h"
#include "TestSampling.h"
#include "PSX.h"

#define FREESTYLE  4
#define JOSHSTYLE  6
#define SPEEDX     7
#define DIRECTIONX 11

extern uint32_t Lx, Ly, Rx, Ry,
       X, Square, Triangle, Circle,
       Up, Down, Right, Left,
       L1, L2, R1, R2;
extern uint32_t BackButton, SelectButton, UpButton, DownButton;
uint32_t leftVal;
uint32_t rightVal;
tADC* leftPot;
tADC* rightPot;
tMotor* left;
tMotor* right;
float leftMotor;
float rightMotor;

///*extern*/ float LeftMotor, RightMotor;


void User_DriveMotors(uint32_t leftValue, uint32_t rightValue){
  // if (leftValue == 127){
       // SetMotor(left, leftMotor/2);
       // Wait(0.05);
  //      SetMotor(left, 0);
  //  }
    if (leftValue > 127){
      leftMotor = (float)(leftValue - 127) / 128;
      SetMotor(left, leftMotor);
    }
    else if (leftValue < 127){
      leftMotor = (float)(127 - leftValue) / 127 * -1;
      SetMotor(left, leftMotor);
    }
    else if (leftValue == 127){
        while (leftMotor >  0){
            leftMotor = leftMotor - 0.001;
            SetMotor(left, leftMotor);
           // Wait(0.001);
        }
        while (leftMotor < 0){
            leftMotor = leftMotor + 0.001;
            SetMotor(left, leftMotor);
           // Wait(0.001);
        }
        
    }
  // else if (leftValue == 127 && rightValue == 127){
  //     SetMotor(left, 0);
  //     Wait(0.1);
  //     SetMotor(right,0);
  // }
   // else {
   //   SetMotor(left, 0);
   // }
    
   // if (rightValue == 127){
     // SetMotor(right, rightMotor/2);
     // Wait(0.05);
   //   SetMotor(right, 0);
   // }
    if (rightValue >= 127){
      rightMotor = (float)(rightValue - 127) / 128;
      SetMotor(right, rightMotor);
    }
    else if (rightValue < 127){
      rightMotor = (float)(127 - rightValue) / 128 * -1;
      SetMotor(right, rightMotor);
    }
    else if (rightValue == 127){
        while (rightMotor > 0){
           rightMotor = rightMotor - 0.001;
            SetMotor(right, rightMotor);
           // Wait(0.001);
        }
        while (rightMotor < 0){
            rightMotor = rightMotor + 0.001;
            SetMotor(right, rightMotor);
           // Wait(0.001);
        }
    }
    // else if (leftValue == 127 && rightValue == 127){
    //   SetMotor(left, 0);
    //   Wait(0.1);
    //   SetMotor(right,0);
    // }

   // else {
    //  SetMotor(right, 0);
   // }
}

// *****User_Begin*********
// Initializes the user interface
// Input: none
// Output: none
void User_Begin(void){
  uint32_t mode;
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Welcome to Joshua");
	ST7735_SetCursor(0, 2);
	ST7735_OutString("What to do?");
	ST7735_SetCursor(3, 4);
	ST7735_OutString("Free Styling");
	ST7735_SetCursor(3, 6);
	ST7735_OutString("Joshua's Style");			// set up the main messages
	
	mode = FREESTYLE;
	ST7735_SetCursor(0, FREESTYLE);
	ST7735_OutString("->");									// set original location of selector at free styling
	
	while(1){
    //TestSampling_Check();
    PSX_Poll();
   // if (Up == 255 && Left == 255 && Down && 255 && Right == 255){
   //     PSX_Clear();
   // }
		if(Down > 0){
			Down = 0;											// acknowdledge down button
			if(mode == FREESTYLE){
				mode = JOSHSTYLE;
				ST7735_SetCursor(0, FREESTYLE);
				ST7735_OutString("  ");						// erase the arrow at free style
				
				ST7735_SetCursor(0, JOSHSTYLE); 
				ST7735_OutString("->");						// put the arrow at josh style
			}
			else if(mode == JOSHSTYLE){
				mode = FREESTYLE;									
				ST7735_SetCursor(0, JOSHSTYLE);
				ST7735_OutString("  ");						// erase the arrow at josh style
				
				ST7735_SetCursor(0, FREESTYLE); 
				ST7735_OutString("->");						// put the arrow at free style
			}
		}
		if(Up > 0){
			Up = 0;											// acknowdledge up button
			if(mode == FREESTYLE){
				mode = JOSHSTYLE;
				ST7735_SetCursor(0, FREESTYLE);
				ST7735_OutString("  ");						// erase the arrow at free style
				
				ST7735_SetCursor(0, JOSHSTYLE); 
				ST7735_OutString("->");						// put the arrow at josh style
			}
			else if(mode == JOSHSTYLE){
				mode = FREESTYLE;									
				ST7735_SetCursor(0, JOSHSTYLE);
				ST7735_OutString("  ");						// erase the arrow at josh style
				
				ST7735_SetCursor(0, FREESTYLE); 
				ST7735_OutString("->");						// put the arrow at free style
			}
		}	
		if(X > 0){
			X = 0;
			if(mode == FREESTYLE){
				User_FreeStyle();
			}
			else if(mode == JOSHSTYLE){
				// User_JoshStyle();							// go to corresponsing style if a selection is made
			}
			
			ST7735_FillScreen(0);              			
			ST7735_SetCursor(0, 0);
			ST7735_OutString("Welcome to Joshua");
			ST7735_SetCursor(0, 2);
			ST7735_OutString("What to do?");
			ST7735_SetCursor(3, 4);
			ST7735_OutString("Free Styling");
			ST7735_SetCursor(3, 6);
			ST7735_OutString("Joshua's Style");			
			ST7735_SetCursor(0, mode);
			ST7735_OutString("->");									// restore screen after returning from selection
		}
	//	UpButton = 0;
	//	DownButton = 0;
	//	SelectButton = 0;
	//	BackButton = 0;
        PSX_Clear();                                                // clear all buttons
    	}
   
}


// *****User_FreeStyle*********
// Sets the free style mode
// Input: none
// Output: none
void User_FreeStyle(void){
	ST7735_FillScreen(0);              																// black screen
	ST7735_SetCursor(0, 0);									
	ST7735_OutString("Free Style:");
	ST7735_SetCursor(0, 1);	
	ST7735_OutString("Drive on!");																		// welcome to free style message
	
	ST7735_SetCursor(0, 3);
	ST7735_OutString("Left:    % of max");
	ST7735_SetCursor(0, 5);
	ST7735_OutString("Right:   % of max");														
	ST7735_SetCursor(0, 7);
	ST7735_OutString("Direction: ");																	// lay out for user info
	
	while(Circle == 0){
    
		uint32_t leftP100 = 0;
		uint32_t rightP100 = 0;
		uint32_t prevLeftP100 = 1;
		uint32_t prevRightP100 = 1;																			// variables for displaying speed percentages
   // TestSampling_Check();
      PSX_Poll();
		
		leftP100 = (uint32_t) fabs(leftMotor) * 100.0;
		rightP100 = (uint32_t) fabs(rightMotor) * 100.0;								// calculate percentage of max
		
		if(leftP100 != prevLeftP100){																		// update left percentage if it changed
			ST7735_SetCursor(SPEEDX, 3);
			ST7735_OutUDec(leftP100);
		}
		if(rightP100 != prevRightP100){																	// update right percentage if it changed
			ST7735_SetCursor(SPEEDX, 5);
			ST7735_OutUDec(rightP100);
		}
		
		prevLeftP100 = leftP100;
		prevRightP100 = rightP100;																			// update the previous values
		
		// code to control motors from ADC
   // leftValue = ADCRead(leftPot);
   // rightValue = ADCRead(rightPot);
      leftVal = Ly;
      rightVal =  Ry;

    User_DriveMotors(leftVal, rightVal);
	}
    SetMotor(left, 0);
    SetMotor(right, 0);
    PSX_Clear();
//		UpButton = 0;
//		DownButton = 0;
//		SelectButton = 0;
//		BackButton = 0;																									// clear all buttons
}
