#include "fuzzyVariable.h"


void FuzzyVariable::AdjustRangeToFit(double min, double max){
	m_dMinRange = min;
	m_dMaxRange = max;
}

FzSet FuzzyVariable::AddLeftShoulderSet(std::string name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = new FuzzySet_LeftShoulder(peak, min, max);
    return *(new FzSet( *(m_MemberSets[name]) ));
}
FzSet FuzzyVariable::AddRightShoulderSet(std::string name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = new FuzzySet_RightShoulder(peak, min, max);
    return *(new FzSet( *(m_MemberSets[name]) ));
}
FzSet FuzzyVariable::AddTriangularSet(std::string name, double minBound, double peak, double maxBound){
    AdjustRangeToFit(minBound, maxBound);
    double min = peak - minBound;
    double max = maxBound - peak;
    m_MemberSets[name] = new FuzzySet_Triangle(peak, min, max);
    return *(new FzSet( *(m_MemberSets[name]) ));
}
//void FuzzyVariable::AddSingletonSet(std::string name, double minBound, double peak, double maxBound){

//}

    //fuzzify a value by calculating its DOM in each of this variable's subsets
// CALCULAMOS EL VALOR DIFUSO PARA CADA MEMBRESIA
void FuzzyVariable::Fuzzify(double val){

    std::map<std::string, FuzzySet*>::iterator actualSet = m_MemberSets.begin();
    // al ser un mapa necesitamos el .second para acceder a la segunda variable
    for (actualSet=m_MemberSets.begin(); actualSet != m_MemberSets.end(); ++actualSet){
        // calculamos el DOM actual
        double actualDOM = (*actualSet).second->CalculateDOM(val);
        // y lo pasamos 
        (*actualSet).second->SetDOM(actualDOM);
    }
}


double FuzzyVariable::DeFuzzifyMaxAv()const{
    double crispValue = 0.0;
    return crispValue;

}