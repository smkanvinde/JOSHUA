#include "../../Rasware/RASLib/inc/common.h"
#include "../../Rasware/RASLib/inc/motor.h"
#include "../../Rasware/RASLib/inc/gpio.h"
#include "../../Rasware/RASLib/inc/spi.h"
#include "../../Rasware/RASLib/inc/time.h"
#include "../../Rasware/RASLib/inc/uart.h"
#include <stdint.h>
#include <stdio.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_sysctl.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/inc/hw_watchdog.h>
#include <StellarisWare/driverlib/watchdog.h>

static int ledState = 0;
static tSPI * spi;
uint32_t Lx, Ly, Rx, Ry, X, Square, Triangle, Circle,
         Up, Down, Left, Right, L1, L2, R1, R2;

void ToggleLED (void) {
        SetPin(PIN_F1, ledState & 1);
        ++ledState;
}

uint8_t reverse_byte (uint8_t toRev) {
        uint32_t toRet;
       asm volatile ("rbit %1, %0;": "=r"(toRet): "r" (toRev));
        return (toRet >> 24) & 0xFF ;
}

void reverse_array (uint32_t * array, int size){
        for (int i = 0; i < size; ++i)
                array[i] = reverse_byte(array[i]);
}

#define mode_byte (1)

int send_packet_PS2(uint32_t * packet, int size, uint32_t * response) {
        int  buff_size = 3;
        SetPin(PIN_B2, 0);
        SPIRequestUS(spi, -1, &packet[0], 3 , &response[0], 3, 16);
        reverse_array (response, 3);
        buff_size += 2 * (response[mode_byte] & 0x0F);
        SPIRequestUS(spi, -1, &packet[3], size - 3 , &response[3], buff_size - 3, 16);
        reverse_array (&response[3], buff_size - 3);
        SetPin(PIN_B2, 1);
        return buff_size;
}

#define d_buf_size (37)
uint32_t poll[3] = {0x01, 0x42, 0x00};
uint32_t poll_and_escape[5] = {0x01, 0x43, 0x00, 0x01, 0x00};
uint32_t set_dualshock_mode[9] = { 0x01, 0x44, 0x00, 0x01, 0x03
                                   , 0x00, 0x00, 0x00, 0x00};
uint32_t set_analog_button_mode[9] = { 0x01, 0x4f, 0x00, 0xFF, 0xFF
                                       , 0x03, 0x00, 0x00, 0x00};
uint32_t leave_escape[5] = {0x01, 0x43, 0x00, 0x00, 0x00};
#define arraySizeof(arry) (sizeof(arry) / sizeof(arry[0]))

uint32_t data[d_buf_size] = {0x00,};
uint32_t dataCheck[d_buf_size] = {0x00,};
 
 

void PSX_Initialize(void){
//        uint32_t data[d_buf_size] = {0x00,};
       // tUART * uart4 = InitializeUARTModule(4, 115200);
       spi = InitializeSPI(PIN_D0, PIN_D3, PIN_D2, 250000, 8, true, true);
      //  spi = InitializeSPI(PIN_A2, PIN_A5, PIN_A4, 250000, 8, true, true);
        
        //int i = 0,j;
        reverse_array(&poll[0],
                     arraySizeof(poll));
        reverse_array(&poll_and_escape[0],
                      arraySizeof(poll_and_escape));
        reverse_array(&set_dualshock_mode[0],
                      arraySizeof(set_dualshock_mode));
        reverse_array(&set_analog_button_mode[0],
                      arraySizeof(set_analog_button_mode));
        reverse_array(&leave_escape[0],
                      arraySizeof(leave_escape));
        CallEvery(ToggleLED, 0, 0.25);
        do {
              // Printf("Trying to turn on the Controller\t %03d\n", data[1]);
               Printf("Powering on %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d" 
                      " %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d"
                      " %03d\n", data[0], data[1], data[2], data[3], data[4],
                       data[5], data[6], data[7], data[8], data[9], data[10],
                       data[11], data[12], data[13], data[14], data[15],
                       data[16], data[17], data[18], data[19], data[20]);   
                send_packet_PS2(&poll_and_escape[0],
                                arraySizeof(poll_and_escape), &data[0]);
                Wait(0.01);
                send_packet_PS2(&set_dualshock_mode[0],
                                arraySizeof(set_dualshock_mode), &data[0]);
                Wait(0.01);
                send_packet_PS2(&set_analog_button_mode[0],
                               arraySizeof(set_analog_button_mode), &data[0]);
                Wait(0.01);
                send_packet_PS2(&leave_escape[0],
                                arraySizeof(leave_escape), &data[0]);
                Wait(0.01);
                send_packet_PS2(&poll[0],
                                arraySizeof(poll), &data[0]);
        } while (data[1] != 0x79);
        Wait(0.2);
		for (int i = 0; i < d_buf_size; i++){
            dataCheck[i] = data[i];
        }

}

void PSX_Poll(void) {
         int buff_size = send_packet_PS2(&poll[0],
                  arraySizeof(poll), &data[0]);
         if (buff_size == 21)
                Printf("Lx %03d Ly %03d Rx %03d Ry %03d "
                    "X %03d Square %03d Triangle %03d Circle %03d "
                    "Up %03d Down %03d Left %03d Right %03d "
                     "L1 %03d L2 %03d R1 %03d R2 %03d\r",
                    data[7], data[8], data[5], data[6],
                    data[15], data[16], data[13], data[14],
                    data[11], data[12], data[10], data[9],
                    data[17], data[19], data[18], data[20]);
		if (data[7] != dataCheck[7] ||
                data[8] != dataCheck[8] ||
                data[5] != dataCheck[5] ||
                data[6] != dataCheck[6]){
            WatchdogReloadSet(WATCHDOG_BASE, 25000000);
        }

        Lx = data[7];
        Ly = data[8];
        Rx = data[5];
        Ry = data[6];
        X = data[15];
        Square = data[16];
        Triangle = data[13];
        Circle = data[14];
        Up = data[11];
        Down = data[12];
        Left = data[10];
        Right = data[9];
        L1 = data[17];
        L2 = data[19];
        R1 = data[18];
        R2 = data[20];
		for (int i = 0; i < d_buf_size; i++){
            dataCheck[i] = data[i];
        }

}
void PSX_Clear(void){
    Lx = 128;
    Ly = 127;
    Rx = 128;
    Ry = 127;
    X = 0;
    Square = 0;
    Triangle = 0;
    Circle = 0;
    Up = 0;
    Left = 0;
    Right = 0;
    Down = 0;
    L1 = 0;
    L2 = 0;
    R1 = 0;
    R2 = 0;
}

