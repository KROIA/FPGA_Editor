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
        enum Type
        {
            input,
            output,
            inOut,
            tristate
        };

        Pin();
        Pin(Type type,int angle);
        ~Pin();

        void label(const string &label);
        const string &label() const;
        void voltage(float voltage);
        float voltage() const;

        Vector2i connectionPoint();
        Type type() const;

        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));

        void addToBlacklist(Pin *pin);
        void addToBlacklist(const vector<Pin *> &pinList);
        void removeFromBlacklist(Pin *pin);
        void removeFromBlacklist(const vector<Pin *> &pinList);

        void addToBlacklist(Type type);
        void addToBlacklist(const vector<Type > typeList);
        void removeFromBlacklist(Type type);
        void removeFromBlacklist(const vector<Type > typeList);

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


        Type m_type;
        Vector2i m_connectionPoint;
        string m_label;
        float m_voltage;


        bool m_startConnecting;
        static Pin *m_tmpIsConnectingToPin;
        vector<Pin *> m_connectedToList;
        vector<Pin *> m_pinConnectionBlackList; // Pins, which can't be connected to
        vector<Type>  m_pinConnectionTypeBlacklist;
        vector<Connection* > m_connectionList;


};
#endif // PIN_H
