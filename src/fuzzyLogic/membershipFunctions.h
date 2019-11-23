#pragma once


struct MembershipFunctions{

    //public:
        // Constructor y destructor
        MembershipFunctions() {}
        ~MembershipFunctions() {}
        // Funciones de membresia en forma de grado normal e inverso
        float membershipGrade(float currentValue, float minValue, float maxValue)const;
        float membershipGradeInverse(float currentValue, float minValue, float maxValue)const;
        // Funcion de membresia en forma de triangulo
        float membershipTriangle(float currentValue, float minValue, float peakValue, float maxValue)const;
        // Funcion de membresia en forma de trapezoide
        float membershipTrapezoid(float currentValue, float minValue, float minPeakValue, float maxPeakValue, float maxValue)const;  
        // Calcular en nivel de membresia entre 2 valores
        float calculateMembership(float currentValue, float minValue, float maxValue)const;  
        float calculateMembershipInverse(float currentValue, float minValue, float maxValue)const;  

    //private:
    // de momento sin variables extra

};