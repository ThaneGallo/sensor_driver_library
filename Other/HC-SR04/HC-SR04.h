#ifndef HC_SR04_H
#define HC_SR04_H

#include <stdint.h>

// Header file for HC-SR04 Ultrasonic Sensor

// Define the trigger and echo pin numbers
#define TRIGGER_PIN   9  // Trigger pin connected to digital pin 9
#define ECHO_PIN      10 // Echo pin connected to digital pin 10

#define METRIC 0x00
#define IMPERIAL 0x01

// Function prototypes
float HC_SR04_getDistance(uint8_t measurement_system);              // Get the distance 

#endif // HC_SR04_H
