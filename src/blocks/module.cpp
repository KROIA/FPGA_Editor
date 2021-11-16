#include "module.h"

std::unordered_map<size_t,Module*> Module::m_global_moduleLibMap;


Module::Module()
{
    m_viewInside = false;
    m_isVisible  = true;
    m_isLibDef = false;
    m_libDefLink = nullptr;
  //  Gate::name("Module"+std::to_string(m_global_moduleLib.size()));
  //  m_global_moduleLib.push_back(this);
}
Module::Module(const Module &other)
    :   Gate(other)
{
    m_isLibDef = false;
    m_isVisible = other.m_isVisible;
    m_viewInside = other.m_viewInside;
    //m_libDefLink = other.m_libDefLink;
    m_libDefLink = nullptr;

    Gate::removeAllPins();

    for(size_t i=0; i<other.m_gateList.size(); i++)
    {
        addGate(*other.m_gateList[i]);
    }
    for(size_t i=0; i<other.m_logicGateList.size(); i++)
    {
        addGate(*other.m_logicGateList[i]);
    }
    for(size_t i=0; i<other.m_moduleGateList.size(); i++)
    {
        addGate(*other.m_moduleGateList[i]);
    }



    for(size_t i=0; i<other.m_ioLeft.size(); i++)
    {
        addIO(other.m_ioLeft[i].insidePin->name(),other.m_ioLeft[i].outsidePin->direction());
    }
    for(size_t i=0; i<other.m_ioRight.size(); i++)
    {
        addIO(other.m_ioRight[i].insidePin->name(),other.m_ioRight[i].outsidePin->direction());
    }
    utilityUpdate();

    // Map: otherGates* : thisGates*
    std::unordered_map<Gate*,Gate*> otherGateMap;
    for(size_t i=0; i<other.m_allGatesList.size(); i++)
    {
        otherGateMap[other.m_allGatesList[i]] = m_allGatesList[i];
    }
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        otherGateMap[other.m_ioLeft[i].insidePin] = m_ioLeft[i].insidePin;
    }
    for(size_t i=0; i<m_ioRight.size(); i++)
    {
        otherGateMap[other.m_ioRight[i].insidePin] = m_ioRight[i].insidePin;
    }

    // Create Connections
    for(size_t i=0; i<other.m_allGatesList.size(); i++)
    {
        for(size_t pin=0; pin<other.m_allGatesList[i]->getLeftSidePinCount(); pin++)
        {
            Gate *otherGateDest = other.m_allGatesList[i];
            Pin* otherPinDest = otherGateDest->getLeftSidePin(pin);
            const vector<Connection*> otherConnections = otherPinDest->getConnections();

            for(size_t con=0; con<otherConnections.size(); con++)
            {
                Pin* otherPinSrc = otherConnections[con]->startPin();
                if(otherPinSrc == otherPinDest)
                    otherPinSrc = otherConnections[con]->endPin();

                Gate* otherGateSrc = (Gate*)otherPinSrc->getOwner();

                otherGateMap[otherGateSrc]->createConnection(otherPinSrc->name(),otherGateMap[otherGateDest],otherPinDest->name());
            }
        }
    }
}
Module::~Module()
{
    for(size_t i=0; i<m_allGatesList.size(); i++)
    {
        delete m_allGatesList[i];
    }
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        delete m_ioLeft[i].insidePin;
    }
    for(size_t i=0; i<m_ioRight.size(); i++)
    {
        delete m_ioRight[i].insidePin;
    }
}

#define CONNECTMODULE(SENDER,TYPE,FUNC,RECEIVER,SLOT) \
    connect(SENDER  ,static_cast<void(Module::*)(TYPE)>(&Module::FUNC), \
            RECEIVER,static_cast<void(Module::*)(TYPE)>(&Module::SLOT));

