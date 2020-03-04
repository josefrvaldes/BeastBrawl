#pragma once

#include "Component.h"

class CRemovableObject : public Component {
   public:
    CRemovableObject();
    
    bool destroy;
};