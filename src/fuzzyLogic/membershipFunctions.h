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


struct MembershipFunctions{

    //public:
        // Constructor y destructor
        MembershipFunctions() {}
        ~MembershipFunctions() {}
        // Funciones de membresia en forma de grado normal e inverso
        static float membershipGrade(float currentValue, float minValue, float maxValue);
        static float membershipGradeInverse(float currentValue, float minValue, float maxValue);
        // Funcion de membresia en forma de triangulo
        static float membershipTriangle(float currentValue, float minValue, float peakValue, float maxValue);
        // Funcion de membresia en forma de trapezoide
        static float membershipTrapezoid(float currentValue, float minValue, float minPeakValue, float maxPeakValue, float maxValue);  
        // Calcular en nivel de membresia entre 2 valores
        static float calculateMembership(float currentValue, float minValue, float maxValue);  
        static float calculateMembershipInverse(float currentValue, float minValue, float maxValue);  

    //private:
    // de momento sin variables extra

};