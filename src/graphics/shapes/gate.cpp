#include "gate.h"

sf::Font* Gate::m_font = nullptr;
//vector<Gate *> Gate::m_globalGatelIst;

const sf::Keyboard::Key Gate::key_shiftCopy = sf::Keyboard::Key::LShift;

Gate::Gate()
{
    setup();
}
Gate::Gate(int inputs, int outputs)
{
    setup(inputs,outputs);
}
Gate::Gate(Vector2i pos,int inputs, int outputs)
{
    setup(inputs,outputs);
    m_position = pos;
}
Gate::Gate(const Gate &other)
    :   Shape(other)
{
    //m_globalGatelIst.push_back(this);
    //m_leftSidePinList = other.m_leftSidePinList;
    //m_rightSidePinList = other.m_rightSidePinList;
    m_inputValues = other.m_inputValues;
    m_outputValues = other.m_outputValues;
    m_label = other.m_label;
    m_position = other.m_position;
    m_angle  = other.m_angle;
    m_moving = false;
    m_dimensions = other.m_dimensions;
    for(size_t i=0; i<other.m_leftSidePinList.size(); i++)
    {
        internalAddPin(*other.m_leftSidePinList[i]);
    }
    for(size_t i=0; i<other.m_rightSidePinList.size(); i++)
    {
        internalAddPin(*other.m_rightSidePinList[i]);
    }
    updateGate();
}
Gate *Gate::clone()const
{
    Gate *cl = new Gate(*this);
    cl->deleteOnEscape(true);
    return cl;
}
void Gate::setup()
{
    //m_globalGatelIst.push_back(this);
    //int inputPins = 4;
    //int outputPins = 2;
    m_position = Vector2i(0,0);
    m_moving = false;
    m_isCopying = false;
    m_angle = 0;
    if(m_font == nullptr)
    {
        m_font = new sf::Font();
        if(!m_font->loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        {
            qDebug() << "ERROR: Gate::Gate() can't load Font";
        }
    }
    m_label.setFont(*m_font);
    m_label.setString("Gate");
    m_label.scale(0.1,0.1);
    m_label.setCharacterSize(150);
    //m_label.setScale(100,100);
    m_label.setFillColor(Color(20,20,20));
    m_label.setStyle(sf::Text::Bold);


}
void Gate::setup(int inputs, int outputs)
{
    setup();

//****
    /*if(inputs > outputs)
    {

        m_dimensions = Vector2i(m_gridsize.x*5,m_gridsize.y*(inputs*2));
    }else
    {
        m_dimensions = Vector2i(m_gridsize.x*5,m_gridsize.y*(outputs*2));
    }



    m_boundingBox.set(Vector2i(m_position.x-m_dimensions.x/2,m_position.y-m_dimensions.y/2),
                      m_position + m_dimensions);*/

    setupPins(inputs,outputs);
}

Gate::~Gate()
{
    /*for(size_t i=0; i<m_globalGatelIst.size(); i++)
        if(m_globalGatelIst[i] == this)
            m_globalGatelIst.erase(m_globalGatelIst.begin() + i);*/

    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        delete m_leftSidePinList[i];
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        delete m_rightSidePinList[i];
    }
}

