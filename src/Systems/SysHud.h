#pragma once

#include <memory>
#include <vector>

class Entity;
class ManHUDEvent;

class SysHud {
    public:
        SysHud() = default;
        ~SysHud() = default;

        void UpdateMinimap(std::vector<std::shared_ptr<Entity>>& entities);
        void UpdateEventHud(ManHUDEvent* manHudEvent);

};