#pragma once

enum class stateNode { Success, Failure, InProcess };

class behaviourTree{

    public:
        // run() hace referencia a la accion a realizar 
		virtual bool run() = 0;

    private:
        stateNode   state;  

};