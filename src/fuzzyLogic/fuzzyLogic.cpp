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
