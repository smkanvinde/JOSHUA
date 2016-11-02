// filename ******** TestSampling.h ************** 
//
// Header file for functions to sample our test controller
//
// Authors:          Shan and Santiago
//                   (we attribute this file to Prof Valvano)
// Date of Creation: October 30, 2016
// TA:               Dylan
// Unique number:    16620


//****TestSampling_Init********
// Initializes PE0-3 as the test buttons of the controller
// Inputs: none
// Outputs: none
void TestSampling_Init(void);


//****Systick_Init********
// Initializes Systick at 20Hz
// Inputs: none
// Outputs: none
void Systick_Init(void);


//****Systick_Handler********
// Reads data from test controller
// Inputs: none
// Outputs: none
void SysTick_Handler(void);

void TestSampling_Check(void);