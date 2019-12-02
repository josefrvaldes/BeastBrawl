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