Module *Module::clone()
{
    Module *cl = new Module(*this);
    cl->deleteOnEscape(true);
    if(m_isLibDef)
    {
       // m_cloneModuleList.push_back(cl);
        connect(cl,&Module::libMasterAddIO,this,&Module::addIO);
        CONNECTMODULE(cl,const Gate &,libMasterAddGate,this,addGate);
        CONNECTMODULE(cl,Gate *      ,libMasterAddGate,this,addGate);
        CONNECTMODULE(cl,Module &    ,libMasterAddGate,this,addGate);
        CONNECTMODULE(cl,Module *    ,libMasterAddGate,this,addGate);
        CONNECTMODULE(cl,const LogicGate &,libMasterAddGate,this,addGate);
        CONNECTMODULE(cl,LogicGate * ,libMasterAddGate,this,addGate);

      //  connect(cl,static_cast<void(Module::*)(const Gate &)>(&Module::libMasterAddGate),
      //         this,static_cast<void(Module::*)(const Gate &)>(&Module::addGate));
      //  connect(cl,SIGNAL(libMasterAddGate(const Gate &gate)),this,SLOT(addGate(const Gate &gate)));
      //  connect(cl,SIGNAL(libMasterAddGate(Gate *gate)),this,SLOT(addGate(Gate *gate)));
      //  connect(cl,SIGNAL(libMasterAddGate(Module &gate)),this,SLOT(addGate(Module &gate)));
      //  connect(cl,SIGNAL(libMasterAddGate(Module *gate)),this,SLOT(addGate(Module *gate)));
      //  connect(cl,SIGNAL(libMasterAddGate(const LogicGate &gate)),this,SLOT(addGate(const LogicGate &gate)));
      //  connect(cl,SIGNAL(libMasterAddGate(LogicGate *gate)),this,SLOT(addGate(LogicGate *gate)));
        connect(cl,&Module::libMasterDeleteGate,this,&Module::deleteGate);


        connect(this,&Module::libMasterAddIO,cl,&Module::onLibMasterAddIO);
        CONNECTMODULE(this,const Gate &,libMasterAddGate,cl,onLibMasterAddGate);
        CONNECTMODULE(this,Gate *      ,libMasterAddGate,cl,onLibMasterAddGate);
        CONNECTMODULE(this,Module &    ,libMasterAddGate,cl,onLibMasterAddGate);
        CONNECTMODULE(this,Module *    ,libMasterAddGate,cl,onLibMasterAddGate);
        CONNECTMODULE(this,const LogicGate &,libMasterAddGate,cl,onLibMasterAddGate);
        CONNECTMODULE(this,LogicGate * ,libMasterAddGate,cl,onLibMasterAddGate);
       // connect(this,SIGNAL(libMasterAddGate(const Gate &gate)),cl,SLOT(onLibMasterAddGate(const Gate &gate)));
       // connect(this,SIGNAL(libMasterAddGate(Gate *gate)),cl,SLOT(onLibMasterAddGate(Gate *gate)));
       // connect(this,SIGNAL(libMasterAddGate(Module &gate)),cl,SLOT(onLibMasterAddGate(Module &gate)));
       // connect(this,SIGNAL(libMasterAddGate(Module *gate)),cl,SLOT(onLibMasterAddGate(Module *gate)));
       // connect(this,SIGNAL(libMasterAddGate(const LogicGate &gate)),cl,SLOT(onLibMasterAddGate(const LogicGate &gate)));
       // connect(this,SIGNAL(libMasterAddGate(LogicGate *gate)),cl,SLOT(onLibMasterAddGate(LogicGate *gate)));
        connect(this,&Module::libMasterDeleteGate,cl,&Module::onLibMasterDeleteGate);
        cl->m_libDefLink = this;

    }
    return cl;
}
Module *Module::createLibBlock(ModuleData data)
{
    m_moduleDef = data;
    Module *module = new Module();
    module->name(data.name);
    module->m_isLibDef = true;
    module->m_libDefLink = nullptr;

    for(size_t i=0; i<data.ioDefList.size(); i++)
    {

    }
}
void Module::isLibBlock(bool enable)
{
    if(enable)
    {
        m_isLibDef = true;
        m_libDefLink = this;
    }
    else
    {
        m_isLibDef = false;
        m_libDefLink = nullptr;
    }

}

void Module::addIO(const string &name, Pin::Direction direction)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddIO(name,direction);
        return;
    }
    onLibMasterAddIO(name,direction);
    emit libMasterAddIO(name,direction);
}
void Module::onLibMasterAddIO(const string &name, Pin::Direction direction)
{
    IO_Def def{.name=name,.dir=direction};
    m_ioToAdd.push_back(def);
}

void Module::addGate(const Gate &gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);
}
void Module::onLibMasterAddGate(const Gate &gate)
{
    Gate *_gate = gate.clone();
    //addGate(_gate);
    m_gateAddList.push_back(_gate);
}


void Module::addGate(Gate *gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);

}
void Module::onLibMasterAddGate(Gate *gate)
{
    m_gateAddList.push_back(gate);
}


