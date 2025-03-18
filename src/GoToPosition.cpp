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
    float sigma = std::atan2(delta_y,delta_x);
    pangle = M_PI/2 + cangle_initial - sigma;
}

void GoToPosition::Go(float x_f,float y_f,float cangle_f){

    pinMode(STEPD,OUTPUT);
    pinMode(STEPG,OUTPUT);

    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    CalculPolar();

    Serial.print("    r :");
    Serial.print((int)r);
    Serial.print("   pangle: ");
    Serial.println((int)((pangle)*180.0/M_PI));

    TaskParams Params;

    if(pangle>=0){
        Serial.println("pangle <= 0");
        Params = {0, (int)((pangle)*180.0/M_PI), 0, 10};
        mot.EnvoyerDonnees(&Params);
    }
    else{
        Serial.println("pangle > 0");
        Params = {0, (int)((pangle)*180.0/M_PI), 1, 10};
        mot.EnvoyerDonnees(&Params);
    }
    Params = {(int)r, 0, 0, 10};
    mot.EnvoyerDonnees(&Params);
}

