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
    int delta_x=x_final-x_initial;
    int delta_y=y_final-y_initial;
    r = sqrt(delta_x^delta_x + delta_y^delta_y);
    int sigma = std::atan(delta_y/delta_x);
    pangle = M_PI/2 + cangle_initial - sigma;
    Serial.println(r);
    Serial.println(pangle);
}

void vgoto(void *Parameters_temp){
    TaskParams* Parameters2 = (TaskParams *) Parameters_temp;
    int angle= Parameters2->angle;
    int r =Parameters2->distance;
    tourner((int)angle,1,10);
    avancer((int)(r*3.2*5),0,1);

    vTaskDelete(NULL);
}

void GoToPosition::Go(){
    CalculPolar();
    //tourner((int)pangle, 1, 10);

}

