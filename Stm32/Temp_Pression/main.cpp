#include "LPS22HHSensor.h"
#include "HTS221Sensor.h"
#include "mbed.h"
#include "XNucleoIKS01A3.h"
 
/* Instantiate the expansion board */
static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);
 
/* Retrieve the composing elements of the expansion board */
static STTS751Sensor *t_sensor = mems_expansion_board->t_sensor;
static LPS22HHSensor *pt_sensor = mems_expansion_board->pt_sensor;
 
DigitalOut myled(LED1);
 
float temperature = 0.0f;
float pressure = 0.0f;

 
/* Simple main function */
int main()
{

    /* Enable temperature sensor */
    t_sensor->enable();
 
    /* Enable pressure sensor */
    pt_sensor->enable();

    printf("\r\n--- Starting new measures ---\r\n");
 
    while (1) {

           
            t_sensor->get_temperature(&temperature);
            pt_sensor->get_pressure(&pressure);
            wait(0.2);
            printf("N: Chambre");
            printf(" T: ");
            printf("%3f",temperature);
            //wait(0.2);
            printf(" P: ");
            printf("%3f \n\r", pressure);
            wait(0.2);
        
    }

    //return 0;
}
 
