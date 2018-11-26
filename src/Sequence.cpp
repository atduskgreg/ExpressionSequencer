#include "Sequence.hpp"
#include <vector>
#include <numeric>
#include <math.h>

float Segment::f(float t) { return m * pow(t, n) + b; }

Segment::Segment() {
    m = 1;
    n = 1.2;
    b = 0;
}

Envelope::Envelope() {
    segments.push_back(new Segment());
    onStep = 0;
    offStep = 1;
    isQuantized = false;
}
float Envelope::f(float t) { return segments.front()->f(t); }
int Envelope::length() { 
    return offStep - onStep;
}

Sequence::Sequence() { envelopes.push_back(new Envelope()); }
float Sequence::f(float t) { return envelopes.front()->f(t); }

struct lengthAccumulator {
   int operator()( int oldvalue, Envelope *e ) const {
      return oldvalue + e->length();
   }
};

int Sequence::length() {
    return std::accumulate(envelopes.begin(), envelopes.end(), 0, lengthAccumulator());
}
