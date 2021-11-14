#ifndef MODULE_H
#define MODULE_H

#include "gate.h"
#include "ioPin.h"



class Module    :   public Gate
{
        Q_OBJECT
    private:
        struct IO_Def{
            string name;
            Pin::Direction dir;
        };
    public:
        struct IO{
            IoPin *insidePin;
            Pin   *outsidePin;
        };

        Module();
        Module(const Module &other);
        ~Module();

        void addIO(const string &name, Pin::Direction direction);

        void addGate(const Gate &gate);
        void addGate(Gate *gate);
        void deleteGate(Gate *gate);

        void setViewInside(bool inside);

        // Processing
        virtual void utilityUpdate();
        virtual void readInputs();
        virtual void processLogic();
        virtual void setOutputs();


        virtual void updateEvents(vector<sf::Event> *events,
                                  bool mouseDoubleClickEvent,
                                  sf::RenderWindow *window);
        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));
        virtual void drawDebug(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));



    signals:
        void insideViewEnter(Module *module);
        void insideViewExit(Module *module);

    private slots:
        void onGateDeleteRequest(Gate *gate);
        void onIODeleteRequest(Gate *gate);

    protected:
        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);
        virtual void onDoubleKlick();


    private:

        bool m_viewInside;
        //bool m_isVisible;
        vector<Gate *> m_gateList;
        vector<Gate *> m_gateAddList;
        vector<Gate *> m_gateDeleteList;

        vector<IO> m_ioLeft;
        vector<IO> m_ioRight;
        vector<IO_Def> m_ioToAdd;
        vector<Gate *> m_ioToDelete;

};

#endif // MODULE_H
