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
 
 
 #pragma once

#include "fuzzySet.h"

class FuzzySet_LeftShoulder : public FuzzySet{
private:
    //the values that define the shape of this FLV
    double m_dPeakPoint;
    double m_dLeftOffset;
    double m_dRightOffset;

public:
    FuzzySet_LeftShoulder(double peak, double LeftOffset, double RightOffset):FuzzySet( ((peak - LeftOffset) + peak) / 2),
                                                                                                                m_dPeakPoint(peak),
                                                                                                                m_dLeftOffset(LeftOffset),
                                                                                                                m_dRightOffset(RightOffset){}
    //this method calculates the degree of membership for a particular value
    double CalculateDOM(double val)const;
};
