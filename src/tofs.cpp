#include "tofs.h"

QWIICMUX Mux;

volatile bool frontClear_tof = true; // Define the variable here
volatile bool backClear_tof = true;  // Define the variable here

void readTofs(void *Parameters_temp){
    tofQueue = xQueueCreate(1, sizeof(int));

    Adafruit_VL53L0X frontTof = Adafruit_VL53L0X();

    if (!frontTof.begin()) {
        // Serial.println(F("Failed to boot VL53L0X"));
    }

    Adafruit_VL53L0X frontTopTof = Adafruit_VL53L0X();
    Adafruit_VL53L0X frontRightTof = Adafruit_VL53L0X();
    Adafruit_VL53L0X frontLeftTof = Adafruit_VL53L0X();
    Adafruit_VL53L0X backTopTof = Adafruit_VL53L0X();
    Adafruit_VL53L0X backRightTof = Adafruit_VL53L0X();
    Adafruit_VL53L0X backLeftTof = Adafruit_VL53L0X();

    // Mux.setPort(6); // Front Top
    // if (!frontTopTof.begin()) {
    //     Serial.println("Failed to boot front top VL53L0X");
    // }

    Mux.setPort(2); // Front Right
    if (!frontRightTof.begin()) {
        Serial.println("Failed to boot front right VL53L0X");
    }

    // Mux.setPort(5); // Front Left
    // if (!frontLeftTof.begin()) {
    //     Serial.println("Failed to boot front left VL53L0X");
    // }

    // Mux.setPort(7); // Back Top
    // if (!backTopTof.begin()) {
    //     Serial.println("Failed to boot back top VL53L0X");
    // }

    // Mux.setPort(3); // Back Right
    // if (!backRightTof.begin()) {
    //     Serial.println("Failed to boot back right VL53L0X");
    // }

    // Mux.setPort(4); // Back Left
    // if (!backLeftTof.begin()) {
    //     Serial.println("Failed to boot back left VL53L0X");
    // }

    VL53L0X_RangingMeasurementData_t measure;

    while(1){
        if(FLAG_TOF){
            // Mux.setPort(6); // Front Top
            // frontTopTof.rangingTest(&measure, false);
            // if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
            //     // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
            //     frontClear_tof = false;
            // }
            // else{
            //     frontClear_tof = true;
            // }
            // Serial.print("Front Top: ");
            // Serial.println(measure.RangeMilliMeter);

            Mux.setPort(2); // Front Right
            frontRightTof.rangingTest(&measure, false);
            if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
                // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
                frontClear_tof = false;
            }
            else{
                frontClear_tof = true;
            }
            Serial.print("Front Right: ");
            Serial.println(measure.RangeMilliMeter);

        //     Mux.setPort(5); // Front Left
        //     frontLeftTof.rangingTest(&measure, false);
        //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
        //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
        //         frontClear_tof = false;
        //     }
        //     else{
        //         frontClear_tof = true;
        //     }
        //     Serial.print("Front Left: ");
        //     Serial.println(measure.RangeMilliMeter);

        //     Mux.setPort(7); // Back Top
        //     backTopTof.rangingTest(&measure, false);
        //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
        //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
        //         backClear_tof = false;
        //     }
        //     else{
        //         backClear_tof = true;
        //     }
        //     Serial.print("Back Top: ");
        //     Serial.println(measure.RangeMilliMeter);

        //     Mux.setPort(3); // Back Right
        //     backRightTof.rangingTest(&measure, false);
        //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
        //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
        //         backClear_tof = false;
        //     }
        //     else{
        //         backClear_tof = true;
        //     }
        //     Serial.print("Back Right: ");
        //     Serial.println(measure.RangeMilliMeter);

        //     Mux.setPort(4); // Back Left
        //     backLeftTof.rangingTest(&measure, false);
        //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
        //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
        //         backClear_tof = false;
        //     }
        //     else{
        //         backClear_tof = true;
        //     }
        //     Serial.print("Back Left: ");
        //     Serial.println(measure.RangeMilliMeter);
        }
        vTaskDelay(100);
    }
}