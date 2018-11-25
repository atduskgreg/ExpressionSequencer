#include "Envelope.hpp"
#include <vector>

class Sequence {
  std::vector<Envelope *> envelopes;

public:
  float f(float t);
  Sequence();
};
