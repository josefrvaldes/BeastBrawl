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
