#ifndef LOGICGATE_CONST_H
#define LOGICGATE_CONST_H

#include "gate.h"

class LogicGate_CONST     :   public Gate
{
    public:
        struct ConstLogicGate_Def
        {
            Vector2i pos;
            size_t gateID;
            float voltage;
        };

        LogicGate_CONST();
        LogicGate_CONST(const LogicGate_CONST &other);
        LogicGate_CONST(Vector2i pos);
        ~LogicGate_CONST();

        virtual Gate *clone() const;

        void voltage(float voltage);
        void logicLevel(bool level);

        ConstLogicGate_Def getGateDef();

    private:
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);
        void updateLabel();

        void processLogic();
        float m_voltage;
        bool m_enabled;


};
#endif // LOGICGATE_CONST_H
