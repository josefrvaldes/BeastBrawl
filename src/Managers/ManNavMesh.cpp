#include "./ManNavMesh.h"
#include <iostream>
#include "../Aliases.h"
#include "../Entities/NavMesh.h"

using namespace std;


ManNavMesh::ManNavMesh() {
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
}

void ManNavMesh::CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints){
    shared_ptr<NavMesh> p = make_shared<NavMesh>(pos,rot,width,height,depth,waypoints);
    entities.push_back(p);
}


