
#include "HC-SR04.h"

void HC_SR04_init(); // Initialize the sensor (set pin modes)

float HC_SR04_getDistance(uint8_t measurement_system)
{
    uint64_t end_trigger, echo_start;

    //sends initial trigger 
    HAL_GPIO_WritePin(GPIOA, TRIGGER_PIN, GPIO_PIN_SET);
    // wait 10 us
    HAL_GPIO_WritePin(GPIOA, TRIGGER_PIN, GPIO_PIN_RESET);
    end_trigger = HAL_GET_TICK();


    //sends out 8 cycle sonic burst at 40kHz



    while(HAL_GPIO_READ_PIN(GPIOA, ECHO_PIN) != 0){
        // wait 60ms
    }
    echo_start = HAL_GET_TICK();

    if(measurement_system == METRIC){
        return (end_trigger - echo_start) / 58.0; // cm
    }
    else{
        return (end_trigger - echo_start) / 148.0; // in
    }


   
  
}