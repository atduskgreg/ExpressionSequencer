#include "Envelope.hpp"
#include <vector>

class Sequence {
  std::vector<Envelope *> envelopes;

public:
  float f(float t) { return envelopes.front()->f(t); }
  Sequence() { envelopes.push_back(new Envelope()); }
  ~Sequence() {}
};
