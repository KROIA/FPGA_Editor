#ifndef LOGICGATEIMPORTED_H
#define LOGICGATEIMPORTED_H

#include "gate.h"
#include "verilog/verilogModule.h"

class LogicGateImported     :   public Gate
{
    public:
        LogicGateImported(const SimpleVerilog::Module &module);
        //LogicGateImported(const string &filePath);
        LogicGateImported(const LogicGateImported &other);
        ~LogicGateImported();

        virtual Gate *clone() const;

        static LogicGateImported *importGate(const string &path);

    private:
        void processLogic();

        void setup();

        SimpleVerilog::Module *m_module;
};
#endif // LOGICGATEIMPORTED_H
