// filename ******** TestSampling.c ************** 
//
// Source code file for functions to sample our test controller
//
// Authors:          Shan and Santiago
//                   (we attribute this file to Prof Valvano)
// Date of Creation: October 30, 2016
// TA:               Dylan
// Unique number:    16620

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>

uint32_t UpButton = 0;
uint32_t DownButton = 0;
uint32_t SelectButton = 0;
uint32_t BackButton = 0;


void TestSampling_Check(void){
  tBoolean buttonValue = false;
  buttonValue = GetPin(PIN_E0);
  Wait(.02);
  if (buttonValue && GetPin(PIN_E0)) UpButton = 1;
  buttonValue = false;
  buttonValue = GetPin(PIN_E1);
  Wait(.02);
  if (buttonValue && GetPin(PIN_E1)) DownButton = 1;
  buttonValue = false;
  buttonValue = GetPin(PIN_E2);
  Wait(.02);
  if (buttonValue && GetPin(PIN_E2)) SelectButton = 1;
  buttonValue = false;
  buttonValue = GetPin(PIN_E3);
  Wait(.02);
  if (buttonValue && GetPin(PIN_E3)) BackButton = 1;
}

//****TestSampling_Init********
// Initializes PE0-3 as the test buttons of the controller
// Inputs: none
// Outputs: none
void TestSampling_Init(void){
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x010; 				 	//Activate clock Port E
  delay = SYSCTL_RCGCGPIO_R;      			// Phony delay
	delay = SYSCTL_RCGCGPIO_R;      			// Phony delay
	delay = SYSCTL_RCGCGPIO_R;      			// Phony delay
	delay = SYSCTL_RCGCGPIO_R;      			// Phony delay
	
  GPIO_PORTE_DIR_R &= ~0x0F;   					// inputs on PE3-0
  GPIO_PORTE_AFSEL_R &= ~0x0F; 					// regular function on PE3-0
  GPIO_PORTE_DEN_R |= 0x0F;    					// enable digital on PE3-0
}



//****Systick_Init********
// Initializes Systick for interrupts every 50 ms
// Inputs: none
// Outputs: none
void Systick_Init(void){
	NVIC_ST_CTRL_R = 0;         																						// disable SysTick during setup
  NVIC_ST_RELOAD_R = 4000000 - 1;																					// reload value for every 50 ms
  NVIC_ST_CURRENT_R = 0;      																						// any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; 							// priority 1
  NVIC_ST_CTRL_R = 0x0007; 																								// enable SysTick with core clock and interrupts
}


//****Systick_Handler********
// Reads data from test controller
// Inputs: none
// Outputs: none
void SysTick_Handler(void){
		//ADCLeft = ADC_InLeft();			// Place Sample in mailbox
		//ADCStatusLeft = 1;					// Signal new data in mailbox
		
		//ADCRight = ADC_InRight();
		//ADCStatusRight = 1;
		static uint32_t button0Before = 0x01;
		static uint32_t button1Before = 0x02;
		static uint32_t button2Before = 0x04;
		static uint32_t button3Before = 0x08;
	
		
		if( ((GPIO_PORTE_DATA_R & 0x01) == 0x01) && (button0Before == 0) ){
			DownButton = 1;
		}
		
		if( ((GPIO_PORTE_DATA_R & 0x02) == 0x02) && (button1Before == 0) ){
			UpButton = 1;
		}
		
		if( ((GPIO_PORTE_DATA_R & 0x04) == 0x04) && (button2Before == 0) ){
			SelectButton = 1;
		}
				
		if( ((GPIO_PORTE_DATA_R & 0x08) == 0x08) && (button3Before == 0) ){
			BackButton = 1;
		}		
		
		button0Before = GPIO_PORTE_DATA_R & 0x01;
		button1Before = GPIO_PORTE_DATA_R & 0x02;
		button2Before = GPIO_PORTE_DATA_R & 0x04;
		button3Before = GPIO_PORTE_DATA_R & 0x08;
		
		
}
