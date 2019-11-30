#include "fuzzySet.h"

void FuzzySet::ORwithDOM(double val){
	if( m_dDOM < val)
		m_dDOM = val;
}
//accessor methods
double FuzzySet::GetRepresentativeVal()const{
	return m_dRepresentativeValue;
}
void FuzzySet::SetDOM(double val){
	if(val >= 0 && val<= 1)
		m_dDOM = val;
}
