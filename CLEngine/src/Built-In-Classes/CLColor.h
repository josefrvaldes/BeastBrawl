#pragma once

#include <stdint.h>

namespace CLE{
class CLColor{
    public:
        CLColor();
        CLColor(uint8_t red,uint8_t green,uint8_t blue,uint8_t alpha);
        ~CLColor(){};

        uint8_t GetRed()   const { return r;};
        uint8_t GetGreen() const { return g;};
        uint8_t GetBlue()  const { return b;};
        uint8_t GetAlpha() const { return a;};

        float GetRedNormalized()   const { return r/255.0;};
        float GetGreenNormalized() const { return g/255.0;};
        float GetBlueNormalized()  const { return b/255.0;};
        float GetAlphaNormalized() const { return a/255.0;};
    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
};
};
