#include "logicGate_AND.h"


LogicGate_AND::LogicGate_AND(int inputs)
    :   Gate(inputs,1)
{
    m_label.setString("AND");
}
LogicGate_AND::LogicGate_AND(Vector2i pos,int inputs)
    :   Gate(pos,inputs,1)
{
    m_label.setString("AND");
}
LogicGate_AND::~LogicGate_AND()
{

}

void LogicGate_AND::processLogic()
{
    bool output = 1;
    for(size_t i=0; i<m_inputPins.size(); i++)
    {
        float voltage = m_inputPins[i]->voltage();
        bool logicLevl;
        if(voltage >= Physics::Electrical::voltage_logic_1_threshold)
           logicLevl = 1;
        else if(voltage <= Physics::Electrical::voltage_logic_0_threshold)
           logicLevl = 0;

        output &= logicLevl;
    }
    m_outputPins[0]->voltage(output * Physics::Electrical::voltage_max);
}
