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
