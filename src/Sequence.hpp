#pragma once
#include <vector>

class Segment {
    float m;
    float b;
    float n;

  public:
    float f(float t);
    Segment();
    Segment(float startLevel, float endLevel, float exp);
};

class Envelope {
    bool isQuantized;
    std::vector<Segment *> segments;
    std::vector<float> segmentStarts;

  public:
    int onStep;
    int offStep;

    float f(float t);
    int length();
    void clearSegments();
    void addSegment(float startLevel, float endLevel, float exp, float startTime);
    Envelope();
};

class Sequence {
    std::vector<Envelope *> envelopes;

  public:
    float f(float t);
    void clearEnvelopes();
    void addEnvelope(Envelope *envelope);
    int length();
    Sequence();
};
