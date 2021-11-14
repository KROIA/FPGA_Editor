#ifndef IOPIN_H
#define IOPIN_H

#include "gate.h"

class IoPin :   public Gate
{
    public:
        IoPin(const string &name,Pin::Direction dir);
        IoPin(const IoPin &other);
        ~IoPin();

        void setVoltage(float voltage);


    protected:

    private:
        Pin::Direction m_direction;
};

#endif // IOPIN_H