void Gate::updateEvents(vector<sf::Event> *events,
                        bool mouseDoubleClickEvent,
                        sf::RenderWindow *window)
{
    Shape::updateEvents(events,mouseDoubleClickEvent,window);
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        m_leftSidePinList[i]->updateEvents(events,mouseDoubleClickEvent,window);
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        m_rightSidePinList[i]->updateEvents(events,mouseDoubleClickEvent,window);
    }
}
void Gate::onEventUpdate(sf::RenderWindow *window)
{
    if(m_moving)
    {
        //sf::Vector2i pixelPos = sf::Mouse::getPosition() - window->getPosition() - sf::Vector2i(window->getView().getViewport().getPosition());
        sf::Vector2i pixelPos = sf::Vector2i(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
        setPos(pixelPos);
    }

}
void Gate::onMouseOver(Vector2i mousePos)
{

}
void Gate::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    emit clicked(this);
    switch(Tool::getSelectedType())
    {
        case Tool::Type::moveModifier:
        {
            switch(mouseButton)
            {
                case sf::Mouse::Button::Left:
                {
                    if(!Tool::getSelected()->isUsed())
                    {
                        Tool::getSelected()->isUsed(true);
                        Tool::startListen(this);
                        m_moving = true;
                        m_movingOldPos = m_position;
                        emit startsMoving(this);
                    }
                    else if(m_moving)
                    {
                        Tool::getSelected()->isUsed(false);
                        Tool::endListen(this);
                        m_moving = false;
                        m_isCopying = false;
                        emit placed(this);
                        if(sf::Keyboard::isKeyPressed(key_shiftCopy))
                        {
                            emit addCopyOf(this);
                        }
                    }
                    break;
                }
                case sf::Mouse::Button::Right:
                {
                    if(m_moving)
                        rotate();
                    break;
                }
            }
            break;
        }
        case Tool::Type::moduleRemover:
        {
            emit deleteRequest(this);
            break;
        }
    }
}
void Gate::onDoubleKlick()
{

}

void Gate::internalAddPin(const Pin &pin)
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        if(m_leftSidePinList[i]->name() == pin.name())
        {
            qDebug() << "Pin already exists";
            return;
        }
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        if(m_rightSidePinList[i]->name() == pin.name())
        {
            qDebug() << "Pin already exists";
            return;
        }
    }

    if(pin.direction() == Pin::Direction::output)
    {
        Pin *p = new Pin(pin);
        p->angle(0);
        p->setOwner(this);
        m_rightSidePinList.push_back(p);
    }
    else
    {
        Pin *p = new Pin(pin);
        p->angle(180);
        p->setOwner(this);
        m_leftSidePinList.push_back(p);
    }
}
void Gate::name(const string &name)
{
    m_label.setString(name);
}
string Gate::name() const
{
    return m_label.getString();
}
void Gate::addPin(const Pin& pin)
{
    internalAddPin(pin);
    updateGate();
}

void Gate::addPin(const vector<Pin> &pinList)
{
    for(size_t i=0; i<pinList.size(); i++)
    {
        internalAddPin(pinList[i]);
    }
    updateGate();
}
Pin* Gate::getLeftSidePin(const string &name) const
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        if(m_leftSidePinList[i]->name() == name)
        {
            return m_leftSidePinList[i];
        }
    }
    return nullptr;
}
Pin* Gate::getLeftSidePin(size_t index) const
{
    if(index >= m_leftSidePinList.size())
    {
        qDebug() << "ERROR: index out of range";
        return nullptr;
    }
    return m_leftSidePinList[index];
}
Pin* Gate::getRightSidePin(const string &name) const
{
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        if(m_rightSidePinList[i]->name() == name)
        {
            return m_rightSidePinList[i];
        }
    }
    return nullptr;
}
Pin* Gate::getRightSidePin(size_t index) const
{
    if(index >= m_rightSidePinList.size())
    {
        qDebug() << "ERROR: index out of range";
        return nullptr;
    }
    return m_rightSidePinList[index];
}
size_t Gate::getLeftSidePinCount() const
{
    return m_leftSidePinList.size();
}
size_t Gate::getRightSidePinCount() const
{
    return m_rightSidePinList.size();
}
Pin* Gate::getPin(const string &name) const
{
    Pin* a = getLeftSidePin(name);
    if(a!=nullptr)
        return a;

    a = getRightSidePin(name);
    if(a!=nullptr)
        return a;

    return nullptr;
}
void Gate::removePin(const string &name)
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        if(m_leftSidePinList[i]->name() == name)
        {
            delete m_leftSidePinList[i];
            m_leftSidePinList.erase(m_leftSidePinList.begin()+i);
            updateGate();
            return;
        }
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        if(m_rightSidePinList[i]->name() == name)
        {
            delete m_rightSidePinList[i];
            m_rightSidePinList.erase(m_rightSidePinList.begin()+i);
            updateGate();
            return;
        }
    }
}
void Gate::removeLastLeftPin()
{
    if(m_leftSidePinList.size()-1 == 0)
        return;
    delete m_leftSidePinList[m_leftSidePinList.size()-1];
    m_leftSidePinList.pop_back();
    updateGate();
}
void Gate::removeLastRightPin()
{
    if(m_rightSidePinList.size()-1 == 0)
        return;
    delete m_rightSidePinList[m_rightSidePinList.size()-1];
    m_rightSidePinList.pop_back();
    updateGate();
}
void Gate::removeAllLeftPins()
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        delete m_leftSidePinList[i];
    }
    m_leftSidePinList.clear();
    updateGate();
}
void Gate::removeAllRightPins()
{
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        delete m_rightSidePinList[i];
    }
    m_rightSidePinList.clear();
    updateGate();
}
void Gate::removeAllPins()
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        delete m_leftSidePinList[i];
    }
    m_leftSidePinList.clear();
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        delete m_rightSidePinList[i];
    }
    m_rightSidePinList.clear();
    updateGate();

}
void Gate::createConnection(const string &thisPinName,
                      Gate *other,const string &otherPinName)
{
    Pin *otherPin = other->getPin(otherPinName);
    if(otherPin == nullptr)
    {
        qDebug() << "ERROR Pin: "<<otherPinName.c_str() << " doesen't exist";
        return;
    }
    createConnection(thisPinName,otherPin);
}
void Gate::createConnection(const string &thisPinName,Pin *otherPin)
{
    Pin *pin = getPin(thisPinName);
    if(pin == nullptr)
    {
        qDebug() << "ERROR Pin: "<<thisPinName.c_str() << " doesen't exist";
        return;
    }
    createConnection(pin,otherPin);
}
void Gate::createConnection(Pin *thisPin,Pin *otherPin)
{
    thisPin->createConnection(otherPin);
}

