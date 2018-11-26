#pragma once
#include <vector>

class Quantizer
{
    enum Scale
    {
        NONE,
        VOLT_PER_OCTAVE,
        BEATS_PER_NOTE
    };

    public:
        static std::vector<float> VoltPerOctaveValues(float minVal, float maxVal);
        static std::vector<float> BeatsPerNoteValues(float minVal, float maxVal, int numBeats);

        Quantizer(std::vector<float> values);
        float quantize(float val);
        std::vector<float> getValidValues(){ return validValues; }

    private:
        std::vector<float> validValues;
};