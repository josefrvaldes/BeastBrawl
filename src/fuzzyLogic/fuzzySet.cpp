#include "fuzzySet.h"

void FuzzySet::ORwithDOM(double val){
	if( m_dDOM < val)
		m_dDOM = val;
}
//accessor methods
double FuzzySet::GetRepresentativeVal()const{
	return m_dRepresentativeValue;
}
double FuzzySet::GetDOM()const{
	return m_dDOM;
}
void FuzzySet::SetDOM(double val){
	if(val >= 0 && val<= 1)
		m_dDOM = val;
}
void FuzzySet::SetReprsentativeValue(double newRepresentative){
	m_dRepresentativeValue = newRepresentative;
}
void FuzzySet::ClearDOM(){
	m_dDOM = 0.0;
}
