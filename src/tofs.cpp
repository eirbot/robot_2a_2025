#include "tofs.h"

volatile bool frontClear_tof = true; // Define the variable here
volatile bool backClear_tof = true;  // Define the variable here

VL53L0X sensor[NB_TOFS];
int xshutPins[6] = {0, 1, 4, 5, 6, 7}; // GPIO reliés aux XSHUT

bool checkClear(int start, int end, int stop_distance) {
    for (int i = start; i <= end; i++) {
        int distance = sensor[i].readRangeContinuousMillimeters();
        // Serial.print("Tof ");
        // Serial.print(i);
        // Serial.print(": ");
        // Serial.println(distance);
        if (distance < STOP_DISTANCE) {
            return false;
        }
    }
    return true;
}

void readTofs(void *Parameters_temp){
    Wire.begin(-1, -1, 400000);

    // Serial.println("Tofs initialisation demaree");

    // Désactiver tous les capteurs
    for (int i = 0; i < NB_TOFS; i++) {
        pcf8575.pinMode(xshutPins[i], OUTPUT);
        pcf8575.digitalWrite(xshutPins[i], LOW);
    }

    vTaskDelay(10);

    for (int i = 0; i < NB_TOFS; i++) {
        pcf8575.digitalWrite(xshutPins[i], HIGH);
        vTaskDelay(10);
        sensor[i].init();
        sensor[i].setAddress(0x31 + i);
    }

    for (int i = 0; i < NB_TOFS; i++) {
        sensor[i].startContinuous(); // ou prepare pour single-shot
    }

    // Serial.println("Tofs initialisation finie");

    while(1){
        if(FLAG_TOF){
            frontClear_tof = checkClear(0, 2, STOP_DISTANCE); // Vérifie les capteurs avant (0, 1, 2)
            vTaskDelay(10);
            backClear_tof = checkClear(3, NB_TOFS-1, STOP_DISTANCE); // Vérifie les capteurs arrière (3, 4, 5)            
        }
        vTaskDelay(100);
    }
}


//######################################################################################################################################

// #include "tofs.h"

// QWIICMUX Mux;

// Adafruit_VL53L0X frontTopTof = Adafruit_VL53L0X();
// Adafruit_VL53L0X frontRightTof = Adafruit_VL53L0X();
// Adafruit_VL53L0X frontLeftTof = Adafruit_VL53L0X();
// Adafruit_VL53L0X backTopTof = Adafruit_VL53L0X();
// Adafruit_VL53L0X backRightTof = Adafruit_VL53L0X();
// Adafruit_VL53L0X backLeftTof = Adafruit_VL53L0X();

// volatile bool frontClear_tof = true; // Define the variable here
// volatile bool backClear_tof = true;  // Define the variable here

// void readTofs(void *Parameters_temp){
//     tofQueue = xQueueCreate(1, sizeof(int));

//     if (!Mux.begin()) {
//         Serial.println(F("Failed to boot VL53L0X"));
//     }



//     // Mux.setPort(6); // Front Top
//     // if (!frontTopTof.begin()) {
//     //     Serial.println("Failed to boot front top VL53L0X");
//     // }

//     Mux.setPort(2); // Front Right
//     if (!frontRightTof.begin()) {
//         Serial.println("Failed to boot front right VL53L0X");
//     }

//     // Mux.setPort(5); // Front Left
//     // if (!frontLeftTof.begin()) {
//     //     Serial.println("Failed to boot front left VL53L0X");
//     // }

//     // Mux.setPort(7); // Back Top
//     // if (!backTopTof.begin()) {
//     //     Serial.println("Failed to boot back top VL53L0X");
//     // }

//     // Mux.setPort(3); // Back Right
//     // if (!backRightTof.begin()) {
//     //     Serial.println("Failed to boot back right VL53L0X");
//     // }

//     // Mux.setPort(4); // Back Left
//     // if (!backLeftTof.begin()) {
//     //     Serial.println("Failed to boot back left VL53L0X");
//     // }

//     VL53L0X_RangingMeasurementData_t measure;

//     while(1){
//         if(FLAG_TOF){
//             // Mux.setPort(6); // Front Top
//             // frontTopTof.rangingTest(&measure, false);
//             // if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//             //     // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//             //     frontClear_tof = false;
//             // }
//             // else{
//             //     frontClear_tof = true;
//             // }
//             // Serial.print("Front Top: ");
//             // Serial.println(measure.RangeMilliMeter);

//             Mux.setPort(2); // Front Right
//             frontRightTof.rangingTest(&measure, false);
//             if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//                 // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//                 frontClear_tof = false;
//             }
//             else{
//                 frontClear_tof = true;
//             }
//             Serial.print("Front Right: ");
//             Serial.println(measure.RangeMilliMeter);

//         //     Mux.setPort(5); // Front Left
//         //     frontLeftTof.rangingTest(&measure, false);
//         //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//         //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//         //         frontClear_tof = false;
//         //     }
//         //     else{
//         //         frontClear_tof = true;
//         //     }
//         //     Serial.print("Front Left: ");
//         //     Serial.println(measure.RangeMilliMeter);

//         //     Mux.setPort(7); // Back Top
//         //     backTopTof.rangingTest(&measure, false);
//         //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//         //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//         //         backClear_tof = false;
//         //     }
//         //     else{
//         //         backClear_tof = true;
//         //     }
//         //     Serial.print("Back Top: ");
//         //     Serial.println(measure.RangeMilliMeter);

//         //     Mux.setPort(3); // Back Right
//         //     backRightTof.rangingTest(&measure, false);
//         //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//         //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//         //         backClear_tof = false;
//         //     }
//         //     else{
//         //         backClear_tof = true;
//         //     }
//         //     Serial.print("Back Right: ");
//         //     Serial.println(measure.RangeMilliMeter);

//         //     Mux.setPort(4); // Back Left
//         //     backLeftTof.rangingTest(&measure, false);
//         //     if (measure.RangeMilliMeter < stop_distance && measure.RangeStatus == 0){
//         //         // xQueueSend(tofQueue, &measure.RangeMilliMeter, 10);
//         //         backClear_tof = false;
//         //     }
//         //     else{
//         //         backClear_tof = true;
//         //     }
//         //     Serial.print("Back Left: ");
//         //     Serial.println(measure.RangeMilliMeter);
//         }
//         vTaskDelay(100);
//     }
// }