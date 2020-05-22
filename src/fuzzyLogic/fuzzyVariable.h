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

#include "fuzzySet_Triangle.h"
#include "fuzzySet_RightShoulder.h"
#include "fuzzySet_LeftShoulder.h"
#include "FzSet.h"

#include <memory>
#include <map>
#include <iostream>

using namespace std;
class FuzzyVariable{
private:
    typedef map<string, shared_ptr<FuzzySet>> MemberSets;

    //disallow copies
    FuzzyVariable(const shared_ptr<FuzzyVariable>);
    shared_ptr<FuzzyVariable> operator=(const shared_ptr<FuzzyVariable>);

    //a map of the fuzzy sets that comprise this variable
    MemberSets m_MemberSets;
    //the minimum and maximum value of the range of this variable
    double  m_dMinRange;
    double  m_dMaxRange;
    //this method is called with the upper and lower bound of a set each time a
    //new set is added to adjust the upper and lower range values accordingly
    void AdjustRangeToFit(double min, double max);
    //a client retrieves a reference to a fuzzy variable when an instance is
    //created via FuzzyModule::CreateFLV(). To prevent the client from deleting
    //the instance the FuzzyVariable destructor is made private and the
    //FuzzyModule class made a friend.
    // implica que FuzzyLogic tiene acceso a la parte privada de FuzzyVariable
    friend class FuzzyLogic;

public:
    FuzzyVariable(){ 
        m_dMinRange = 0.0;
        m_dMaxRange = 0.0;
    };
    ~FuzzyVariable(){};
    //the following methods create instances of the sets named in the method
    //name and adds them to the member set map. Each time a set of any type is
    //added the m_dMinRange and m_dMaxRange are adjusted accordingly. All of the
    //methods return a proxy class representing the newly created instance. This
    //proxy set can be used as an operand when creating the rule base.
    shared_ptr<FzSet> AddLeftShoulderSet(const string& name, double minBound, double peak, double maxBound);
    shared_ptr<FzSet> AddRightShoulderSet(const string& name, double minBound, double peak, double maxBound);
    shared_ptr<FzSet> AddTriangularSet(const string& name, double minBound, double peak, double maxBound);
    //void AddSingletonSet(std::string name, double minBound, double peak, double maxBound);

    //fuzzify a value by calculating its DOM in each of this variable's subsets
    void Fuzzify(double val);
    //defuzzify the variable using the MaxAv method
    double DeFuzzifyMaxAv();

};
