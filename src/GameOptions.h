#pragma once

class GameOptions {
    public:
        virtual ~GameOptions(){};
        static GameOptions* GetInstance();

        void SetCharacter(int c)        { mainCharacter = c; };
        void SetMap(int m)              { mapGame = m; };

        int GetCharacter()              { return mainCharacter; };
        int GetMap()                    { return mapGame; };

    private:
        GameOptions() = default;;
        static GameOptions* instance;

        int mainCharacter { 0 };
        int mapGame { 0 };

};

