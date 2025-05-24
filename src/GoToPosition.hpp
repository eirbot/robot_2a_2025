#ifndef GOTOPOSITION_H
#define GOTOPOSITION_H

#include "Arduino.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "common.h"
#include "ClassMotors.hpp"
#include "tofs.h"

struct Zone {
    float x1, y1, x2, y2;
};

class GoToPosition{

    public:
    /**
     * @brief Construct a new Go To Position object
     * 
     * @param x_i para
     * @param y_i 
     * @param cangle_i 
     * @param x_f 
     * @param y_f 
     * @param cangle_f 
     */
    GoToPosition(const float &x_i,const float &y_i,const float &cangle_i, const float &x_f,const float &y_f,const float &cangle_f);

    float x_initial;
    float y_initial;
    float cangle_initial;
    
    float x_final;
    float y_final;
    float cangle_final;

    float r;
    float pangle;
    float pangleFin;

    
    void StopMatch();
    void Go(float x_f,float y_f,float cangle_f);
    void AllerEtSet(float x_f, float y_f, float cangle_f, float x_set, float y_set, float cangle_set);
    void UpdateFinalPoseAfterStop(float distanceDid);
    bool Evitement();
    bool IsInForbiddenZone(float x, float y);

    private:
    void CalculPolar();
};

#endif


