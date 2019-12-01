#include "FzAND.h"

#include <vector>
#include <iostream>

FzAND::	~FzAND(){
	
}

//FzAND::FzAND(const FzAND& a){
//}


//	vector<FuzzyTerm*> arrayFzAND;
FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two){
	arrayFzAND.push_back(one.Clone());
	arrayFzAND.push_back(two.Clone());	
}
// crear un AND de 3
FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three){
	arrayFzAND.push_back(one.Clone());
	arrayFzAND.push_back(two.Clone());
	arrayFzAND.push_back(three.Clone());
}
// crear un AND de 4
FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three, FuzzyTerm& four){
	arrayFzAND.push_back(one.Clone());
	arrayFzAND.push_back(two.Clone());
	arrayFzAND.push_back(three.Clone());
	arrayFzAND.push_back(four.Clone());
}

/*
EJEMPLO CLONE()
    virtual Implementation* clone() const override
    {
        return new Implementation(*this);
    }
*/
// DUDA como hago el clon?
//FuzzyTerm* FzAND::Clone()const{
//	return new FzAND(*this);
//}
//retrieves the degree of membership of the term
// puerta AND = minimo
double FzAND::GetDOM()const{
	//std::cout << "hola4" << std::endl;
	// DUDA en el ejemplo se iguala a 0, supomgo que esta mal
	double minDOM = 1000000;
	//std::cout << "hola5" << std::endl;
	for (auto curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		if( (*curTerm)->GetDOM() < minDOM  )
			minDOM = (*curTerm)->GetDOM();
	}
	return minDOM;
}
//clears the degree of membership of the term
void FzAND::ClearDOM(){
	for (auto curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		(*curTerm)->ClearDOM();
	}	
}
// method for updating the DOM of a consequent when a rule fires
void FzAND::ORwithDOM(double val){
	//std::cout << "hola3" << std::endl;
	for (auto curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		(*curTerm)->ORwithDOM(val);
	}
}

