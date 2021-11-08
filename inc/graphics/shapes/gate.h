#ifndef GATE_H
#define GATE_H

#include "shape.h"
#include "pin.h"


class Gate  :   public Shape
{
    public:
        Gate();
        Gate(Vector2i pos);
        ~Gate();

        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));

    protected:
        // From ToolListener
        virtual void toolChanged(Tool *oldTool, Tool *newTool);
        virtual void toolCleared(Tool *oldTool);

        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);


        void setupPins(int inputs, int outputs);

    private:
        void setup();

        vector<Pin*> m_inputPins;
        vector<Pin*> m_outputPins;

        Vector2i m_dimensions;

        sf::Text m_label;
        static sf::Font *m_font;

        bool m_moving;
        Vector2i m_movingOldPos;

};
#endif // GATE_H