void Gate::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
    {
       // m_isVisible = m_parent->isVisible();
        drawPos += m_parent->getPos();
    }
    if(!m_isVisible)
        return;

    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        m_leftSidePinList[i]->draw(window,drawPos);
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        m_rightSidePinList[i]->draw(window,drawPos);
    }

    drawPos += m_position;
    m_boundingBox.set(Vector2i(drawPos.x-m_dimensions.x/2,drawPos.y-m_dimensions.y/2),
                      Vector2i(drawPos.x+m_dimensions.x/2,drawPos.y+m_dimensions.y/2));



    /*if(m_parent != nullptr)
    {
        drawPos + m_parent->getPos();
    }*/

    sf::ConvexShape convex;

    // resize it to 5 points
    convex.setPointCount(4);

    // define the points
    convex.setPoint(0, sf::Vector2f(-m_dimensions.x/2, -m_dimensions.y/2));
    convex.setPoint(1, sf::Vector2f( m_dimensions.x/2, -m_dimensions.y/2));
    convex.setPoint(2, sf::Vector2f( m_dimensions.x/2,  m_dimensions.y/2));
    convex.setPoint(3, sf::Vector2f(-m_dimensions.x/2,  m_dimensions.y/2));

    convex.setFillColor(sf::Color(80,80,80));
    convex.setPosition((Vector2f(drawPos)));


    sf::FloatRect labelBounds = m_label.getGlobalBounds();
    m_label.setOrigin(labelBounds.width/2/m_label.getScale().x,
                      (labelBounds.height-labelBounds.height/10)/m_label.getScale().y);
    m_label.setPosition((Vector2f(drawPos)));
   // qDebug() << "Bounds: "<<labelBounds.width << " " <<labelBounds.height;

    /**Color col(255,0,0);
    sf::Vertex line[] =
    {
        sf::Vertex(Vector2f(drawPos),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(labelBounds.width,0)),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(labelBounds.width,labelBounds.height)),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(0,labelBounds.height)),col)
    };
*/

   /* for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
       // qDebug() << "drawPin";
        m_leftSidePinList[i].draw(window,drawPos);
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        m_rightSidePinList[i].draw(window,drawPos);
    }*/



    window->draw(convex);
    window->draw(m_label);
   // window->draw(line,4,sf::LineStrip);
}
void Gate::drawDebug(sf::RenderWindow *window, Vector2i drawPos)
{
    if(!m_isVisible)
        return;
    Shape::drawDebug(window,drawPos);
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        m_leftSidePinList[i]->drawDebug(window,drawPos);
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        m_rightSidePinList[i]->drawDebug(window,drawPos);
    }
}

