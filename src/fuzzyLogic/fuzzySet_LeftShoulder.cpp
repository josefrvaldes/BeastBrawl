#include    "fuzzySet_LeftShoulder.h"

#include <iostream>
double FuzzySet_LeftShoulder::CalculateDOM(double val)const{
    //check for case where the offset may be zero
    if ( (m_dRightOffset == 0) && (m_dPeakPoint == val) ){
        std::cout<< "IZQUIERDA : PRIMERA EEEESSS: 1" << std::endl; 
        return 1.0;
    }
    //find DOM if left of center
    if ( (val > m_dPeakPoint) && (val < (m_dPeakPoint + m_dRightOffset)) ){
        double grad = 1.0 / -m_dRightOffset;
        grad = grad * (val - m_dPeakPoint) + 1.0;
        std::cout<< "IZQUIERDA : SEGUNDO EEEESSS: " << grad << std::endl; 
        return grad;
    }
    //find DOM if right of center
    else if (val < m_dPeakPoint){
        std::cout<< "IZQUIERDA : TERCERA EEEESSS: 1" << std::endl; 
        return 1.0;
    }
    //out of range of this FLV, return zero
    else{
        std::cout<< "IZQUIERDA : CUARTO EEEESSS: 0" << std::endl; 
        return 0.0;
    }
}
