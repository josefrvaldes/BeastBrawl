#pragma once

#include "fuzzyTerm.h"
#include <memory>

using namespace std;

class FuzzyRule
{
private:
    //antecedent (usually a composite of several fuzzy sets and operators)
    const shared_ptr<FuzzyTerm> m_pAntecedent;
    //consequence (usually a single fuzzy set, but can be several ANDed together)
    shared_ptr<FuzzyTerm> m_pConsequence;
    //it doesn't make sense to allow clients to copy rules
    FuzzyRule(const shared_ptr<FuzzyRule>);
    shared_ptr<FuzzyRule> operator=(const shared_ptr<FuzzyRule>);
public:
    FuzzyRule(shared_ptr<FuzzyTerm> ant, shared_ptr<FuzzyTerm> con):m_pAntecedent(ant->Clone()),
                                                    m_pConsequence(con->Clone()){}

    ~FuzzyRule(){}
    void SetConfidenceOfConsequentToZero(){m_pConsequence->ClearDOM();}
    //this method updates the DOM (the confidence) of the consequent term with
    //the DOM of the antecedent term.
    void Calculate();
};
