// filename ******** PSX.h ************** 
//
// Header file for functions to interface with
// a PSX controller
//
// Authors:          Shan and Santiago
//                   (we attribute this file to Prof Valvano)
// Date of Creation: November 2, 2016
// TA:               Dylan
// Unique number:    16620


// *****PSX_Initialize*********
// Initializes the controller
// Input: none
// Output: none
void PSX_Initialize(void);


// *****PSX_Poll*********
// Poll the controller and set globals
// associated with each button
// Input: none
// Output: none
void PSX_Poll(void);

// *****PSX_Clear*********
// Ack/clear buttons associated with each button
// Input: none
// Output: none
void PSX_Clear(void);
