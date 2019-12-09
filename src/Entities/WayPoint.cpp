#include "WayPoint.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"
#include "../Components/CWayPointEdges.h"


WayPoint::WayPoint()
{
    
}

//TODO: Este constructor hay que quitarlo 
WayPoint::WayPoint(glm::vec3 _position) {
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>(_position,0,0);
    shared_ptr<CWayPointEdges> cWayPointEdges = make_shared<CWayPointEdges>();
    AddComponent(cWayPoint);
    AddComponent(cWayPointEdges);

    position = _position;
    type = 0;
    id = 0;
}

WayPoint::WayPoint(glm::vec3 _position, int _type, int _id){
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>(_position,_type,_id);
    AddComponent(cWayPoint);
}

void WayPoint::AddEdge(int to, float cost){
    auto components = GetComponents();

	auto mapEdges = components.find(CompType::WayPointEdgesComp);
	auto cEdges = static_cast<CWayPointEdges*>(mapEdges->second.get());

    cEdges->AddEdge(to, cost);
}

WayPoint::~WayPoint(){

}
