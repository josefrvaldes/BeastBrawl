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
	cout << "Sale fuzzy variable\n";
	return	m_Variables[VarName];
}

void FuzzyLogic::SetConfidencesOfConsequentsToZero(){

	auto curRule = m_Rules.begin();
	for (curRule = m_Rules.begin(); curRule != m_Rules.end(); ++curRule){
		(*curRule)->SetConfidenceOfConsequentToZero();
	}
}

void FuzzyLogic::Fuzzify(const std::string& NameOfFLV, double val){
	std::cout << "Fuzzificamos a: " << NameOfFLV << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	m_Variables[NameOfFLV]->Fuzzify(val);

}
  

double FuzzyLogic::DeFuzzify(const std::string& NameOfFLV){
	//first make sure the named FLV exists in this module
	assert ( (m_Variables.find(NameOfFLV) != m_Variables.end()) &&
				"<FuzzyLogic::DeFuzzifyMaxAv>:key not found");

	//std::cout << "existe la varible" << std::endl;
	//clear the DOMs of all the consequents
	SetConfidencesOfConsequentsToZero();
	//std::cout << "pasamos por el SetConfidencesOfConsequentsToZero()" << std::endl;
	
	//process the rules 
    std::vector<shared_ptr<FuzzyRule>>::iterator actualRule;
	for (actualRule = m_Rules.begin(); actualRule != m_Rules.end(); ++actualRule){
	//std::cout << "entramos al for, NO al calculate" << std::endl;
		(*actualRule)->Calculate();
	//std::cout << "entramos al for, SI al calculate" << std::endl;
	}


	std::cout << "bucle con el calculo de las diferentes reglas" << std::endl;
	//now defuzzify the resultant conclusion using the specified method
	return m_Variables[NameOfFLV]->DeFuzzifyMaxAv();

}
