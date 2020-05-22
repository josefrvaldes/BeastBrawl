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
 
 
 #include "FzAND.h"

#include <vector>


FzAND::	~FzAND(){
	
}

//FzAND::FzAND(const FzAND& a){
//}


//	vector<FuzzyTerm*> arrayFzAND;
FzAND::FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two){
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(one->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(two->Clone()));	
}
// crear un AND de 3
FzAND::FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two, shared_ptr<FuzzyTerm> three){
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(one->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(two->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(three->Clone()));
}
// crear un AND de 4
FzAND::FzAND(shared_ptr<FuzzyTerm> one, shared_ptr<FuzzyTerm> two, shared_ptr<FuzzyTerm> three, shared_ptr<FuzzyTerm> four){
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(one->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(two->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(three->Clone()));
	arrayFzAND.push_back(static_cast<shared_ptr<FuzzyTerm>>(four->Clone()));
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
	for (auto curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		(*curTerm)->ORwithDOM(val);
	}
}

