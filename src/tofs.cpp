#include "tofs.h"

volatile bool frontClear_tof = true; // Define the variable here
volatile bool backClear_tof = true;  // Define the variable here

void readTofs(void *Parameters_temp){
    // tofQueue = xQueueCreate(1, sizeof(int));

    Adafruit_VL53L0X frontTof = Adafruit_VL53L0X();

    if (!frontTof.begin()) {
        // Serial.println(F("Failed to boot VL53L0X"));
    }

    VL53L0X_RangingMeasurementData_t measure;

    while(1){
        frontTof.rangingTest(&measure, false);
        if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
            // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
            frontClear_tof = false;
        }
        else{
            frontClear_tof = true;
        }
        // Serial.println(measure.RangeMilliMeter);
        vTaskDelay(100);
    }
}