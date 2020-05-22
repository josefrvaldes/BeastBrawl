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
    
    //given a fuzzy variable and a defuzzification method this returns a
    //crisp value
    double DeFuzzify(const string& key);

};
