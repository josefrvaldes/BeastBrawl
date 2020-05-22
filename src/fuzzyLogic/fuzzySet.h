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

class FuzzySet{
protected:
    //this will hold the degree of membership in this set of a given value
    double m_dDOM;
    //this is the maximum of the set's membership function. For instance, if
    //the set is triangular then this will be the peak point of the triangle.
    //If the set has a plateau then this value will be the midpoint of the
    //plateau. This value is set in the constructor to avoid run-time
    //calculation of midpoint values.
    double m_dRepresentativeValue;

public:
    FuzzySet(double RepVal):m_dDOM(0.0), m_dRepresentativeValue(RepVal){}
    //return the degree of membership in this set of the given value. NOTE:
    //this does not set m_dDOM to the DOM of the value passed as the parameter.
    //This is because the centroid defuzzification method also uses this method
    //to determine the DOMs of the values it uses as its sample points.
    virtual double CalculateDOM(double val)const = 0;
    //if this fuzzy set is part of a consequent FLV and it is fired by a rule,
    //then this method sets the DOM (in this context, the DOM represents a
    //confidence level) to the maximum of the parameter value or the set's
    //existing m_dDOM value
    void ORwithDOM(double val);
    //accessor methods
    double GetRepresentativeVal()const;
    void ClearDOM();
    double GetDOM()const;
    // no comprobar lo que enviamos....
    void SetDOM(double val);
    void SetReprsentativeValue(double newRepresentative);
};
