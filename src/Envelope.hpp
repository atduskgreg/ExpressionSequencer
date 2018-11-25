#include "Segment.hpp"
#include <vector>

class Envelope {
  int onStep;
  int offStep;
  bool isQuantized;
  std::vector<Segment *> segments;

public:
  Envelope() {
    segments.push_back(new Segment());
    onStep = 0;
    offStep = 1;
    isQuantized = false;
  }
  float f(float t) { return segments.front()->f(t); }
};
