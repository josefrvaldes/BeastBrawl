#include    "fuzzySet_RightShoulder.h"

double FuzzySet_RightShoulder::CalculateDOM(double val)const{
    //check for case where the offset may be zero
    if ( (m_dLeftOffset == 0) && (m_dPeakPoint == val) ){
        return 1.0;
    }
    //find DOM if left of center
    if ( (val <= m_dPeakPoint) && (val > (m_dPeakPoint - m_dLeftOffset)) ){
        double grad = 1.0 / m_dLeftOffset;
        grad = grad * (val - (m_dPeakPoint - m_dLeftOffset));
        return grad;
    }
    //find DOM if right of center
    else if (val > m_dPeakPoint){
        return 1.0;
    }
    //out of range of this FLV, return zero
    else{
        return 0.0;
    }
}
