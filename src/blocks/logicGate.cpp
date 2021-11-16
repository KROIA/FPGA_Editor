#include "logicGate.h"

LogicGate::LogicGate(const LogicGate &other)
    :   Gate(other)
{
    m_keyAddPressed = false;
    m_keyRemovePressed = false;
    m_logic = other.logic();
}
LogicGate::LogicGate(int inputs)
{
    Gate(inputs,1);
    m_label.setString("AND");
    m_keyAddPressed = false;
    m_keyRemovePressed = false;
    logic(Logic::AND);
    vector<Pin*> pins = getLeftPins();
    for(size_t i=0; i<pins.size(); i++)
    {
        pins[i]->name("in"+std::to_string(i+1));
    }
}
LogicGate::LogicGate(Vector2i pos,int inputs)
    :   Gate(pos,inputs,1)
{
    m_label.setString("AND");
    logic(Logic::AND);
    m_keyAddPressed = false;
    m_keyRemovePressed = false;
    vector<Pin*> pins = getLeftPins();
    for(size_t i=0; i<pins.size(); i++)
    {
        pins[i]->name("in"+std::to_string(i+1));
    }
}
LogicGate::~LogicGate()
{

}

LogicGate *LogicGate::clone() const
{
    return new LogicGate(*this);
}

void LogicGate::logic(Logic logic)
{
    m_logic = logic;
    Gate::name(logicToString(m_logic));
    if(m_logic == Logic::NOT)
    {
        while(Gate::leftSidePinCount() > 1)
            Gate::removeLastLeftPin();
    }
}
LogicGate::Logic LogicGate::logic() const
{
    return m_logic;
}

string LogicGate::logicToString(Logic logic)
{
    switch(logic)
    {
        case Logic::AND:     return "AND";
        case Logic::OR:      return "OR";
        case Logic::XOR:     return "XOR";
        case Logic::NAND:    return "NAND";
        case Logic::NOR:     return "NOR";
        case Logic::XNOR:    return "XNOR";
        case Logic::NOT:  return "NOT";
        default:
            break;
    }
    return "NOT DEFINED";
}
LogicGate::Logic LogicGate::stringToLogic(const string &logicStr)
{
    if(logicStr.find(logicToString(Logic::AND))!=string::npos) return Logic::AND;
    if(logicStr.find(logicToString(Logic::OR))!=string::npos) return Logic::OR;
    if(logicStr.find(logicToString(Logic::XOR))!=string::npos) return Logic::XOR;
    if(logicStr.find(logicToString(Logic::NAND))!=string::npos) return Logic::NAND;
    if(logicStr.find(logicToString(Logic::NOR))!=string::npos) return Logic::NOR;
    if(logicStr.find(logicToString(Logic::XNOR))!=string::npos) return Logic::XNOR;
    if(logicStr.find(logicToString(Logic::NOT))!=string::npos) return Logic::NOT;
    return __NOT_DEFINED;
}

LogicGate::LogicGate_Def LogicGate::getGateDef()
{
    LogicGate_Def def;
    def.logic = m_logic;
    def.pos = m_position;
    def.gateID = Gate::getObjectID();
    for(size_t i=0; i<Gate::getLeftSidePinCount(); i++)
    {
        def.pinNames.push_back(Gate::getLeftPins()[i]->name());
    }
    def.connections = Gate::getConnectionsDef();
    return def;
}

void LogicGate::processLogic()
{
    Gate::processLogic();
    bool output = true;
    bool invert = false;
    switch(m_logic)
    {
        case Logic::AND:      output=1; break;
        case Logic::OR:       output=0; break;
        case Logic::XOR:      output=0; break;
        case Logic::NAND:     output=1;invert = true; break;
        case Logic::NOR:      output=0;invert = true; break;
        case Logic::XNOR:     output=0;invert = true; break;
        case Logic::NOT:   invert = true; break;
        default:
            break;
    }
    for(size_t i=0; i<leftSidePinCount(); i++)
    {
        bool logicLevl = Physics::voltageToLogicLevel(inputVoltage(i));
        switch(m_logic)
        {
            case Logic::AND:
            case Logic::NAND:     output &= logicLevl; break;
            case Logic::OR:
            case Logic::NOR:      output |= logicLevl; break;
            case Logic::XOR:
            case Logic::XNOR:     output ^= logicLevl; break;
            case Logic::NOT:   output = logicLevl; break;
            default:
                break;
        }

    }
    if(invert)
        output = !output;
    outputVoltage(0,Physics::logicLevelToVoltage(output));
}

void LogicGate::onEventUpdate(sf::RenderWindow *window)
{
    Gate::onEventUpdate(window);

    if(m_logic != Logic::NOT)
    {
        if(m_moving)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
            {
                if(!m_keyAddPressed)
                {
                    string name = "in"+std::to_string(getLeftPins().size()+1);
                    Pin pin(Pin::Direction::input);
                    pin.name(name);
                    Gate::addPin(pin);
                    m_keyAddPressed = true;
                }
            }
            else
                m_keyAddPressed = false;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
            {
                if(!m_keyRemovePressed && getLeftPins().size() > 2)
                {
                    string name = "in"+std::to_string(getLeftPins().size());
                    Gate::removePin(name);
                    m_keyRemovePressed = true;
                }
            }
            else
                m_keyRemovePressed = false;
        }
        else
        {
            m_keyAddPressed = false;
            m_keyRemovePressed = false;
        }
    }

}
void LogicGate::onMouseOver(Vector2i mousePos)
{
    Gate::onMouseOver(mousePos);
}
void LogicGate::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    Gate::onKlick(mouseButton,mousePos);
}
