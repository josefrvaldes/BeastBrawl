#include "fuzzyRules.h"

void FuzzyRule::Calculate(){
    m_pConsequence->ORwithDOM(m_pAntecedent->GetDOM());
}
