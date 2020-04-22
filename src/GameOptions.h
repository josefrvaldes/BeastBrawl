#pragma once

#include <vector>

class GameOptions {
    public:
        virtual ~GameOptions(){};
        static GameOptions* GetInstance();

        void SetCharacter(int c);
        void SetMainCPoints(int p)                      { mainCPoints = p; };
        void SetIAPoints(std::vector<int> p)            { iaPoints = p; };
        void SetMap(int m)                              { mapGame = m; };

        const int GetCharacter()                        { return mainCharacter; };
        const int GetMap()                              { return mapGame; };
        const std::vector<int>& GetIACharacters()       { return iaCharacter; };

    private:
        GameOptions() = default;;
        static GameOptions* instance;

        int mainCharacter { 0 };
        std::vector<int> iaCharacter = { 1, 2, 3, 4, 5};  // Por defecto  
        int mapGame { 0 };

        // Para el modo torneo. Modificar como tenga que ser.
        int mainCPoints { 0 };
        std::vector<int> iaPoints = { 0, 0, 0, 0, 0};  

};