void Gate::inputVoltage(size_t index, float voltage)
{
    if(index >= m_inputValues.size())
    {
        qDebug() << "ERROR: Gate::inputVoltage(size_t index, float voltage): index out of range";
        return;
    }
    m_inputValues[index] = voltage;
}
float Gate::inputVoltage(size_t index)
{
    if(index >= m_inputValues.size())
    {
        qDebug() << "ERROR: Gate::inputVoltage(size_t index): index out of range";
        return 0;
    }
    return m_inputValues[index];
}

void Gate::outputVoltage(size_t index, float voltage)
{
    if(index >= m_outputValues.size())
    {
        qDebug() << "ERROR: Gate::outputVoltage(size_t index, float voltage): index out of range";
        return;
    }
    m_outputValues[index] = voltage;
}

float Gate::outputVoltage(size_t index)
{
    if(index >= m_outputValues.size())
    {
        qDebug() << "ERROR: Gate::outputVoltage(size_t index): index out of range";
        return 0;
    }
    return m_outputValues[index];
}
size_t Gate::leftSidePinCount() const
{
    return m_leftSidePinList.size();
}
size_t Gate::rightSidePinCount() const
{
    return m_rightSidePinList.size();
}

void Gate::snapToMouse(bool enable)
{
    if(enable)
    {
        Tool::startListen(this);
        m_moving = true;
        m_movingOldPos = m_position;
    }
    else
    {
        Tool::endListen(this);
        m_moving = false;
    }
}
void Gate::deleteOnEscape(bool enable)
{
    m_isCopying = true;
}
void Gate::rotate()
{
    m_angle = (m_angle+90)%360;
    updateGate();
}
void Gate::rotate(int angle)
{
    m_angle = (m_angle+angle)%360;
    updateGate();
}
void Gate::setRotation(int angle)
{
    m_angle = angle%360;
    updateGate();
}
/*void Gate::global_processLogic()
{
    for(size_t i=0; i<m_globalGatelIst.size(); i++)
        m_globalGatelIst[i]->readInputs();

    for(size_t i=0; i<m_globalGatelIst.size(); i++)
        m_globalGatelIst[i]->processLogic();

    for(size_t i=0; i<m_globalGatelIst.size(); i++)
        m_globalGatelIst[i]->setOutputs();
}
*/
void Gate::utilityUpdate()
{

}
void Gate::readInputs()
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
        m_inputValues[i] = m_leftSidePinList[i]->voltage();
}
void Gate::processLogic()
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        m_leftSidePinList[i]->processLogic();
    }
}
void Gate::setOutputs()
{
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
        m_rightSidePinList[i]->voltage(m_outputValues[i]);
}

vector<Gate::Connection_Def> Gate::getConnectionsDef()
{
    vector<Connection_Def> defList;
    for(size_t i=0; i<m_connectionList.size(); i++)
    {
        Connection_Def def;
        def.beginGateID = m_connectionList[i]->startPin()->getOwner()->getObjectID();
        def.beginPin    = m_connectionList[i]->startPin()->name();

        def.endGateID = m_connectionList[i]->endPin()->getOwner()->getObjectID();
        def.endPin    = m_connectionList[i]->endPin()->name();
        defList.push_back(def);
    }
    return defList;
}

void Gate::onConnectionCreate(Pin *pin,Connection *con)
{
    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        if(m_leftSidePinList[i] == pin)
            m_connectionList.push_back(con);
    }

}
void Gate::onConnectionRemove(Pin *pin,Connection *con)
{
    for(size_t i=0; i<m_connectionList.size(); i++)
    {
        if(m_connectionList[i] == con)
        {
            m_connectionList.erase(m_connectionList.begin()+i);
        }
    }
}

