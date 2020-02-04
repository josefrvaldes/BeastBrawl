#include "fuzzyRules.h"

void FuzzyRule::Calculate(){
    //std::cout << "hola" << std::endl;
    
    m_pConsequence->ORwithDOM(m_pAntecedent->GetDOM());
    //std::cout << "hola2" << std::endl;
}
