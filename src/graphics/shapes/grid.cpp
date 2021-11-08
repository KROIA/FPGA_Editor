#include "grid.h"


Grid::Grid()
{
    m_boundingBox.set(Vector2i(-100,-100),Vector2i(100,100));
    m_boundingBox.set(Vector2i(0,0),Vector2i(1000,1000));
    int gray = 100;
    m_color = Color(gray,gray,gray);
}

void Grid::setColor(Color &color)
{
    m_color = color;
}
void Grid::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    size_t width = m_boundingBox.width()/m_gridsize.x;
    size_t height = m_boundingBox.height()/m_gridsize.y;

    Vector2i begin(m_boundingBox.begin());
    Vector2i end(0,m_boundingBox.height());
    sf::Vertex lineX[width*2];

    for(size_t i=0; i<width*2; i+=2)
    {
        lineX[i]    = sf::Vertex(sf::Vector2f(begin),m_color);
        lineX[i+1]  = sf::Vertex(sf::Vector2f(begin+end),m_color);
        begin.x+= m_gridsize.x;
    };
    window->draw(lineX,width*2,sf::Lines);

    begin = Vector2i(m_boundingBox.begin());
    end = Vector2i(m_boundingBox.width(),0);
    sf::Vertex lineY[height*2];

    for(size_t i=0; i<height*2; i+=2)
    {
        lineY[i]    = sf::Vertex(sf::Vector2f(begin),m_color);
        lineY[i+1]  = sf::Vertex(sf::Vector2f(begin+end),m_color);
        begin.y+= m_gridsize.y;
    };
    window->draw(lineY,height*2,sf::Lines);
}

void Grid::onEventUpdate(sf::RenderWindow *window)
{

}
void Grid::onMouseOver(Vector2i mousePos)
{

}
void Grid::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{

}
