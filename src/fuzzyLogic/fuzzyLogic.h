#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "fuzzyVariable.h"
#include "fuzzyRules.h"
#include "FzAND.h"

using namespace std;

class FuzzyLogic{

private:

    //a map of all the fuzzy variables this module uses
    map<string, shared_ptr<FuzzyVariable>> m_Variables;
    //a vector containing all the fuzzy rules
    vector<shared_ptr<FuzzyRule>> m_Rules;
    //zeros the DOMs of the consequents of each rule. Used by Defuzzify()
    void SetConfidencesOfConsequentsToZero();
public:
    ~FuzzyLogic();
    //creates a new "empty" fuzzy variable and returns a reference to it.
    shared_ptr<FuzzyVariable> CreateFLV(const string& VarName);
    //adds a rule to the module
    void AddRule(shared_ptr<FuzzyTerm> antecedent, shared_ptr<FuzzyTerm> consequence);
    //this method calls the Fuzzify method of the named FLV
    void Fuzzify(const string& NameOfFLV, double val);
        //cout<< "hola";
    
    //given a fuzzy variable and a defuzzification method this returns a
    //crisp value
    double DeFuzzify(const string& key);

};
