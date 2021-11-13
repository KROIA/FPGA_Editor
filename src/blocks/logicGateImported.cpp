#include "logicGateImported.h"

/*LogicGateImported::LogicGateImported(const string &filePath)
    :   Gate()
{
    try {
        m_module = new SimpleVerilog::Module(filePath);
    }  catch (std::runtime_error &e) {

    }

    setup();
}*/
LogicGateImported::LogicGateImported(const SimpleVerilog::Module &module)
{
    m_module = new SimpleVerilog::Module(module);
    setup();
}
LogicGateImported::LogicGateImported(const LogicGateImported &other)
    :   Gate(other)
{
    m_module = new SimpleVerilog::Module(*other.m_module);
    setup();
}
void LogicGateImported::setup()
{
    Gate::name(m_module->name());

    vector<SimpleVerilog::Pin> pins = m_module->pinList();
    vector<Pin> newPins;
    for(size_t i=0; i<pins.size(); i++)
    {
        Pin::Direction dir = Pin::Direction::inout;
        string name = pins[i].name();
        switch(pins[i].direction())
        {
            case SimpleVerilog::Pin::Direction::input:  dir = Pin::Direction::input; break;
            case SimpleVerilog::Pin::Direction::output: dir = Pin::Direction::output; break;
            case SimpleVerilog::Pin::Direction::inout:  dir = Pin::Direction::inout; break;
            default: {qDebug() << "direction not defined"; break;}
        }
        if(pins[i].dimension() > 1)
        {
            for(size_t j=0; j< pins[i].dimension(); j++)
            {
                Pin newPin(dir);
                newPin.name(pins[i].name() + "["+to_string(j)+"]");
                newPins.push_back(newPin);
            }
        }else
        {
            Pin newPin(dir);
            newPin.name(pins[i].name());
            newPins.push_back(newPin);
        }
    }
    Gate::addPin(newPins);
}

LogicGateImported::~LogicGateImported()
{

}

Gate *LogicGateImported::clone()
{
    return new LogicGateImported(*this);
}

LogicGateImported *LogicGateImported::importGate(const string &path)
{
    SimpleVerilog::Module *m_module = SimpleVerilog::Module::parseModule(path);
    if(m_module == nullptr)
        return nullptr;

    LogicGateImported *imported = new LogicGateImported(*m_module);

    delete m_module;
    return imported;
}

void LogicGateImported::processLogic()
{

}

