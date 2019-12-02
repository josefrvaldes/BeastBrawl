#include    "fuzzySet_Triangle.h"
//#include "membershipFunctions.h"

#include <iostream>


double calculateMembership(double currentValue, double minValue, double maxValue){
    return (currentValue / (maxValue-minValue)) - (minValue / (maxValue-minValue));
}

double calculateMembershipInverse(double currentValue, double minValue, double maxValue){
    return (-currentValue / (maxValue-minValue)) + (maxValue / (maxValue-minValue));
}

double FuzzySet_Triangle::CalculateDOM(double val)const {
//test for the case where the triangle's left or right offsets are zero
//(to prevent divide by zero errors below)
    // DUDA diferencia con Equals??????
    if ( ((m_dRightOffset == 0.0) && (m_dPeakPoint == val)) || ((m_dLeftOffset == 0.0) && (m_dPeakPoint == val)) ){
        std::cout<< "TRIANGULO : PRIMERA EEEESSS: 1" << std::endl; 
        return 1.0;
    }
    //find DOM if left of center
    if ( (val <= m_dPeakPoint) && (val >= (m_dPeakPoint - m_dLeftOffset)) ){
        double grad =  calculateMembership(val, (m_dPeakPoint - m_dLeftOffset), m_dPeakPoint);
        std::cout<< "TRIANGULO : SEGUNDA EEEESSS: " << grad << std::endl; 
        return grad;
    }
    //find DOM if right of center
    else if ( (val > m_dPeakPoint) && (val < (m_dPeakPoint + m_dRightOffset)) ){
        double grad = calculateMembershipInverse(val, m_dPeakPoint, (m_dPeakPoint + m_dRightOffset));
        std::cout<< "TRIANGULO : TERCERA EEEESSS: " << grad << std::endl; 
        return grad;
    }
    //out of range of this FLV, return zero
    else{
        std::cout<< "TRIANGULO : CUARTA EEEESSS: 0" << std::endl; 
        return 0.0;
    }
}
