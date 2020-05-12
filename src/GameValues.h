#pragma once

#include <vector>
#include <map>


enum DifficultyAI { EASY, NORMAL, DIFFICULT };

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
        void SetSeconds(std::map<uint16_t, uint16_t> s) { seconds = s; };

        //void SetMainCPoints(int p)                      { mainCPoints = p; };
        //void SetIAPoints(std::vector<int> p)            { iaPoints = p; };
        void SetNumBattles(uint8_t num)                 { numBattles = num; };
        void IncrementBattle()                          { actualBattle++; };
        void ResetCountBattles()                        { actualBattle=1; };
        void ResetPoints()                              { for(size_t i = 0; i<totalPoints.size(); i++) totalPoints[i]=0; };
        void SetRankingPoints(std::multimap<uint16_t, uint16_t, std::greater<uint16_t>> p){ rankingPoints = p; };
        void SetTotalPoints( std::vector<uint16_t> tp)  { totalPoints = tp; };
        void SetNumPlayers(uint8_t p)                   { numPlayers = p; };
        void SetDifficultAI(DifficultyAI d)   { difficultAI = d; };

        // GETTERS
        const int GetCharacter()                                                            { return mainCharacter; };
        const std::vector<int>& GetIACharacters()                                           { return iaCharacter; };
        const int GetMap()                                                                  { return mapGame; };
        const bool GetWin()                                                                 { return win; };
        const int GetTimeTotem()                                                            { return timeTotem; };
        const int GetGameTime()                                                             { return gameTime; };
        const DifficultyAI GetDifficultAI()                                      { return difficultAI; };

        const std::map<uint16_t, uint16_t>& GetRanking()                                    { return ranking; };
        const std::map<uint16_t, uint16_t>& GetSeconds()                                    { return seconds; };
        const std::multimap<uint16_t, uint16_t, std::greater<uint16_t>>& GetRankingPoints() { return rankingPoints; };
        const uint8_t GetNumPlayers()                                                       { return numPlayers; };

        //const int GetMainCPoints()                      { return mainCPoints; };
        //const std::vector<int> GetIAPoints()            { return iaPoints; };
        uint8_t GetNumBattles()                         { return numBattles; };
        uint8_t GetActualBattle()                       { return actualBattle; };
        std::vector<uint16_t>& GetTotalPoints()         { return totalPoints; };
        const uint16_t* GetPosPoints()                  { return posPoints; };

    private:
        GameValues() = default;;
        static GameValues* instance;

        int mainCharacter { 0 };
        std::vector<int> iaCharacter = { 1, 2, 3, 4, 5};  // Por defecto  
        int mapGame { 0 };
        int timeTotem { 45 };
        int gameTime { 180 };
        uint8_t numPlayers{ 4 };
        DifficultyAI difficultAI { DifficultyAI::NORMAL };
        
        // Ranking: posicion | personaje
        bool win { false };
        std::map<uint16_t, uint16_t> ranking { {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} };
        std::map<uint16_t, uint16_t> seconds { {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} };


        // Para el modo torneo
        uint8_t numBattles {4};
        uint8_t actualBattle {1};

        //uint16_t actualPoints[6] {0, 0, 0, 0, 0, 0};
        std::vector<uint16_t> totalPoints {0, 0, 0, 0, 0, 0};
        std::multimap<uint16_t, uint16_t, std::greater<uint16_t>> rankingPoints;
        //std::map<uint16_t, uint16_t> ranking { {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} };
        //std::map<uint16_t, uint16_t> seconds { {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} };
        const uint16_t posPoints[6] {12, 8, 5, 3, 1, 0}; // tener estos puntos independiente del numJugadores


};

