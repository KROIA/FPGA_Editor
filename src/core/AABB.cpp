#include "AABB.h"

AABB::AABB()
{

}
AABB::AABB(const AABB &other)
{
    m_begin = other.m_begin;
    m_end = other.m_end;
}
AABB::AABB(Vector2i begin, Vector2i end)
{
    set(begin,end);
}

AABB::~AABB()
{

}


const AABB &AABB::operator=(const AABB &other)
{
    m_begin = other.m_begin;
    m_end = other.m_end;
    return *this;
}
void AABB::set(Vector2i begin, Vector2i end)
{
    m_begin = begin;
    m_end = end;
    Vector2i div = end - begin;
    if(div.x<0)
    {
        m_begin.x = end.x;
        m_end.x = begin.x;
    }
    if(div.y<0)
    {
        m_begin.y = end.y;
        m_end.y = begin.y;
    }


}

void AABB::begin(Vector2i begin)
{
    Vector2i div = m_end - begin;
    if(div.x < 0 || div.y < 0)
    {
        Vector2i lastEnd = m_end;
        if(div.x < 0)
        {
            m_end.x = begin.x;
            m_begin.x = lastEnd.x;

        }
        if(div.y < 0)
        {
            m_end.y = begin.y;
            m_begin.y = lastEnd.y;
        }
        return;
    }
    m_begin = begin;
}

void AABB::end(Vector2i end)
{
    Vector2i div = end - m_begin;
    if(div.x < 0 || div.y < 0)
    {
        Vector2i lastBegin = m_begin;
        if(div.x < 0)
        {
            m_begin.x = end.x;
            m_end.x = lastBegin.x;
        }
        if(div.y < 0)
        {
            m_begin.y = end.y;
            m_end.y = lastBegin.y;
        }
        return;
    }
    m_end   = end;
}
void AABB::move(Vector2i delta)
{
    m_begin += delta;
    m_end   += delta;
}

Vector2i AABB::begin() const
{
    return m_begin;
}

Vector2i AABB::end() const
{
    return m_end;
}
Vector2i AABB::middle() const
{
    return Vector2i((m_end.x+m_begin.x)/2,(m_end.y+m_begin.y)/2);
}


int AABB::width() const
{
    return m_end.x - m_begin.x;
}

int AABB::height() const
{
    return m_end.y - m_begin.y;
}


bool AABB::overlaps(const AABB &other)
{
    return m_end.x > other.m_begin.x &&
           m_begin.x < other.m_end.x &&
           m_end.y > other.m_begin.y &&
           m_begin.y < other.m_end.y;
}

bool AABB::contains(const AABB &other)
{
    return m_begin.x < other.m_begin.x &&
           m_end.x   > other.m_end.x &&
           m_begin.y < other.m_begin.y &&
           m_end.y   > other.m_end.y;
}

bool AABB::contains(const Vector2i point)
{
    return m_begin.x < point.x &&
           m_end.x   > point.x &&
           m_begin.y < point.y &&
           m_end.y   > point.y;
}

void AABB::draw(sf::RenderWindow *window, Color color)
{
    sf::Vertex line[5] =
    {
        sf::Vertex(sf::Vector2f(m_begin),color),
        sf::Vertex(sf::Vector2f((float)m_end.x,(float)m_begin.y),color),
        sf::Vertex(sf::Vector2f(m_end),color),
        sf::Vertex(sf::Vector2f((float)m_begin.x,(float)m_end.y),color),
        sf::Vertex(sf::Vector2f(m_begin),color)
    };
    window->draw(line,5,sf::LineStrip);
}
