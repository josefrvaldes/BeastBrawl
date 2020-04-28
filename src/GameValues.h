#pragma once

#include <vector>
#include <map>

class GameValues {
    public:
        virtual ~GameValues(){};
        static GameValues* GetInstance();

        // SETTERS
        void SetCharacter(int c);
        void SetMap(int m)                              { mapGame = m; };
        void SetWin(bool w)                             { win = w; };
        void SetTimeTotem(int tt)                       { timeTotem = tt; };
        void SetGameTime(int gt)                        { gameTime = gt; };

        void SetRanking(std::map<uint16_t, uint16_t> r) { ranking = r; };

        void SetMainCPoints(int p)                      { mainCPoints = p; };
        void SetIAPoints(std::vector<int> p)            { iaPoints = p; };

        // GETTERS
        const int GetCharacter()                        { return mainCharacter; };
        const std::vector<int>& GetIACharacters()       { return iaCharacter; };
        const int GetMap()                              { return mapGame; };
        const bool GetWin()                             { return win; };
        const int GetTimeTotem()                        { return timeTotem; };
        const int GetGameTime()                         { return gameTime; };

        const std::map<uint16_t, uint16_t>& GetRanking(){ return ranking; };

        const int GetMainCPoints()                      { return mainCPoints; };
        const std::vector<int> GetIAPoints()            { return iaPoints; };

    private:
        GameValues() = default;;
        static GameValues* instance;

        int mainCharacter { 0 };
        std::vector<int> iaCharacter = { 1, 2, 3, 4, 5};  // Por defecto  
        int mapGame { 0 };
        int timeTotem { 45 };
        int gameTime { 180 };

        // Ranking: posicion | personaje
        bool win { false };
        std::map<uint16_t, uint16_t> ranking { {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} };

        // Para el modo torneo. Modificar como tenga que ser.
        int mainCPoints { 0 };
        std::vector<int> iaPoints = { 0, 0, 0, 0, 0};  

};
