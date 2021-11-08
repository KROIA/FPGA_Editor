#ifndef TOOL_H
#define TOOL_H

#include "classdef.h"
#include "toollistener.h"
#include "vector"

using std::vector;

class Tool
{
    public:
        enum Type{
            none,
            moveModifier,
            signalConnector,
            signalRemover,
        };
        Tool();
        Tool(Type type);
        Tool(const Tool &other);
        ~Tool();

        const Tool &operator=(const Tool &other);
        Type type() const;
        void type(Type type);

        void setSelected(bool select);
        static Tool &getSelected();
        static Type getSelectedType();
        static void unselect();

        void isUsed(bool used);
        bool isUsed() const;

        static void startListen(ToolListener *listener);
        static void endListen(ToolListener *listener);

    protected:


    private:
        static void beginBroadcast();
        static void endBroadcast();
        static void broadcast_toolChanged(Tool *oldTool, Tool* newTool);
        static void broadcast_toolCleared(Tool *oldTool);

        Type m_type;
        static Tool *m_selected;
        bool m_isInUse;

        static bool broadcastingToListeners;
        static vector<ToolListener* > m_toolListener;
        static vector<ToolListener* > m_listenerToAdd;
        static vector<ToolListener* > m_listenerToRemove;
};
#endif
