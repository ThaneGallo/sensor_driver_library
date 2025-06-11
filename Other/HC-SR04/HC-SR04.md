# HC-SR04
An 5v ultrasonic distance sensor with a range of 2cm to 4m and a resolution of 3mm.
Can be purchased [here](https://www.sparkfun.com/ultrasonic-distance-sensor-hc-sr04.html)


## How to use
Within the .h file change the echo and trigger pins to the desired ones and then simply call HC_SR04_getDistance(uint8_t measurement_system); Currently only supports METRIC and IMPERIAL and returns values in the scale of cm or in depending on system chosen.


## Development 
This driver was relatively simple to write as it just polls the state of two different pins first sending an on condition in the form of the trigger pin then waiting for the echo pin to toggle. Polling was chosen rather than some sort of interupt handiling as the sensor does not perform any sort of real time function.

