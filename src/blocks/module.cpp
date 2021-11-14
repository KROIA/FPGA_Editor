#include "module.h"

Module::Module()
{
    m_viewInside = false;
    m_isVisible  = true;
}
Module::Module(const Module &other)
{
    m_isVisible = other.m_isVisible;
    m_viewInside = other.m_viewInside;

    for(size_t i=0; i<other.m_gateList.size(); i++)
    {
        addGate(*other.m_gateList[i]);
    }
}
Module::~Module()
{
    for(size_t i=0; i<m_gateList.size(); i++)
    {
        delete m_gateList[i];
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

void Module::addIO(const string &name, Pin::Direction direction)
{
    IO_Def def{.name=name,.dir=direction};
    m_ioToAdd.push_back(def);
}

void Module::addGate(const Gate &gate)
{
    Gate *_gate = gate.clone();
    addGate(_gate);
}
void Module::addGate(Gate *gate)
{
    m_gateAddList.push_back(gate);
}
void Module::deleteGate(Gate *gate)
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
                delete m_gateList[a];
                m_gateList.erase(m_gateList.begin()+a);
            }
        }
    }

    // Add Gates
    for(size_t i=0; i<m_gateAddList.size(); i++)
    {
        m_gateAddList[i]->setParent(this);
        //m_gateAddList[i]->setVisibility(m_isVisible);
        connect(m_gateAddList[i],&Gate::deleteRequest,this,&Module::onGateDeleteRequest);
        m_gateList.push_back(m_gateAddList[i]);
    }

    m_gateDeleteList.clear();
    m_gateAddList.clear();

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
            insidePin->setParent(this);
            connect(insidePin,&Gate::deleteRequest,this,&Module::onIODeleteRequest);
        }
    }

    m_ioToAdd.clear();

    // Update the other Gates
    for(size_t i=0; i<m_gateList.size(); i++)
    {
        m_gateList[i]->utilityUpdate();
    }
}
void Module::readInputs()
{
    Gate::readInputs();
    for(size_t i=0; i<m_ioLeft.size(); i++)
    {
        m_ioLeft[i].insidePin->setVoltage(m_ioLeft[i].outsidePin->voltage());
    }
    for(size_t i=0; i<m_gateList.size(); i++)
    {
        m_gateList[i]->readInputs();
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
    for(size_t i=0; i<m_gateList.size(); i++)
    {
        m_gateList[i]->processLogic();
    }
}
void Module::setOutputs()
{
    for(size_t i=0; i<m_gateList.size(); i++)
    {
        m_gateList[i]->setOutputs();
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
        for(size_t i=0; i<m_gateList.size(); i++)
        {
            m_gateList[i]->onEventUpdate(window);
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
        for(size_t i=0; i<m_gateList.size(); i++)
        {
            m_gateList[i]->updateEvents(events,mouseDoubleClickEvent,window);
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
        for(size_t i=0; i<m_gateList.size(); i++)
        {
            m_gateList[i]->draw(window,drawPos-m_position);
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
        for(size_t i=0; i<m_gateList.size(); i++)
        {
            m_gateList[i]->drawDebug(window,drawPos);
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