void Gate::toolChanged(Tool *oldTool, Tool *newTool)
{
    toolCleared(oldTool);
}
void Gate::toolCleared(Tool *oldTool)
{
    Tool::endListen(this);
    m_moving = false;
    if(m_isCopying)
    {
        m_isVisible = false;
        emit deleteRequest(this);
    }
    setPos(m_movingOldPos);
}

void Gate::setupPins(int inputs, int outputs)
{
    vector<Pin> pinList;
    for(int i=0; i<inputs; i++)
    {
        Pin pin(Pin::Direction::input);
        pin.name("in_"+std::to_string(i+1));
        pinList.push_back(pin);
    }
    for(int i=0; i<outputs; i++)
    {
        Pin pin(Pin::Direction::output);
        pin.name("out_"+std::to_string(i+1));
        pinList.push_back(pin);
    }

    addPin(pinList);

}
void Gate::updateGate()
{
    int pinSpacing;
    m_inputValues.clear();
    m_outputValues.clear();
    if(m_leftSidePinList.size() > m_rightSidePinList.size())
    {
        m_dimensions = Vector2i(m_gridsize.x*6,m_gridsize.y*(m_leftSidePinList.size()*2));
        if(m_leftSidePinList.size() > 0)
            pinSpacing = m_dimensions.y / (m_leftSidePinList.size());


    }
    else
    {
        m_dimensions = Vector2i(m_gridsize.x*6,m_gridsize.y*(m_rightSidePinList.size()*2));
        if(m_rightSidePinList.size() > 0)
            pinSpacing = m_dimensions.y / (m_rightSidePinList.size());

    }
    //sf::Transform rotationTransform;
    //qDebug() << "dimension1: "<<m_dimensions.x<<" "<<m_dimensions.y;
    //rotationTransform.rotate(m_angle);


    for(size_t i=0; i<m_leftSidePinList.size(); i++)
    {
        Vector2i pos(-m_dimensions.x/2,
                     - ((m_leftSidePinList.size()-1)*pinSpacing)/2 + pinSpacing*i);
        for(int j=0; j<4-m_angle/90; j++)
        {
            //pos = Vector2i(rotationTransform.transformPoint(Vector2f(pos)));
            int tmp = pos.x;
            pos.x = pos.y;
            pos.y = -tmp;
        }
        //pos = Vector2i(rotationTransform.transformPoint(Vector2f(pos)));
        m_leftSidePinList[i]->setPos(pos);
        m_leftSidePinList[i]->angle(m_angle+180);
        m_inputValues.push_back(0);
    }
    for(size_t i=0; i<m_rightSidePinList.size(); i++)
    {
        Vector2i pos(m_dimensions.x/2,
                     - ((m_rightSidePinList.size()-1)*pinSpacing)/2 + pinSpacing*i);

        for(int j=0; j<4-m_angle/90; j++)
        {
            //pos = Vector2i(rotationTransform.transformPoint(Vector2f(pos)));
            int tmp = pos.x;
            pos.x = pos.y;
            pos.y = -tmp;
        }

        m_rightSidePinList[i]->setPos(pos);
        m_rightSidePinList[i]->angle(m_angle);
        m_outputValues.push_back(0);
    }

    if((m_angle+90) % 180 == 0)
    {
        //pos = Vector2i(rotationTransform.transformPoint(Vector2f(pos)));
        int tmp = m_dimensions.x;
        m_dimensions.x = m_dimensions.y;
        m_dimensions.y = -tmp;
    }
    //m_dimensions = Vector2i(rotationTransform.transformPoint(Vector2f(m_dimensions*10)))/10;
    if(m_dimensions.x<0)m_dimensions.x*=-1;
    if(m_dimensions.y<0)m_dimensions.y*=-1;

    //qDebug() << "dimension2: "<<m_dimensions.x<<" "<<m_dimensions.y;
    m_boundingBox.set(Vector2i(m_position.x-m_dimensions.x/2,m_position.y-m_dimensions.y/2),
                      m_position + m_dimensions);




}

const vector<Pin*> &Gate::getLeftPins() const
{
    return m_leftSidePinList;
}
const vector<Pin*> &Gate::getRightPins() const
{
    return m_rightSidePinList;
}