void Module::addGate(Module &gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);
}
void Module::onLibMasterAddGate(Module &gate)
{
    Module *_gate = gate.clone();
    //addGate(_gate);
    m_moduleGateAddList.push_back(_gate);
}

void Module::addGate(Module *gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);
}
void Module::onLibMasterAddGate(Module *gate)
{
    m_moduleGateAddList.push_back(gate);
}

void Module::addGate(const LogicGate &gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);

}
void Module::onLibMasterAddGate(const LogicGate &gate)
{
    LogicGate *_gate = gate.clone();
    //addGate(_gate);
    m_logicGateAddList.push_back(_gate);
}



void Module::addGate(LogicGate *gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterAddGate(gate);
        return;
    }
    onLibMasterAddGate(gate);
    emit libMasterAddGate(gate);
}
void Module::onLibMasterAddGate(LogicGate *gate)
{
    m_logicGateAddList.push_back(gate);
}


void Module::deleteGate(Gate *gate)
{
    if(!m_isLibDef && m_libDefLink != nullptr)
    {
        emit libMasterDeleteGate(gate);
        return;
    }
    onLibMasterDeleteGate(gate);
    emit libMasterDeleteGate(gate);
}
void Module::onLibMasterDeleteGate(Gate *gate)
{
    m_gateDeleteList.push_back(gate);
}


void Module::setViewInside(bool inside)
{
    if(inside == m_viewInside)
        return;
    m_viewInside = inside;
    if(m_viewInside)
        emit insideViewEnter(this);
    else
        emit insideViewExit(this);
}

