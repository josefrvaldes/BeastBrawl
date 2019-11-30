#pragma once

#include <vector>
#include <map>
#include <string>
#include "fuzzyVariable.h"
#include "fuzzyRules.h"
#include "FzAND.h"


using namespace std;

class FuzzyLogic{
private:
    typedef map<string, FuzzyVariable*> VarMap;

    //a map of all the fuzzy variables this module uses
    VarMap  m_Variables;
    //a vector containing all the fuzzy rules
    vector<FuzzyRule*> m_Rules;
    //zeros the DOMs of the consequents of each rule. Used by Defuzzify()
    void SetConfidencesOfConsequentsToZero();

public:
    ~FuzzyLogic();
    //creates a new "empty" fuzzy variable and returns a reference to it.
    FuzzyVariable& CreateFLV(const std::string& VarName);
    //adds a rule to the module
    void AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence);
    //this method calls the Fuzzify method of the named FLV
    void Fuzzify(const std::string& NameOfFLV, double val);
        //cout<< "hola";
    
    //given a fuzzy variable and a defuzzification method this returns a
    //crisp value
    double DeFuzzify(const string& key);

};
