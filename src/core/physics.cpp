#include "physics.h"

namespace Physics{
    namespace Electrical{
        // Color def
        const Color color_highVoltage       = Color(255,0,0);
        const Color color_lowVoltage        = Color(0,0,0);

        // Voltage def
        const float voltage_max = 5;
        const float voltage_min = 0;

        const float voltage_logic_1_threshold = 3;
        const float voltage_logic_0_threshold = 2;

    }
    bool displayPhysical = false;


    long map(long x, long in_min, long in_max, long out_min, long out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    float mapF(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


    Color coloredVoltage(float voltage)
    {
        int red = mapF(voltage,Physics::Electrical::voltage_min,Physics::Electrical::voltage_max,
                   Physics::Electrical::color_lowVoltage.r,Physics::Electrical::color_highVoltage.r);

        int green = mapF(voltage,Physics::Electrical::voltage_min,Physics::Electrical::voltage_max,
                   Physics::Electrical::color_lowVoltage.g,Physics::Electrical::color_highVoltage.g);

        int blue = mapF(voltage,Physics::Electrical::voltage_min,Physics::Electrical::voltage_max,
                   Physics::Electrical::color_lowVoltage.b,Physics::Electrical::color_highVoltage.b);
        return Color(red,green,blue);
    }

    std::string voltageString(float voltage, int precision, bool leadingUnit)
    {
        std::string str = std::to_string(voltage).substr(0,std::to_string(voltage).find(".") + precision + 1);
        if(leadingUnit)
            str+="V";
        return str;
    }

}
