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
    int onStep;
    int offStep;
    bool isQuantized;
    std::vector<Segment *> segments;

public:
    float f(float t);
    int length();
    Envelope();
};

class Sequence {
  std::vector<Envelope *> envelopes;

public:
  float f(float t);
  int length();
  Sequence();
};
