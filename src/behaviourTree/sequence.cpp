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
 * 
 */
 
 
 #include "sequence.h"
#include "Blackboard.h"

bool sequence::run(Blackboard* blackboard){
	for (const shared_ptr<behaviourTree>& child : getChildren()) {  // The generic Sequence implementation.
		if (!child->run(blackboard))  						 // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
			return false;
	}
	return true;  											 // All children suceeded, so the entire run() operation succeeds.
}