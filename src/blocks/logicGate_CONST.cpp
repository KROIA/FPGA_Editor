#include "logicGate_CONST.h"


LogicGate_CONST::LogicGate_CONST()
    :   Gate(0,1)
{
    voltage(5);
    m_label.setCharacterSize(80);
    m_enabled = false;

}
LogicGate_CONST::LogicGate_CONST(const LogicGate_CONST &other)
    :   Gate(other)
{
    m_voltage = other.m_voltage;
    m_enabled = other.m_enabled;
}
LogicGate_CONST::LogicGate_CONST(Vector2i pos)
    :   Gate(pos,0,1)
{
    voltage(5);
    m_label.setCharacterSize(80);
    m_enabled = false;

}
LogicGate_CONST::~LogicGate_CONST()
{

}

Gate *LogicGate_CONST::clone()
{
    return new LogicGate_CONST(*this);
}

void LogicGate_CONST::voltage(float voltage)
{
    m_voltage = voltage;
    updateLabel();
}
void LogicGate_CONST::logicLevel(bool level)
{
    m_voltage = Physics::Electrical::voltage_max*level;
    updateLabel();
}

void LogicGate_CONST::processLogic()
{
    if(m_enabled)
        outputVoltage(0,m_voltage);
    else
        outputVoltage(0,0);
}


void LogicGate_CONST::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    if(Physics::displayPhysical)
    {
        m_enabled = !m_enabled;
        updateLabel();
    }
    else
    {
        Gate::onKlick(mouseButton,mousePos);
    }

}
void LogicGate_CONST::updateLabel()
{
    if(m_enabled)
        m_label.setString("C: "+Physics::voltageString(m_voltage,2));
    else
        m_label.setString("C: "+Physics::voltageString(0,2));
}
