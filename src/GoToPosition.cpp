#include "GoToPosition.hpp"


GoToPosition::GoToPosition(const float &x_i,const float &y_i,const float &cangle_i, const float &x_f,const float &y_f,const float &cangle_f){
    x_initial = x_i;
    y_initial = y_i;
    cangle_initial = cangle_i;
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
}

void GoToPosition::CalculPolar(){
    float delta_x=x_final-x_initial;
    float delta_y=y_final-y_initial;
    r = sqrt(delta_x*delta_x + delta_y*delta_y);
    float sigma = std::atan(delta_y/delta_x);
    pangle = M_PI/2 + cangle_initial - sigma;
}

// void vgoto(void *Parameters_temp){
//     TaskParams* Parameters2 = (TaskParams *) Parameters_temp;
//     float angle= Parameters2->angle;
//     float r =Parameters2->distance;
//     tourner((int)angle,1,10);
//     avancer((int)(r*3.2*5),0,1);

//     vTaskDelete(NULL);
// }

void GoToPosition::Go(float x_f,float y_f,float cangle_f){
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    Serial.println(x_final);
    Serial.println(y_final);
    Serial.println(cangle_final);

    CalculPolar();
    tourner((int)((pangle)*180.0/M_PI), 0, 10);
    vTaskDelay(2000);
    avancer((int)r,0,10);
}

