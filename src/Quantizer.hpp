#pragma once
#include <vector>

class Quantizer
{
    public:
        enum Scale
        {
            NONE,
            VOLT_PER_OCTAVE,
            BEATS_PER_NOTE
        };

        static std::vector<float> VoltPerOctaveValues(float minVal, float maxVal);
        static std::vector<float> BeatsPerNoteValues(float minVal, float maxVal, int numBeats);

        Quantizer(){ this->setScale(Scale::NONE); }
        Quantizer(std::vector<float> values);
        float quantize(float val);
        std::vector<float> getValidValues(){ return validValues; }
        void setValidValues(std::vector<float> values){ validValues = values;  }
        Scale getScale(){ return scale; }
        void setScale(Scale inScale, float minVal=0, float maxVal=0, int numBeats=0);

    private:
        std::vector<float> validValues;
        Scale scale;
};