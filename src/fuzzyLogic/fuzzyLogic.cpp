#include "fuzzyLogic.h"

#include <cassert>



FuzzyLogic::~FuzzyLogic(){

}


void FuzzyLogic::AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence){
    m_Rules.push_back(new FuzzyRule(antecedent, consequence));
}


FuzzyVariable& FuzzyLogic::CreateFLV(const std::string& VarName){
	m_Variables[VarName] = new FuzzyVariable();
	return	*m_Variables[VarName];
}


void FuzzyLogic::SetConfidencesOfConsequentsToZero(){

	std::vector<FuzzyRule*>::iterator curRule = m_Rules.begin();
	for (curRule = m_Rules.begin(); curRule != m_Rules.end(); ++curRule){
		(*curRule)->SetConfidenceOfConsequentToZero();
	}
}


inline void FuzzyLogic::Fuzzify(const std::string& NameOfFLV, double val){

	m_Variables[NameOfFLV]->Fuzzify(val);

}
  



inline double FuzzyLogic::DeFuzzify(const std::string& NameOfFLV){
	//first make sure the named FLV exists in this module
	assert ( (m_Variables.find(NameOfFLV) != m_Variables.end()) &&
				"<FuzzyLogic::DeFuzzifyMaxAv>:key not found");
	//clear the DOMs of all the consequents
	SetConfidencesOfConsequentsToZero();
	//process the rules
	std::vector<FuzzyRule*>::iterator curRule = m_Rules.begin();
	for (curRule = m_Rules.begin(); curRule != m_Rules.end(); ++curRule){
		(*curRule)->Calculate();
	}
	//now defuzzify the resultant conclusion using the specified method
	return m_Variables[NameOfFLV]->DeFuzzifyMaxAv();

}
