#pragma once

#include <vector>
#include <memory>

class ManCar;

class SystemRanking {
   public:
        SystemRanking() = default;
        ~SystemRanking() = default;

        void Update(ManCar*);

   private:
};