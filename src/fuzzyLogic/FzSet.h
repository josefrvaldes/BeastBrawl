#pragma once

#include "fuzzyTerm.h"
#include "fuzzySet.h"

class FzSet : public FuzzyTerm{
    
    public:
    FzSet(FuzzySet& fuzzySet) : m_set(&fuzzySet){};
    ~FzSet()=default;
    
    //all terms must implement a virtual constructor
    FuzzyTerm* Clone() const override{ return new FzSet(*this); };
    //retrieves the degree of membership of the term
    double GetDOM() const override{ return m_set->GetDOM(); };
    //clears the degree of membership of the term
    void ClearDOM() override{ m_set->ClearDOM(); };
    // method for updating the DOM of a consequent when a rule fires
    void ORwithDOM(double val) override{ m_set->ORwithDOM(val); };

    private:
    FuzzySet* m_set;
};
