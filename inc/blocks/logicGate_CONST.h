#ifndef LOGICGATE_CONST_H
#define LOGICGATE_CONST_H

#include "gate.h"

class LogicGate_CONST     :   public Gate
{
    public:
        LogicGate_CONST();
        LogicGate_CONST(Vector2i pos);
        ~LogicGate_CONST();

        void voltage(float voltage);
        void logicLevel(bool level);

    private:
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);
        void updateLabel();

        void processLogic();
        float m_voltage;
        bool m_enabled;


};
#endif // LOGICGATE_CONST_H
