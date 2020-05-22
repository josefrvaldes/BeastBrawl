/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 //#pragma once

#include "membershipFunctions.h"

float MembershipFunctions::calculateMembership(float currentValue, float minValue, float maxValue){
    return (currentValue / (maxValue-minValue)) - (minValue / (maxValue-minValue));
}

float MembershipFunctions::calculateMembershipInverse(float currentValue, float minValue, float maxValue){
    return (-currentValue / (maxValue-minValue)) + (maxValue / (maxValue-minValue));
}

float MembershipFunctions::membershipGrade(float currentValue, float minValue, float maxValue){
    if(currentValue <= minValue)
        return  0.f;
    else if(currentValue > minValue && currentValue < maxValue)
        return  calculateMembership(currentValue, minValue, maxValue);
    else
        return  1.f;
}

float MembershipFunctions::membershipGradeInverse(float currentValue, float minValue, float maxValue){
    if(currentValue <= minValue)
        return  0.f;
    else if(currentValue > minValue && currentValue < maxValue)
        return  calculateMembershipInverse(currentValue, minValue, maxValue);
    else
        return  1.f;
}

float MembershipFunctions::membershipTriangle(float currentValue, float minValue, float peakValue, float maxValue){
    if(currentValue <= minValue)
        return  0.f;
    else if(currentValue > minValue && currentValue < peakValue)
        return  calculateMembership(currentValue, minValue, peakValue);
    else if(currentValue == peakValue)
        return  1.f;
    else if(currentValue > peakValue && currentValue < maxValue)
        return  calculateMembershipInverse(currentValue, peakValue, maxValue);
    else
        return  0.f;
}

float MembershipFunctions::membershipTrapezoid(float currentValue, float minValue, float minPeakValue, float maxPeakValue, float maxValue){
    if(currentValue <= minValue)
        return  0.f;
    else if(currentValue > minValue && currentValue < minPeakValue)
        return  calculateMembership(currentValue, minValue, minPeakValue);
    else if(currentValue >= minPeakValue && currentValue <= maxPeakValue)
        return  1.f;
    else if(currentValue > maxPeakValue && currentValue < maxValue)
        return  calculateMembershipInverse(currentValue, maxPeakValue, maxValue);
    else
        return  0.f;
}