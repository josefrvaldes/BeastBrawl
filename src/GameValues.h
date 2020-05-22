/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include <vector>
#include <map>


enum MAP {
    MAYAN
};



enum DifficultyAI { EASY, NORMAL, DIFFICULT };

class GameValues {
    public:
        virtual ~GameValues(){};
        static GameValues* GetInstance();

        // SETTERS
        void SetCharacter(int c);
        void SetMap(MAP m)                              { mapGame = m; };
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
        void SetCharacterSel(std::vector<uint8_t> cs)   { charactersSelected = cs; };
        void SetDifficultAI(DifficultyAI d)   { difficultAI = d; };

        // GETTERS
        const int GetCharacter()                        { return mainCharacter; };
        const std::vector<int>& GetIACharacters()       { return iaCharacter; };
        const MAP GetMap()                              { return mapGame; };
        const bool GetWin()                             { return win; };
        const int GetTimeTotem()                        { return timeTotem; };
        const int GetGameTime()                         { return gameTime; };
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
        const std::vector<uint8_t>& GetCharacterSel()   { return charactersSelected; };

    private:
        GameValues() = default;;
        static GameValues* instance;

        int mainCharacter { 0 };
        std::vector<int> iaCharacter = { 1, 2, 3, 4, 5};  // Por defecto  
        MAP mapGame { MAP::MAYAN };
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
        std::vector<uint16_t> totalPoints {0, 0, 0, 0, 0, 0};
        std::multimap<uint16_t, uint16_t, std::greater<uint16_t>> rankingPoints;
        const uint16_t posPoints[6] {12, 8, 5, 3, 1, 0}; // tener estos puntos independiente del numJugadores


        // multiplayer
        std::vector<uint8_t> charactersSelected;
};

