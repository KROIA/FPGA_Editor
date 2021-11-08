#ifndef TOOLINTERFACE_H
#define TOOLINTERFACE_H

#include "classdef.h"

class ToolListener
{
    public:
        virtual void toolChanged(Tool *oldTool, Tool *newTool) =0;
        virtual void toolCleared(Tool *oldTool) = 0;
};
#endif // TOOLINTERFACE_H
