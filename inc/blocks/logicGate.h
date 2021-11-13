#ifndef LOGICGATE_AND_H
#define LOGICGATE_AND_H

#include "gate.h"

class LogicGate_AND     :   public Gate
{
    public:
        LogicGate_AND(int inputs);
        LogicGate_AND(Vector2i pos,int inputs);
        ~LogicGate_AND();

    private:
        void processLogic();

};
#endif // LOGICGATE_AND_H
