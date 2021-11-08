#include "tool.h"

Tool *Tool::m_selected = nullptr;
bool Tool::broadcastingToListeners = false;
vector<ToolListener* > Tool::m_toolListener;
vector<ToolListener* > Tool::m_listenerToAdd;
vector<ToolListener* > Tool::m_listenerToRemove;

Tool::Tool()
{
    m_type = Type::none;
}
Tool::Tool(Type type)
{
    m_type = type;
}
Tool::Tool(const Tool &other)
{
    m_type = other.m_type;
}
Tool::~Tool()
{
    if(m_selected == this)
    {
        m_selected = nullptr;
        broadcast_toolCleared(this);
    }
}

const Tool &Tool::operator=(const Tool &other)
{
    m_type = other.m_type;
    return *this;
}

Tool::Type Tool::type() const
{
    return m_type;
}
void Tool::type(Type type)
{
    m_type = type;
}

void Tool::setSelected(bool select)
{
    if(select)
    {
        if(m_selected != this)
        {
            Tool* tmp = m_selected;
            m_selected = this;
            m_isInUse = false;
            broadcast_toolChanged(tmp,m_selected);
        }


    }
    else
    {
        if(m_selected == this)
        {
            m_selected = nullptr;
            m_isInUse = false;
            broadcast_toolCleared(this);

        }
    }
}
Tool &Tool::getSelected()
{
    return *m_selected;
}
Tool::Type Tool::getSelectedType()
{
    if(m_selected == nullptr)
        return Type::none;
    return m_selected->m_type;
}
void Tool::unselect()
{
    if(m_selected == nullptr)
        return;

    m_selected->m_isInUse = false;
    Tool* tmp = m_selected;
    m_selected = nullptr;
    broadcast_toolCleared(tmp);
}
void Tool::isUsed(bool used)
{
    m_isInUse = used;
}
bool Tool::isUsed() const
{
    return m_isInUse;
}


void Tool::startListen(ToolListener *listener)
{
    if(broadcastingToListeners)
    {
        m_listenerToAdd.push_back(listener);
        return;
    }
    for(size_t i=0; i<m_toolListener.size(); i++)
        if(m_toolListener[i] == listener)
            return;
    m_toolListener.push_back(listener);
}
void Tool::endListen(ToolListener *listener)
{
    if(broadcastingToListeners)
    {
        m_listenerToRemove.push_back(listener);
        return;
    }
    for(size_t i=0; i<m_toolListener.size(); i++)
        if(m_toolListener[i] == listener)
            m_toolListener.erase(m_toolListener.begin()+i);
}

void Tool::beginBroadcast()
{
    broadcastingToListeners = true;
}
void Tool::endBroadcast()
{
    broadcastingToListeners = false;
    for(size_t i=0; i<m_listenerToAdd.size(); i++)
    {
        startListen(m_listenerToAdd[i]);
    }
    m_listenerToAdd.clear();
    for(size_t i=0; i<m_listenerToRemove.size(); i++)
    {
        endListen(m_listenerToRemove[i]);
    }
    m_listenerToRemove.clear();
}
void Tool::broadcast_toolChanged(Tool *oldTool, Tool* newTool)
{
    beginBroadcast();
    for(size_t i=0; i<m_toolListener.size(); i++)
    {
        m_toolListener[i]->toolChanged(oldTool,newTool);
    }
    endBroadcast();
}
void Tool::broadcast_toolCleared(Tool *oldTool)
{
    beginBroadcast();
    for(size_t i=0; i<m_toolListener.size(); i++)
    {
        m_toolListener[i]->toolCleared(oldTool);
    }
    endBroadcast();
}
