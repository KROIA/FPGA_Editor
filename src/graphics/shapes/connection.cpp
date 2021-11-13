#include "connection.h"


Connection::Connection()
{
    m_standardColor     = Color(50,50,50);
    m_selectedColor     = Color(0,200,0);
    m_color = m_standardColor;
    m_voltage = 0;
}
Connection::~Connection()
{
}




void Connection::startPin(Pin *pin)
{
    m_startPin = pin;
}
void Connection::endPin(Pin *pin)
{
    m_endPin = pin;
}

Pin *Connection::startPin()
{
    return m_startPin;
}
Pin *Connection::endPin()
{
    return m_endPin;
}
void Connection::setColorToDefault()
{
    m_color = m_standardColor;
}
void Connection::setColor(Color color)
{
    m_color = color;
}

void Connection::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    if(!m_isVisible || m_startPin == nullptr || m_endPin == nullptr)
        return;
    Color drawColor = m_color;
    if(Physics::displayPhysical)
    {
        if(m_startPin->direction() == Pin::Direction::output)
            m_voltage = m_startPin->voltage();
        else if(m_endPin->direction() == Pin::Direction::output)
            m_voltage = m_endPin->voltage();
        drawColor = Physics::coloredVoltage(m_voltage);
    }

    sf::Vertex line[2] =
    {
        sf::Vertex(sf::Vector2f(m_startPin->connectionPoint()+drawPos),m_color),
        sf::Vertex(sf::Vector2f(m_endPin->connectionPoint()+drawPos),m_color),
    };
    window->draw(line,2,sf::LineStrip);
}
void Connection::drawDebug(sf::RenderWindow *window, Vector2i drawPos)
{

}
