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
    Vector2f delta(m_endPin->connectionPoint()-m_startPin->connectionPoint());
    float angle;
    float lengt = sqrt(delta.x*delta.x + delta.y*delta.y);
    if(delta.x != 0)
    {
        angle = atan(delta.y/delta.x)*180/M_PI;
        if(delta.x < 0)
            angle = 180+angle;
    }
    else
        angle = delta.y > 0 ? 90 : 270;

    //qDebug() << "angle: "<<angle << " length: "<<lengt;

    sf::RectangleShape line((Vector2f(lengt,2)));
    line.setOrigin(Vector2f(0,1));
    line.rotate(angle);

    if(Physics::displayPhysical)
    {

        line.setFillColor(Physics::coloredVoltage(m_voltage));
    }
    else
        line.setFillColor(drawColor);

    line.setPosition((Vector2f(m_startPin->connectionPoint()+drawPos)));

    /*sf::Vertex line[2] =
    {
        sf::Vertex(sf::Vector2f(m_startPin->connectionPoint()+drawPos),drawColor),
        sf::Vertex(sf::Vector2f(m_endPin->connectionPoint()+drawPos),drawColor),
    };
    window->draw(line,2,sf::LineStrip);*/
    window->draw(line);
}
void Connection::drawDebug(sf::RenderWindow *window, Vector2i drawPos)
{

}
