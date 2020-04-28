#include "SystemRanking.h"

#include <iostream>
#include <map>
#include <algorithm>

#include <Managers/ManCar.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>
#include <Components/CId.h>
#include <Components/CTotem.h>
#include <Components/CCar.h>

#include "GameValues.h"

using namespace std;

void SystemRanking::Update(ManCar* manCars){

    //struct auxiliar para guardarme tiempo y numero de coche
    struct ranking_t{
        uint16_t carNumber;
        float    time;
        inline bool operator() (const ranking_t& struct1, const ranking_t& struct2)
        {
            return (struct1.time > struct2.time);
        }
    };

    std::vector<ranking_t> ranking;
    CTotem* cTotem;
    CId* cId;
    CCar* cCar;

    if(manCars && !manCars->GetEntities().empty()){
        ranking_t rank{};
        int i = 0;
        for(auto& carAux : manCars->GetEntities()){
            cTotem = static_cast<CTotem*>(carAux->GetComponent(CompType::TotemComp).get());
            rank.carNumber = i++;
            rank.time = cTotem->accumulatedTime;
            ranking.push_back(rank);
        }

        std::sort (ranking.begin(), ranking.end(), ranking_t());


        //Ya tenemos ordenados las posiciones, ahora vamos a actualizar sus valores en el CTotem
        int j = 1;
        for(auto aux : ranking){
            uint16_t numCar = aux.carNumber;
            cTotem = static_cast<CTotem*>(manCars->GetEntities()[numCar]->GetComponent(CompType::TotemComp).get());
            if(cTotem) {
                cTotem->positionRanking = j++;
            }
        }

        // Actualizar ranking de GameValues
        std::map<uint16_t, uint16_t> order;
        auto mCar = static_cast<CId*>(manCars->GetCar()->GetComponent(CompType::IdComp).get());

        for (auto& car : manCars->GetEntities() ) {
            cCar = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
            cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
            if (cCar) {
                uint16_t position = cTotem->positionRanking;
                uint16_t pj = (uint16_t)cCar->character;
                order.insert(std::pair<uint16_t,uint16_t>(position,pj));

                if (cId && mCar && mCar->id == cId->id) {
                    if (cTotem->positionRanking == 1) {
                        GameValues::GetInstance()->SetWin(true);
                    } else {
                        GameValues::GetInstance()->SetWin(false);
                    }
                }
            }
        }

        GameValues::GetInstance()->SetRanking(order);
        
        //cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++ ORDEN\n";
        //for (auto it = order.begin(); it != order.end(); ++it)
        //    std::cout << it->first << " => " << it->second << '\n';
        //cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++ ORDEN\n";

    }
}