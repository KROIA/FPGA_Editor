#ifndef PIN_H
#define PIN_H

#include "shape.h"

#include "connection.h"
#include "classdef.h"

#include "vector"
using std::vector;
using std::string;


class Pin   :   public Shape
{
    public:
        enum Direction
        {
            input,
            output,
            inout
        };

        Pin();
        Pin(const Pin &other);
        Pin(Direction direction,int angle = 0);
        ~Pin();

        void angle(int angle);
        int angle() const;

        void name(const string &label);
        string name() const;
        void voltage(float voltage);
        float voltage() const;

        Vector2i connectionPoint();
        Direction direction() const;

        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));

        void addToBlacklist(Pin *pin);
        void addToBlacklist(const vector<Pin *> &pinList);
        void removeFromBlacklist(Pin *pin);
        void removeFromBlacklist(const vector<Pin *> &pinList);

        void addToBlacklist(Direction direction);
        void addToBlacklist(const vector<Direction> directionList);
        void removeFromBlacklist(Direction direction);
        void removeFromBlacklist(const vector<Direction> directionList);

    protected:
        // From ToolListener
        virtual void toolChanged(Tool *oldTool, Tool *newTool);
        virtual void toolCleared(Tool *oldTool);

        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);


    private:

        void createConnection(Pin *other);
        void deleteConnection(Pin *other);
        Connection *getConnection(Pin *other);
        void setConnectionColorToDefault();
        void setConnectionColorToDefault(size_t index);
        void setConnectionColor(Color color);
        void setConnectionColor(size_t index, Color color);


        Vector2i m_pinDimensions;
        unsigned int m_angle;

        Color m_color;
        Color m_defaultColor;
        static Color m_selectedColor;


        Direction m_direction;
        Vector2i m_connectionPoint;
        sf::Text m_label;
        static sf::Font *m_font;
        float m_voltage;


        bool m_startConnecting;
        static Pin *m_tmpIsConnectingToPin;
        vector<Pin *> m_connectedToList;
        vector<Pin *> m_pinConnectionBlackList; // Pins, which can't be connected to
        vector<Direction>  m_pinConnectionTypeBlacklist;
        vector<Connection* > m_connectionList;


};
#endif // PIN_H
