#include "pin.h"

Pin *Pin::m_tmpIsConnectingToPin = nullptr;

Color Pin::m_selectedColor    = Color(0,200,0);

Pin::Pin()
{
    Pin(Type::input,0);
}
Pin::Pin(Type type,int angle)
{
    m_type  = type;

    switch(m_type)
    {
        case output:
        {
            addToBlacklist(output); // Connecting Outputs together is illegal
            break;
        }
        default:
            break;
    }

    m_angle = angle;
    m_pinDimensions = Vector2i(20,6);
    m_defaultColor = Color(150,150,150);
    m_color = m_defaultColor;
    m_useGrid = false;
    m_startConnecting = false;
    m_voltage = 0;
    m_label = "Pin";
}
Pin::~Pin()
{

}
void Pin::label(const string &label)
{
    m_label = label;
}
const string &Pin::label() const
{
    return m_label;
}
void Pin::voltage(float voltage)
{
    m_voltage = voltage;
}
float Pin::voltage() const
{
    return m_voltage;
}
Vector2i Pin::connectionPoint()
{
    switch(m_angle)
    {
        case 0:
        {
            m_connectionPoint = m_position + Vector2i(m_pinDimensions.x,0);
            break;
        }
        case 90:
        {
            m_connectionPoint = m_position + Vector2i(0,m_pinDimensions.x);
            break;
        }
        case 180:
        {
            m_connectionPoint = m_position - Vector2i(m_pinDimensions.x,0);
            break;
        }
        case 270:
        {
            m_connectionPoint = m_position - Vector2i(0,m_pinDimensions.x);
            break;
        }
    }


    if(m_parent != nullptr)
    {
        m_connectionPoint += m_parent->getPos();
    }
    return m_connectionPoint;
}
Pin::Type Pin::type() const
{
    return m_type;
}
void Pin::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
    {
        m_isVisible = m_parent->isVisible();
        drawPos += m_parent->getPos();
    }
    if(!m_isVisible)
        return;
    drawPos += m_position;
    switch(m_angle)
    {
        case 0:
        {
            m_boundingBox.set(Vector2i(drawPos.x,drawPos.y-m_pinDimensions.y/2),
                              Vector2i(drawPos.x+m_pinDimensions.x,drawPos.y+m_pinDimensions.y/2));
            break;
        }
        case 90:
        {
            m_boundingBox.set(Vector2i(drawPos.x-m_pinDimensions.y/2,drawPos.y),
                              Vector2i(drawPos.x+m_pinDimensions.y/2,drawPos.y+m_pinDimensions.x));
            break;
        }
        case 180:
        {
            m_boundingBox.set(Vector2i(drawPos.x-m_pinDimensions.x,drawPos.y-m_pinDimensions.y/2),
                              Vector2i(drawPos.x,drawPos.y+m_pinDimensions.y/2));
            break;
        }
        case 270:
        {
            m_boundingBox.set(Vector2i(drawPos.x-m_pinDimensions.y/2,drawPos.y-m_pinDimensions.x),
                              Vector2i(drawPos.x+m_pinDimensions.y/2,drawPos.y));
            break;
        }
    }

    sf::RectangleShape line((Vector2f(m_pinDimensions)));
    line.setOrigin(Vector2f(0,m_pinDimensions.y/2));
    line.rotate(m_angle);

    if(Physics::displayPhysical)
    {
        if(m_type != Type::output) // Update the voltage
        {
            for(size_t i=0; i<m_connectedToList.size(); i++)
            {
                if(m_connectedToList[i]->m_type == Type::output)
                {
                    m_voltage = m_connectedToList[i]->m_voltage;
                }
            }
        }
        line.setFillColor(Physics::coloredVoltage(m_voltage));
    }
    else
        line.setFillColor(m_color);

    line.setPosition((Vector2f(drawPos)));

    window->draw(line);
}
void Pin::addToBlacklist(Pin *pin)
{
    for(size_t i=0; i<m_pinConnectionBlackList.size(); i++)
    {
        if(m_pinConnectionBlackList[i] == pin)
        {
            qDebug() << "WARNING: Pin::addToBlacklist(Pin *pin): This pin is already in the blacklist";
            return;
        }
    }
    m_pinConnectionBlackList.push_back(pin);
}
void Pin::addToBlacklist(const vector<Pin *> &pinList)
{
    for(size_t i=0; i<pinList.size(); i++)
        addToBlacklist(pinList[i]);
}
void Pin::removeFromBlacklist(Pin *pin)
{
    for(size_t i=0; i<m_pinConnectionBlackList.size(); i++)
    {
        if(m_pinConnectionBlackList[i] == pin)
        {
            m_pinConnectionBlackList.erase(m_pinConnectionBlackList.begin() + i);
            return;
        }
    }
    qDebug() << "WARNING: Pin::removeFromBlacklist(Pin *pin): Pin is not blacklisted";
}
void Pin::removeFromBlacklist(const vector<Pin *> &pinList)
{
    for(size_t i=0; i<pinList.size(); i++)
        removeFromBlacklist(pinList[i]);
}

