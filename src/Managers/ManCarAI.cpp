#include "../Entities/CarAI.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"
#include "./ManCarAI.h"



void EjecutarMeHanCogido(Data d) 
{
    cout << "Han cogido un CarAI, madafaka!!" << endl;
}



ManCarAI::ManCarAI()
{
    SubscribeToEvents();
    //CreateCarAI(30, 20, 30);
    cout << "Hemos creado un CarAI, ahora tenemos " << CarAIs.size() << " CarAIs" << endl;
}

ManCarAI::~ManCarAI()
{
    CarAIs.clear();
    CarAIs.shrink_to_fit();
}


void ManCarAI::CreateCarAI(glm::vec3 _position) 
{
	shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    CarAIs.push_back(p);
}


void ManCarAI::CreateCarAI() 
{
	shared_ptr<CarAI> p = make_shared<CarAI>();
    CarAIs.push_back(p);
}


void ManCarAI::SubscribeToEvents() 
{
    
}




//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);



