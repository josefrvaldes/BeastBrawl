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

#include <vector>
#include <memory>
using namespace std;

class FzAND : public FuzzyTerm{
private:
	//vector con las AND
	vector<shared_ptr<FuzzyTerm>> arrayFzAND;

    //it doesn't make sense to allow clients to copy rules
    shared_ptr<FzAND> operator=(const shared_ptr<FzAND>);


public:
   // FzAND(const FzAND&);
	~FzAND();
	//crear un AND con 2
	//FzAND(FuzzyTerm& one, FuzzyTerm& two):arrayFzAND.push_back(one.Clone()),   
	//                                         arrayFzAND.push_back(two.Clone()){}
	FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two);
	// crear un AND de 3
	FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two, shared_ptr<FuzzyTerm> three);
	// crear un AND de 4
	FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two, shared_ptr<FuzzyTerm> three, shared_ptr<FuzzyTerm> four);

    shared_ptr<FuzzyTerm> Clone()const override{return make_shared<FzAND>(*this);};
    //retrieves the degree of membership of the term
    double GetDOM()const override;
    //clears the degree of membership of the term
    void ClearDOM() override;
    // method for updating the DOM of a consequent when a rule fires
    void ORwithDOM(double val) override;
	
	
};