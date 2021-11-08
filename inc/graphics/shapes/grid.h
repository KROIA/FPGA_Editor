#ifndef GRID_H
#define GRID_H

#include "shape.h"

class Grid  : public Shape
{
    public:
        Grid();

        void setColor(Color &color);
        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));


    private:

        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);

        Color m_color;

};
#endif // GRID_H
