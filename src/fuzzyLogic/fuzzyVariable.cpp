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
 
 
 #include "fuzzyVariable.h"
#include <iostream>


void FuzzyVariable::AdjustRangeToFit(double min, double max){
	m_dMinRange = min;
	m_dMaxRange = max;
}

shared_ptr<FzSet> FuzzyVariable::AddLeftShoulderSet(const std::string& name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = make_shared<FuzzySet_LeftShoulder>(peak, min, max);
    return (make_shared<FzSet>( (m_MemberSets[name]) ));
}
shared_ptr<FzSet> FuzzyVariable::AddRightShoulderSet(const std::string& name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = make_shared<FuzzySet_RightShoulder>(peak, min, max);
    return (make_shared<FzSet>( (m_MemberSets[name]) ));
}
shared_ptr<FzSet> FuzzyVariable::AddTriangularSet(const std::string& name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = make_shared<FuzzySet_Triangle>(peak, min, max);
    return (make_shared<FzSet>( (m_MemberSets[name]) ));
}
//void FuzzyVariable::AddSingletonSet(std::string name, double minBound, double peak, double maxBound){

//}

//fuzzify a value by calculating its DOM in each of this variable's subsets
// CALCULAMOS EL VALOR DIFUSO PARA CADA MEMBRESIA
void FuzzyVariable::Fuzzify(double val){

    auto actualSet = m_MemberSets.begin();
    // al ser un mapa necesitamos el .second para acceder a la segunda variable
    for (actualSet=m_MemberSets.begin(); actualSet != m_MemberSets.end(); ++actualSet){
        // calculamos el DOM actual
        double actualDOM = (*actualSet).second->CalculateDOM(val);
        // y lo pasamos 
        (*actualSet).second->SetDOM(actualDOM);
    }
}


double FuzzyVariable::DeFuzzifyMaxAv(){
    double crispValue = 0.0;
    double numerador = 0.0;
    double denominador = 0.0;
    
    auto actualSet = m_MemberSets.begin();

    // al ser un mapa necesitamos el .second para acceder a la segunda variable
    for (actualSet=m_MemberSets.begin(); actualSet != m_MemberSets.end(); ++actualSet){
        numerador += (*actualSet).second->GetRepresentativeVal() * (*actualSet).second->GetDOM();
        denominador += (*actualSet).second->GetDOM();

        //std::cout << "Valor Representativo de " << (*actualSet).first  << ": " << (*actualSet).second->GetRepresentativeVal() << std::endl;
        //std::cout << "Valor DOM de " << (*actualSet).first << ": " << (*actualSet).second->GetDOM() << std::endl;  // T0-Do :  ALGO DEL GetDOM ESTA MAL !!!!
    }

    if(denominador!=0)
        crispValue = numerador / denominador;
    
    // calcular maxAv
    return crispValue;

}