#ifndef PHYSICS_H
#define PHYSICS_H

#include <SFML/Graphics.hpp>
#include <math.h>

using sf::Color;

namespace Physics{
    namespace Electrical{
        // Color def
        const extern Color color_highVoltage;
        const extern Color color_lowVoltage;

        // Voltage def
        const extern float voltage_max;
        const extern float voltage_min;

        const extern float voltage_logic_1_threshold;
        const extern float voltage_logic_0_threshold;

    }
    extern bool displayPhysical;


    long map(long x, long in_min, long in_max, long out_min, long out_max);
    float mapF(float x, float in_min, float in_max, float out_min, float out_max);
    Color coloredVoltage(float voltage);
    std::string voltageString(float voltage, int precision, bool leadingUnit = true);
    bool voltageToLogicLevel(float voltage);
    float logicLevelToVoltage(bool logicLevel);
}

#endif // PHYSICS_H