// Processing
void Module::utilityUpdate()
{
    // Remove Gates
    for(size_t i=0; i<m_gateDeleteList.size(); i++)
    {
        for(size_t a=0; a<m_gateList.size(); a++)
        {
            if(m_gateList[a] == m_gateDeleteList[i])
            {
                m_gateList.erase(m_gateList.begin()+a);
            }
        }
        for(size_t a=0; a<m_logicGateList.size(); a++)
        {
            if(m_logicGateList[a] == m_gateDeleteList[i])
            {
                m_logicGateList.erase(m_logicGateList.begin()+a);
            }
        }
        for(size_t a=0; a<m_moduleGateList.size(); a++)
        {
            if(m_moduleGateList[a] == m_gateDeleteList[i])
            {
                m_moduleGateList.erase(m_moduleGateList.begin()+a);
            }
        }
        for(size_t a=0; a<m_allGatesList.size(); a++)
        {
            if(m_allGatesList[a] == m_gateDeleteList[i])
            {
                delete m_allGatesList[a];
                m_allGatesList.erase(m_allGatesList.begin()+a);
            }
        }

    }
    m_gateDeleteList.clear();

    // Add Gates
    for(size_t i=0; i<m_gateAddList.size(); i++)
    {
        m_gateAddList[i]->setOwner(this);
        //m_gateAddList[i]->setVisibility(m_isVisible);
        connect(m_gateAddList[i],&Gate::deleteRequest,this,&Module::onGateDeleteRequest);
        m_gateList.push_back(m_gateAddList[i]);
        m_allGatesList.push_back(m_gateAddList[i]);
    }
    for(size_t i=0; i<m_logicGateAddList.size(); i++)
    {
        m_logicGateAddList[i]->setOwner(this);
        //m_gateAddList[i]->setVisibility(m_isVisible);
        connect(m_logicGateAddList[i],&Gate::deleteRequest,this,&Module::onGateDeleteRequest);
        m_logicGateList.push_back(m_logicGateAddList[i]);
        m_allGatesList.push_back(m_logicGateAddList[i]);
    }
    for(size_t i=0; i<m_moduleGateAddList.size(); i++)
    {
        m_moduleGateAddList[i]->setOwner(this);
        //m_gateAddList[i]->setVisibility(m_isVisible);
        connect(m_moduleGateAddList[i],&Gate::deleteRequest,this,&Module::onGateDeleteRequest);
        m_moduleGateList.push_back(m_moduleGateAddList[i]);
        m_allGatesList.push_back(m_moduleGateAddList[i]);
    }

    m_gateAddList.clear();
    m_logicGateAddList.clear();
    m_moduleGateAddList.clear();



    // Remove IO's
    for(size_t j=0; j<m_ioToDelete.size(); j++)
    {
        for(size_t i=0; i<m_ioLeft.size(); i++)
        {
            if(m_ioLeft[i].insidePin == m_ioToDelete[j])
            {
                Gate::removePin(m_ioLeft[i].outsidePin->name());

                delete m_ioLeft[i].insidePin;
                m_ioLeft.erase(m_ioLeft.begin()+i);

            }
        }
        for(size_t i=0; i<m_ioRight.size(); i++)
        {
            if(m_ioRight[i].insidePin == m_ioToDelete[j])
            {
                Gate::removePin(m_ioRight[i].outsidePin->name());
                delete m_ioRight[i].insidePin;
                m_ioRight.erase(m_ioRight.begin()+i);

            }
        }
    }
    m_ioToDelete.clear();
    // Add IO's
    for(size_t i=0; i<m_ioToAdd.size(); i++)
    {
        if(Gate::getPin(m_ioToAdd[i].name) == nullptr)
        {
            //qDebug() << "Pin name already exists";



            Pin p(m_ioToAdd[i].dir);
            p.name(m_ioToAdd[i].name);
            addPin(p);
            Pin *p_pin = Gate::getLeftSidePin(m_ioToAdd[i].name);
            IoPin *insidePin;


            IO io;



            if(p_pin != nullptr)
            {
                // Left side Pin
                insidePin = new IoPin(m_ioToAdd[i].name,Pin::Direction::input);
                io.insidePin = insidePin;
                io.outsidePin = p_pin;
                m_ioLeft.push_back(io);
                insidePin->setPos(Vector2i(50,50));
                bool overlap = false;
                do{
                    overlap = false;
                    for(size_t j=0; j<m_ioLeft.size()-1; j++)
                    {
                        if(insidePin->overlaps(*m_ioLeft[j].insidePin))
                        {
                            insidePin->setPos(insidePin->getPos()+Vector2i(0,50));
                            overlap = true;
                        }
                    }
                }while(overlap);
            }
            else
            {
                // Right side Pin
                p_pin = Gate::getRightSidePin(m_ioToAdd[i].name);

                insidePin = new IoPin(m_ioToAdd[i].name,Pin::Direction::output);
                io.insidePin = insidePin;
                insidePin->setRotation(0);
                io.outsidePin = p_pin;
                m_ioRight.push_back(io);
                insidePin->setPos(Vector2i(500,50));

                bool overlap = false;
                do{
                    overlap = false;
                    for(size_t j=0; j<m_ioRight.size()-1; j++)
                    {
                        if(insidePin->overlaps(*m_ioRight[j].insidePin))
                        {
                            insidePin->setPos(insidePin->getPos()+Vector2i(0,50));
                            overlap = true;
                        }
                    }
                }while(overlap);
            }
            insidePin->setOwner(this);
            connect(insidePin,&Gate::deleteRequest,this,&Module::onIODeleteRequest);
        }
    }

    m_ioToAdd.clear();

    // Update the other Gates
    for(size_t i=0; i<m_allGatesList.size(); i++)
    {
        m_allGatesList[i]->utilityUpdate();
    }
}
void Module::readInputs()
{
    Gate::readInputs();
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        m_ioLeft[i].insidePin->setVoltage(m_ioLeft[i].outsidePin->voltage());
    }
    for(size_t i=0; i<m_allGatesList.size(); i++)
    {
        m_allGatesList[i]->readInputs();
    }

}
void Module::processLogic()
{
    Gate::processLogic();
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        m_ioLeft[i].insidePin->processLogic();
    }
    for(size_t i=0; i<m_ioRight.size(); i++)
    {
        m_ioRight[i].insidePin->processLogic();
    }
    for(size_t i=0; i<m_allGatesList.size(); i++)
    {
        m_allGatesList[i]->processLogic();
    }


}
void Module::setOutputs()
{
    for(size_t i=0; i<m_allGatesList.size(); i++)
    {
        m_allGatesList[i]->setOutputs();
    }


    for(size_t i=0; i<m_ioRight.size(); i++)
    {
        m_ioRight[i].insidePin->readInputs();
        m_ioRight[i].outsidePin->voltage(m_ioRight[i].insidePin->inputVoltage(0));
    }
    //Gate::setOutputs();
}

void Module::onGateDeleteRequest(Gate *gate)
{
    deleteGate(gate);
}
void Module::onIODeleteRequest(Gate *gate)
{
    m_ioToDelete.push_back(gate);
}

