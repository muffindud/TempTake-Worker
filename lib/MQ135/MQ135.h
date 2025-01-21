#ifndef MQ135_H
#define MQ135_H

#include <Arduino.h>
#include <driver/adc.h>

#define PARA 116.6020682
#define PARB 2.769034857
#define CORA .00035
#define CORB .02718
#define CORC 1.39538
#define CORD .0018
#define CORE -.003333333
#define CORF -.001923077
#define CORG 1.130128205
#define ATMOCO2 422.5
#define RLOAD_KOHM 10.0
#define ADC_RESOLUTION 4095.0

class MQ135{
    private:
        int pin;
        float rZero;
    public:
        /*
        * @brief Main MQ135 class constructor
        * @param pin: The pin where the MQ135 sensor is connected
        * @param rZero: The RZero value of the MQ135 sensor
        */
        MQ135(int pin, float rZero);

        /*
        * @brief Get the resistance of the MQ135 sensor
        * @param temperature_c: The temperature in Celsius
        * @param humidity_perc: The humidity in percentage
        * @return The resistance of the MQ135 sensor
        */
        float getRes(float temperature_c, float humidity_perc);

        /*
        * @brief Get the PPM of the MQ135 sensor
        * @param temperature_c: The temperature in Celsius
        * @param humidity_perc: The humidity in percentage
        * @return The PPM of the MQ135 sensor
        */
        float getPPM(float temperature_c, float humidity_perc);

        /*
        * @brief Get the RZero of the MQ135 sensor
        * @param temperature_c: The temperature in Celsius
        * @param humidity_perc: The humidity in percentage
        * @return The RZero of the MQ135 sensor
        */
        float getRZero(float temperature_c, float humidity_perc);
};

#endif
