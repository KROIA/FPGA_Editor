#include "ioPin.h"

IoPin::IoPin(const string &name,Pin::Direction dir)
{
    m_direction = dir;
    if(m_direction == Pin::Direction::input)
        setupPins(0,1);
    else
        setupPins(1,0);
    Gate::name(name);
}
IoPin::IoPin(const IoPin &other)
    :   Gate(other)
{

}
IoPin::~IoPin()
{

}

void IoPin::setVoltage(float voltage)
{
    Gate::outputVoltage(0,voltage);
    Gate::setOutputs();
}

