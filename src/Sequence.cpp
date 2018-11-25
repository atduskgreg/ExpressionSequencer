#include "Envelope.hpp"
#include "Sequence.hpp"
#include <vector>

float Sequence::f(float t) { return envelopes.front()->f(t); }
Sequence::Sequence() { envelopes.push_back(new Envelope()); }
