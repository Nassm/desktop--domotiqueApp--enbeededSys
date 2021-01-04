

#include "LSM6DSOSensor.h"
#include "mbed.h"
#include "XNucleoIKS01A3.h"
//#include <ros.h>




static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);
static LSM6DSOSensor *acc_gyro = mems_expansion_board->acc_gyro;

volatile int mems_event = 0;
volatile int toggle_tilt_enable = 0;
static int tilt_is_enabled = 1;
DigitalOut led = LED1;


void pressed_cb()
{
    toggle_tilt_enable = 1;
}

 

void int1_cb()
{
    mems_event = 1;
}


int main() {

acc_gyro->attach_int1_irq(&int1_cb);
    acc_gyro->enable_x();
    acc_gyro->enable_tilt_detection();

    while (1) {
        
        if (toggle_tilt_enable) {
            toggle_tilt_enable = 0;
            
            if (tilt_is_enabled == 0) 
            {
                acc_gyro->enable_tilt_detection();
                tilt_is_enabled = 1;
                
            }
            else 
            {
                acc_gyro->disable_tilt_detection();
                tilt_is_enabled = 0;      
            }
       }
        
        if (mems_event) {
           
                mems_event = 0;
                LSM6DSO_Event_Status_t status;
                acc_gyro->get_event_status(&status);
            
            if (status.TiltStatus) {
                printf("O\r\n");
                
                wait_ms(3000);
                
                printf("F\r\n");
                                
            }
        }
    }   
}
