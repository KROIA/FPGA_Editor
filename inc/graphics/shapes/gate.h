#ifndef GATE_H
#define GATE_H

#include "shape.h"
#include "pin.h"



class Gate  :   public Shape
{
    Q_OBJECT

    public:
        const static sf::Keyboard::Key key_shiftCopy;

        /*struct PinSnapPoint
        {
            Vector2i pos;
            int angle;
            Pin *pin;
        };*/
        struct Connection_Def
        {
            size_t beginGateID;
            string beginPin;

            size_t endGateID;
            string endPin;
        };
        Gate();
        Gate(int inputs, int outputs);
        Gate(Vector2i pos,int inputs, int outputs);
        Gate(const Gate &other);
        ~Gate();

        virtual Gate *clone() const;


        void name(const string &name);
        string name() const;

        void addPin(const Pin& pin);
        void addPin(const vector<Pin> &pinList);
        Pin* getPin(const string &name) const;
        Pin* getLeftSidePin(const string &name) const;
        Pin* getLeftSidePin(size_t index) const;
        Pin* getRightSidePin(const string &name) const;
        Pin* getRightSidePin(size_t index) const;

        size_t getLeftSidePinCount() const;
        size_t getRightSidePinCount() const;

        void removePin(const string &name);
        void removeLastLeftPin();
        void removeLastRightPin();
        void removeAllLeftPins();
        void removeAllRightPins();
        void removeAllPins();

        void createConnection(const string &thisPinName,
                              Gate *other,const string &otherPinName);
        void createConnection(const string &thisPinName,Pin *otherPin);
        void createConnection(Pin *thisPin,Pin *otherPin);


        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));
        virtual void drawDebug(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));


        void inputVoltage(size_t index, float voltage);
        float inputVoltage(size_t index);

        void outputVoltage(size_t index, float voltage);
        float outputVoltage(size_t index);

        size_t leftSidePinCount() const;
        size_t rightSidePinCount() const;

        void snapToMouse(bool enable);
        void deleteOnEscape(bool enable);
        void rotate();
        void rotate(int angle);
        void setRotation(int angle);


        virtual void updateEvents(vector<sf::Event> *events,
                                  bool mouseDoubleClickEvent,
                                  sf::RenderWindow *window);
        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);
        virtual void onDoubleKlick();

        virtual void utilityUpdate();
        virtual void readInputs();
        virtual void processLogic();
        virtual void setOutputs();

        vector<Connection_Def> getConnectionsDef();


    signals:
        void startsMoving(Gate *gate);
        void placed(Gate *gate);
        void clicked(Gate *gate);
        void addCopyOf(Gate *gate);
        void deleteRequest(Gate *gate);

    private slots:
        void onConnectionCreate(Pin *pin,Connection *con);
        void onConnectionRemove(Pin *pin,Connection *con);

    protected:
        // From ToolListener
        virtual void toolChanged(Tool *oldTool, Tool *newTool);
        virtual void toolCleared(Tool *oldTool);




        void setupPins(int inputs, int outputs);
        void updateGate();

        const vector<Pin*> &getLeftPins() const;
        const vector<Pin*> &getRightPins() const;






        Vector2i m_dimensions;


        sf::Text m_label;
        static sf::Font *m_font;
        //sf::Transform m_transform;

        bool m_moving;
        Vector2i m_movingOldPos;
        int m_angle;
        bool m_isCopying;

    private:
        void internalAddPin(const Pin &pin);
        void setup();
        void setup(int inputs, int outputs);

        vector<Pin*> m_leftSidePinList;
        vector<Pin*> m_rightSidePinList;
        vector<Connection *> m_connectionList;

        //vector<PinSnapPoint> m_pinSnapPointList;

        vector<float> m_inputValues;
        vector<float> m_outputValues;

        //static vector<Gate *> m_globalGatelIst;

};
#endif // GATE_H
