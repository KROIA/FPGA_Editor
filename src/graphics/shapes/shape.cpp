#include "shape.h"


vector<Shape*> Shape::global_shapeList;
Vector2i Shape::m_gridsize = Vector2i(10,10);

Shape::Shape()
{
    m_useGrid = true;
    m_parent = nullptr;
    setPos(Vector2i(0,0));
    global_shapeList.reserve(global_shapeList.size()*2);
    global_shapeList.push_back(this);
    m_isVisible = true;
}
Shape::Shape(Shape *parent)
{
    m_useGrid = true;
    m_parent = parent;
    setPos(Vector2i(0,0));
    global_shapeList.reserve(global_shapeList.size()*2);
    global_shapeList.push_back(this);
    m_isVisible = true;
}
Shape::Shape(const Shape &other)
{
    m_parent = other.m_parent;
    m_position = other.m_position;
    m_boundingBox = other.m_boundingBox;
    m_isVisible = other.m_isVisible;
}
Shape::~Shape()
{
    for(size_t i=0; i<global_shapeList.size(); i++)
    {
        if(global_shapeList[i] == this)
        {
            global_shapeList.erase(global_shapeList.begin() + i);
            return;
        }
    }
    Tool::endListen(this);
}

void Shape::setParent(Shape *parent)
{
    m_parent = parent;
}

void Shape::setPos(Vector2i pos)
{
    m_position = getGridSnaped(pos);
}
Vector2i Shape::getPos() const
{
    return m_position;
}

void Shape::setVisibility(bool visible)
{
    m_isVisible = visible;
}
bool Shape::isVisible() const
{
    return m_isVisible;
}

void Shape::draw(sf::RenderWindow *window, Vector2i drawPos)
{

}
void Shape::drawDebug(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
        m_isVisible = m_parent->m_isVisible;

    if(!m_isVisible)
        return;

    m_boundingBox.draw(window,Color(255,0,0));
}

void Shape::updateEvents(vector<sf::Event> *events,
                         sf::RenderWindow *window/*,
                         Tool *selectedTool*/)
{


    for(size_t i=0; i<events->size(); i++)
    {
        sf::Event *e = &(*events)[i];
        switch(e->type)
        {
            case sf::Event::MouseButtonPressed:
            {
                Vector2i pos(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
                //Vector2i pos(e->mouseButton.x,e->mouseButton.y);
                if(m_boundingBox.contains(pos))
                {
                    onKlick(e->mouseButton.button,pos);
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                Vector2i pos(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
                //Vector2i pos(e->mouseMove.x,e->mouseMove.y);
                if(m_boundingBox.contains(pos))
                {
                    onMouseOver(pos);
                }
                break;
            }

            default:
                break;
        }
    }
    onEventUpdate(window);

    /*for(int i=0; i<sf::Mouse::Button::ButtonCount; i++)
        if(sf::Mouse::isButtonPressed((sf::Mouse::Button)i))
        {
            onKlick
        }*/

}
void Shape::toolChanged(Tool *oldTool, Tool *newTool)
{

}
void Shape::toolCleared(Tool *oldTool)
{

}
void Shape::onEventUpdate(sf::RenderWindow *window)
{

}
void Shape::onMouseOver(Vector2i mousePos)
{

}
void Shape::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{

}

void Shape::global_updateEvents(vector<sf::Event> *events,
                                sf::RenderWindow *window/*,
                                Tool *selectedTool*/)
{
    for(size_t i=0; i<global_shapeList.size(); i++)
    {
        global_shapeList[i]->updateEvents(events,window/*,selectedTool*/);
    }
}
void Shape::global_draw(sf::RenderWindow *window)
{
    for(size_t i=0; i<global_shapeList.size(); i++)
    {
        global_shapeList[i]->draw(window);
    }
}
void Shape::global_drawDebug(sf::RenderWindow *window)
{
    for(size_t i=0; i<global_shapeList.size(); i++)
    {
        global_shapeList[i]->drawDebug(window);
    }
}

Vector2i Shape::getGridSnaped(Vector2i pos)
{
    if(!m_useGrid)
        return pos;
    pos.x = m_gridsize.x * (pos.x/m_gridsize.x);
    pos.y = m_gridsize.y * (pos.y/m_gridsize.y);
    return pos;
}
