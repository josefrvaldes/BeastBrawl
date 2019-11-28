#include "fuzzyTerm.h"

#include <vector>

using namespace std;

class FzAND : public FuzzyTerm{
private:
	//vector con las AND
	vector<FuzzyTerm*> arrayFzAND;

    //it doesn't make sense to allow clients to copy rules

    FzAND& operator=(const FzAND&);


public:
    FzAND(const FzAND&);
	~FzAND();
	//crear un AND con 2
	//FzAND(FuzzyTerm& one, FuzzyTerm& two):arrayFzAND.push_back(one.Clone()),   
	//                                          arrayFzAND.push_back(two.Clone()){}
	//FzAND(FuzzyTerm& one, FuzzyTerm& two);
	// crear un AND de 3
	//FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three);
	// crear un AND de 4
	//FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three, FuzzyTerm& four);

    virtual FuzzyTerm* Clone()const{return new FzAND(*this);}
    //retrieves the degree of membership of the term
    virtual double GetDOM()const;
    //clears the degree of membership of the term
    virtual void ClearDOM();
    // method for updating the DOM of a consequent when a rule fires
    virtual void ORwithDOM(double val);

	
};