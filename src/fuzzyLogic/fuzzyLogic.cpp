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
 
 
 #include "fuzzyLogic.h"

#include <cassert>
#include <iostream>


FuzzyLogic::~FuzzyLogic(){

}

void FuzzyLogic::AddRule(shared_ptr<FuzzyTerm> antecedent, shared_ptr<FuzzyTerm> consequence){
    m_Rules.push_back(make_shared<FuzzyRule>(antecedent, consequence));
}

shared_ptr<FuzzyVariable> FuzzyLogic::CreateFLV(const string& VarName){
	m_Variables.insert(std::pair<string,shared_ptr<FuzzyVariable>>(VarName,make_shared<FuzzyVariable>()));
	//m_Variables[VarName] = fv;
	return	m_Variables[VarName];
}

void FuzzyLogic::SetConfidencesOfConsequentsToZero(){

	auto curRule = m_Rules.begin();
	for (curRule = m_Rules.begin(); curRule != m_Rules.end(); ++curRule){
		(*curRule)->SetConfidenceOfConsequentToZero();
	}
}

void FuzzyLogic::Fuzzify(const std::string& NameOfFLV, double val){
	m_Variables[NameOfFLV]->Fuzzify(val);
}
  

double FuzzyLogic::DeFuzzify(const std::string& NameOfFLV){
	//first make sure the named FLV exists in this module
	assert ( (m_Variables.find(NameOfFLV) != m_Variables.end()) &&
				"<FuzzyLogic::DeFuzzifyMaxAv>:key not found");

	//clear the DOMs of all the consequents
	SetConfidencesOfConsequentsToZero();
	
	//process the rules 
    std::vector<shared_ptr<FuzzyRule>>::iterator actualRule;
	for (actualRule = m_Rules.begin(); actualRule != m_Rules.end(); ++actualRule){
		(*actualRule)->Calculate();
	}

	// defuzzify con el metodo de MaxAv
	return m_Variables[NameOfFLV]->DeFuzzifyMaxAv();
}
