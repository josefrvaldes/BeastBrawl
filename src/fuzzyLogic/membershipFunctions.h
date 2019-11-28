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