void Module::onEventUpdate(sf::RenderWindow *window)
{
    if(m_viewInside)
    {
        for(size_t i=0; i<m_allGatesList.size(); i++)
        {
            m_allGatesList[i]->onEventUpdate(window);
        }


    }
    else
    {
        Gate::onEventUpdate(window);
    }
}
void Module::onMouseOver(Vector2i mousePos)
{
    if(m_viewInside)
    {

    }
    else
    {
        Gate::onMouseOver(mousePos);
    }
}
void Module::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    if(m_viewInside)
    {

    }
    else
    {
        Gate::onKlick(mouseButton,mousePos);
    }
}

void Module::onDoubleKlick()
{
    //qDebug() << "double Klick";
    if(Tool::getSelected() == nullptr || (Tool::getSelected() != nullptr && !Tool::getSelected()->isUsed()))
        setViewInside(true);
}
void Module::updateEvents(vector<sf::Event> *events,
                          bool mouseDoubleClickEvent,
                          sf::RenderWindow *window)
{
    if(m_viewInside)
    {
        for(size_t i=0; i<m_allGatesList.size(); i++)
        {
            m_allGatesList[i]->updateEvents(events,mouseDoubleClickEvent,window);
        }

        for(size_t i=0; i<m_ioLeft.size(); i++)
        {
            m_ioLeft[i].insidePin->updateEvents(events,mouseDoubleClickEvent,window);
        }
        for(size_t i=0; i<m_ioRight.size(); i++)
        {
            m_ioRight[i].insidePin->updateEvents(events,mouseDoubleClickEvent,window);
        }
    }
    else
    {
        Gate::updateEvents(events,mouseDoubleClickEvent,window);
    }
}
void Module::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
    {
        //m_isVisible = m_parent->isVisible();
        drawPos += m_parent->getPos();
    }

    if(m_viewInside)
    {
        for(size_t i=0; i<m_allGatesList.size(); i++)
        {
            m_allGatesList[i]->draw(window,drawPos-m_position);
        }

        for(size_t i=0; i<m_ioLeft.size(); i++)
        {
            m_ioLeft[i].insidePin->draw(window,drawPos-m_position);
        }
        for(size_t i=0; i<m_ioRight.size(); i++)
        {
            m_ioRight[i].insidePin->draw(window,drawPos-m_position);
        }
    }
    else
    {
        Gate::draw(window,drawPos);
    }
}
void Module::drawDebug(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
    {
        //m_isVisible = m_parent->isVisible();
        drawPos += m_parent->getPos();
    }
    //if(!m_isVisible)
    //    return;
    if(m_viewInside)
    {
        for(size_t i=0; i<m_allGatesList.size(); i++)
        {
            m_allGatesList[i]->drawDebug(window,drawPos);
        }

        for(size_t i=0; i<m_ioLeft.size(); i++)
        {
            m_ioLeft[i].insidePin->drawDebug(window,drawPos-m_position);
        }
        for(size_t i=0; i<m_ioRight.size(); i++)
        {
            m_ioRight[i].insidePin->drawDebug(window,drawPos-m_position);
        }
    }
    else
    {
        Gate::drawDebug(window,drawPos);
    }
}


Module::ModuleData Module::getModuleDef()
{
    ModuleData data;
    data.name = Gate::name();
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        IO_Def ioDef;
        ioDef.name = m_ioLeft[i].insidePin->name();
        ioDef.dir  = m_ioLeft[i].insidePin->direction();
        ioDef.pos  = m_ioLeft[i].insidePin->getPos();

        data.ioDefList.push_back(ioDef);
    }
    for(size_t i=0; i<m_ioRight.size(); i++)
    {
        IO_Def ioDef;
        ioDef.name = m_ioRight[i].insidePin->name();
        ioDef.dir  = m_ioRight[i].insidePin->direction();
        ioDef.pos  = m_ioRight[i].insidePin->getPos();

        data.ioDefList.push_back(ioDef);
    }

    for(size_t i=0; i<m_logicGateList.size(); i++)
    {
        data.logicGates.push_back(m_logicGateList[i]->getGateDef());
    }
    for(size_t i=0; i<m_moduleGateList.size(); i++)
    {
        data.moduleGates.push_back(m_moduleGateList[i]->getModuleGateDef());
    }
    return data;
}

Module::ModuleGate_Def Module::getModuleGateDef()
{
    ModuleGate_Def def;
    def.name = Gate::name();
    def.pos  = Gate::getPos();
    def.connections = Gate::getConnectionsDef();
    return def;
}
