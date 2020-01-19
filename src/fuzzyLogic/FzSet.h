#pragma once

#include "fuzzyTerm.h"
#include "fuzzySet.h"

#include <memory>
using namespace std;
class FzSet : public FuzzyTerm{
    
    public:
    FzSet(shared_ptr<FuzzySet> fuzzySet) : m_set(fuzzySet){};
    ~FzSet()=default;
    
    //all terms must implement a virtual constructor
    shared_ptr<FuzzyTerm> Clone() const override{ return make_shared<FzSet>(*this); };
    //retrieves the degree of membership of the term
    double GetDOM() const override{ return m_set->GetDOM(); };
    //clears the degree of membership of the term
    void ClearDOM() override{ m_set->ClearDOM(); };
    // method for updating the DOM of a consequent when a rule fires
    void ORwithDOM(double val) override{ m_set->ORwithDOM(val); };

    private:
    shared_ptr<FuzzySet> m_set;
};
