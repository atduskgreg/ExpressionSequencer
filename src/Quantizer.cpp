#include "Quantizer.hpp"
#include <vector>
#include <math.h>

Quantizer::Quantizer(std::vector<float> values)
{
    validValues = values;
}

float Quantizer::quantize(float value)
{
    std::vector<float>::iterator lower = std::lower_bound(validValues.begin(), validValues.end(), value);
    
    if(lower == validValues.end())
    {
        return *lower;
    }
    else
    {
        std::vector<float>::iterator upper = std::next(lower);
        return fabs(*lower - value) > fabs(*upper - value) ? *lower : *upper;
    }
}

std::vector<float> Quantizer::VoltPerOctaveValues(float minVal, float maxVal)
{
    std::vector<float> result;
    float currVal = minVal;
    while(currVal < maxVal)
    {
        result.push_back(currVal);
        currVal += 1.0/12;
    }

    return result;
}

std::vector<float> Quantizer::BeatsPerNoteValues(float minVal, float maxVal, int numBeats)
{
    std::vector<float> result;
    float rangeSize = maxVal - minVal;
    float epsilon = rangeSize / numBeats;
    float currVal = minVal;
    while(currVal <= maxVal)
    {
        result.push_back(currVal);
        currVal += epsilon;
    }

    return result;
}