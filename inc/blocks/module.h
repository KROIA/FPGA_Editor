#ifndef MODULE_H
#define MODULE_H

#include "gate.h"
#include "ioPin.h"
#include "logicGate.h"
#include "unordered_map"



class Module    :   public Gate
{
        Q_OBJECT        
    public:
        struct IO_Def{
            string name;
            Pin::Direction dir;
            Vector2i pos;
        };


        struct ModuleGate_Def
        {
            string name;
            size_t gateID;
            Vector2i pos;
            vector<Connection_Def> connections;
        };

        struct ModuleData
        {
            string name;
            vector<IO_Def> ioDefList;


            vector<LogicGate::LogicGate_Def> logicGates;
            vector<ModuleGate_Def> moduleGates;
        };
        struct IO{
            IoPin *insidePin;
            Pin   *outsidePin;
        };


        Module();
        Module(const Module &other);

        ~Module();

        virtual Module *clone();
        Module *createLibBlock(ModuleData data);
        void isLibBlock(bool enable);

        void addIO(const string &name, Pin::Direction direction);

        void addGate(const Gate &gate);
        void addGate(Gate *gate);
        void addGate(Module &gate);
        void addGate(Module *gate);
        void addGate(const LogicGate &gate);
        void addGate(LogicGate *gate);
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

        ModuleData getModuleDef();
        ModuleGate_Def getModuleGateDef();


    signals:

        void insideViewEnter(Module *module);
        void insideViewExit(Module *module);

    public slots:


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
        vector<Gate* > m_allGatesList;

        vector<Gate *> m_gateList;
        vector<LogicGate*> m_logicGateList;
        vector<Module*> m_moduleGateList;

        vector<Gate *> m_gateAddList;
        vector<LogicGate* > m_logicGateAddList;
        vector<Module*> m_moduleGateAddList;

        vector<Gate *> m_gateDeleteList;

        vector<IO> m_ioLeft;
        vector<IO> m_ioRight;
        vector<IO_Def> m_ioToAdd;
        vector<Gate *> m_ioToDelete;

        Module *m_libDefLink;// ptr to the Lib Module

        bool m_isLibDef;
        ModuleData m_moduleDef;
        vector<Module *> m_cloneModuleList;
        static std::unordered_map<size_t,Module*> m_global_moduleLibMap;
};

#endif // MODULE_H
