#ifndef CONNECTION_H
#define CONNECTION_H

#include "debug.h"
#include "shape.h"
#include "pin.h"
#include <vector>
#include "classdef.h"

using std::vector;

class Connection    : public Shape
{
    public:
        Connection();
        ~Connection();

        void startPin(Pin *pin);
        void endPin(Pin *pin);

        Pin *startPin();
        Pin *endPin();

        void setColorToDefault();
        void setColor(Color color);


        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));
        virtual void drawDebug(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));

    protected:

    private:
        Color m_color;
        Color m_standardColor;
        Color m_selectedColor;


        Pin *m_startPin;
        Pin *m_endPin;
};
#endif // CONNECTION_H
