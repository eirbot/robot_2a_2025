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
    float sigma = std::atan2(delta_y,delta_x) * RAD_TO_DEG;
    pangle = 90 + cangle_initial - sigma;
    pangleFin = -(cangle_initial - pangle + cangle_final);
    
    Serial.print("cangle_initial: ");
    Serial.print(cangle_initial);
    Serial.print("  pangle: ");
    Serial.print(pangle);
    Serial.print("  cangle_final: ");
    Serial.print(cangle_final);
    Serial.print("  pangleFin: ");
    Serial.println(pangleFin);
    
}

void GoToPosition::Go(float x_f,float y_f,float cangle_f){

    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    CalculPolar();

    Serial.print("r :");
    Serial.print((int)r);
    Serial.print("   pangle: ");
    Serial.print(pangle);
    Serial.print("   pangleFin: ");
    Serial.println(pangleFin);

    TaskParams Params;

    Params = {0, (int)((pangle)), (pangle > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    Params = {(int)r, 0, 0, 600};
    mot.EnvoyerDonnees(&Params);

    Params = {0, (int)((pangleFin)), (pangleFin > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    x_initial = x_final;
    y_initial = y_final;
    cangle_initial = cangle_final;
}

