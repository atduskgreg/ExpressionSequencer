#include <vector>

class Segment {
    float m;
    float b;
    float n;

public:
    float f(float t);
    Segment();
};

class Envelope {
    int onStep;
    int offStep;
    bool isQuantized;
    std::vector<Segment *> segments;

public:
    float f(float t);
    Envelope();
};

class Sequence {
  std::vector<Envelope *> envelopes;

public:
  float f(float t);
  Sequence();
};
