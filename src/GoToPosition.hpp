#ifndef GOTOPOSITION_H
#define GOTOPOSITION_H

#include "Arduino.h"
#include <cmath>
#include <iostream>
#include "common.h"
#include "motors.h"



class GoToPosition{

    public:
    GoToPosition(const float &x_i,const float &y_i,const float &cangle_i, const float &x_f,const float &y_f,const float &cangle_f);
    float x_initial;
    float y_initial;
    float cangle_initial;
    
    float x_final;
    float y_final;
    float cangle_final;

    float r;
    float pangle;

    void Go();

    private:
    void CalculPolar();
};

#endif


