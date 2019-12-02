#pragma once

#include "fuzzySet.h"

class FuzzySet_Triangle : public FuzzySet{
private:
    //the values that define the shape of this FLV
    double m_dPeakPoint;
    double m_dLeftOffset;
    double m_dRightOffset;
public:
    FuzzySet_Triangle(double mid, double lft, double rgt):FuzzySet(mid),
                                                            m_dPeakPoint(mid),
                                                            m_dLeftOffset(lft),
                                                            m_dRightOffset(rgt){}

    //this method calculates the degree of membership for a particular value
    double CalculateDOM(double val)const;
};