void Pin::addToBlacklist(Type type)
{
    for(size_t i=0; i<m_pinConnectionTypeBlacklist.size(); i++)
    {
        if(m_pinConnectionTypeBlacklist[i] == type)
        {
            qDebug() << "WARNING: Pin::addToBlacklist(Type type): This type is already in the blacklist";
            return;
        }
    }
    m_pinConnectionTypeBlacklist.push_back(type);
}
void Pin::addToBlacklist(const vector<Type > typeList)
{
    for(size_t i=0; i<typeList.size(); i++)
        addToBlacklist(typeList[i]);
}
void Pin::removeFromBlacklist(Type type)
{
    for(size_t i=0; i<m_pinConnectionTypeBlacklist.size(); i++)
    {
        if(m_pinConnectionTypeBlacklist[i] == type)
        {
            m_pinConnectionTypeBlacklist.erase(m_pinConnectionTypeBlacklist.begin() + i);
            return;
        }
    }
    qDebug() << "WARNING: Pin::removeFromBlacklist(Type type): Type is not blacklisted";
}
void Pin::removeFromBlacklist(const vector<Type > typeList)
{
    for(size_t i=0; i<typeList.size(); i++)
        removeFromBlacklist(typeList[i]);
}

void Pin::onEventUpdate(sf::RenderWindow *window)
{

}
void Pin::onMouseOver(Vector2i mousePos)
{

}
void Pin::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    if(Tool::getSelectedType() == Tool::Type::signalConnector)
    {
        if(!Tool::getSelected().isUsed())
        {
            Tool::getSelected().isUsed(true);
            Tool::startListen(this);
            m_startConnecting = true;
            m_tmpIsConnectingToPin = this;
            m_color = m_selectedColor;
        }
        else
        {



            if(m_tmpIsConnectingToPin != this &&
               m_tmpIsConnectingToPin != nullptr)
            {
                Tool::getSelected().isUsed(false);
                Tool::endListen(this);
                createConnection(m_tmpIsConnectingToPin);
                m_tmpIsConnectingToPin->m_color = m_tmpIsConnectingToPin->m_defaultColor;
                m_color = m_defaultColor;
                m_tmpIsConnectingToPin = nullptr;
            }
        }
    }else if(Tool::getSelectedType() == Tool::Type::signalRemover)
    {
        if(m_connectedToList.size() != 0)
        {
            if(!Tool::getSelected().isUsed())
            {


                if(m_connectedToList.size() == 1)
                {
                    deleteConnection(m_connectedToList[0]);
                }
                else
                {
                    Tool::getSelected().isUsed(true);
                    Tool::startListen(this);
                    m_color = m_selectedColor;
                    m_tmpIsConnectingToPin = this;
                    setConnectionColor(m_selectedColor);
                }
            }
            else
            {
                if(m_tmpIsConnectingToPin != this &&
                   m_tmpIsConnectingToPin != nullptr)
                {
                    m_color = m_defaultColor;
                    m_tmpIsConnectingToPin->m_color = m_tmpIsConnectingToPin->m_defaultColor;
                    deleteConnection(m_tmpIsConnectingToPin);
                    m_tmpIsConnectingToPin->setConnectionColorToDefault();
                    m_tmpIsConnectingToPin = nullptr;
                    Tool::getSelected().isUsed(false);
                    Tool::endListen(this);
                }


            }
        }
    }
}
void Pin::createConnection(Pin *other)
{
    if(other == nullptr)
    {
        qDebug() << "ERROR: Pin::createConnection(Pin *other): other = nullptr";
        return;
    }
    if(other == this)
    {
        qDebug() << "ERROR: Pin::createConnection(Pin *other): other = this Pin, Can't connect to it self";
        return;
    }
    for(size_t i=0; i<m_connectedToList.size(); i++)
    {
        if(m_connectedToList[i] == other)
        {
            qDebug() << "WARNING: Pin::createConnection(Pin *other): already connected to this Pin";
            return;
        }
    }

    for(size_t i=0; i<m_pinConnectionTypeBlacklist.size(); i++)
    {
        if(m_pinConnectionTypeBlacklist[i] == other->m_type)
        {
            qDebug() << "ERROR: Pin::createConnection(Pin *other): This connection is not allowed";
            return;
        }
    }
    for(size_t i=0; i<m_pinConnectionBlackList.size(); i++)
    {
        if(m_pinConnectionBlackList[i] == other)
        {
            qDebug() << "ERROR: Pin::createConnection(Pin *other): This connection is not allowed";
            return;
        }
    }
    for(size_t i=0; i<other->m_pinConnectionBlackList.size(); i++)
    {
        if(other->m_pinConnectionBlackList[i] == this)
        {
            qDebug() << "ERROR: Pin::createConnection(Pin *other): This connection is not allowed";
            return;
        }
    }

    Connection *connection = new Connection();
    connection->startPin(this);
    connection->endPin(other);


    m_connectedToList.push_back(other);
    other->m_connectedToList.push_back(this);
    m_connectionList.push_back(connection);
    other->m_connectionList.push_back(connection);
}
void Pin::deleteConnection(Pin *other)
{
    for(size_t i=0; i<m_connectedToList.size(); i++)
    {
        if(m_connectedToList[i] == other)
        {
            m_connectedToList.erase(m_connectedToList.begin() + i);
            delete m_connectionList[i];
            m_connectionList.erase(m_connectionList.begin() + i);

            for(size_t j=0; j<other->m_connectedToList.size(); j++)
            {
                if(other->m_connectedToList[j] == this)
                {
                    other->m_connectedToList.erase(other->m_connectedToList.begin() + j);
                    other->m_connectionList.erase(other->m_connectionList.begin() + j);
                    return;
                }
            }
            qDebug() << "ERROR: Pin::deleteConnection(Pin *other): Pin connection only partly removed";
        }
    }
    qDebug() << "WARNING: Pin::deleteConnection(Pin *other): Pin connection does not exist";
}
Connection *Pin::getConnection(Pin *other)
{
    for(size_t i=0; i<m_connectedToList.size(); i++)
    {
        if(m_connectedToList[i] == other)
        {
            return m_connectionList[i];
        }
    }
    return nullptr;
}
void Pin::setConnectionColorToDefault()
{
    for(size_t i=0; i<m_connectionList.size(); i++)
        setConnectionColorToDefault(i);
}
void Pin::setConnectionColorToDefault(size_t index)
{
    m_connectionList[index]->setColorToDefault();
}
void Pin::setConnectionColor(Color color)
{
    for(size_t i=0; i<m_connectionList.size(); i++)
        setConnectionColor(i,color);
}
void Pin::setConnectionColor(size_t index, Color color)
{
    m_connectionList[index]->setColor(color);
}

void Pin::toolChanged(Tool *oldTool, Tool *newTool)
{
    toolCleared(oldTool);
}
void Pin::toolCleared(Tool *oldTool)
{
    m_tmpIsConnectingToPin = nullptr;
    m_color = m_defaultColor;
    setConnectionColorToDefault();
    Tool::endListen(this);
}
