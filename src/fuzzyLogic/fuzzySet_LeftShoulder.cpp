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
 
 
 #include    "fuzzySet_LeftShoulder.h"

double FuzzySet_LeftShoulder::CalculateDOM(double val)const{
    //check for case where the offset may be zero
    if ( (m_dRightOffset == 0) && (m_dPeakPoint == val) ){
        return 1.0;
    }
    //find DOM if left of center
    if ( (val > m_dPeakPoint) && (val < (m_dPeakPoint + m_dRightOffset)) ){
        double grad = 1.0 / -m_dRightOffset;
        grad = grad * (val - m_dPeakPoint) + 1.0;
        return grad;
    }
    //find DOM if right of center
    else if (val < m_dPeakPoint){
        return 1.0;
    }
    //out of range of this FLV, return zero
    else{
        return 0.0;
    }
}
