#ifndef LogicGate_H
#define LogicGate_H

#include "gate.h"

class LogicGate     :   public Gate
{
    public:
        enum Logic
        {
            AND = 0,
            OR  = 1,
            XOR = 2,
            NAND = 3,
            NOR  = 4,
            XNOR = 5,
            NOT = 10,

            __NOT_DEFINED
        };

        LogicGate(const LogicGate &other);
        LogicGate(int inputs);
        LogicGate(Vector2i pos,int inputs);
        ~LogicGate();

        virtual Gate *clone();

        void logic(Logic logic);
        Logic logic() const;

        static string logicToString(Logic logic);
        static Logic stringToLogic(const string &logicStr);

    private:
        void processLogic();


        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);

        bool m_keyAddPressed;
        bool m_keyRemovePressed;

        Logic m_logic;
};
#endif // LogicGate_H
