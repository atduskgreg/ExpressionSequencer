#include "Sequence.hpp"
#include <math.h>
#include <numeric>
#include <vector>

float Segment::f(float t) { return m * pow(t, n) + b; }

Segment::Segment() {
    m = 1;
    n = 1.2;
    b = 0;
}

Segment::Segment(float startLevel, float endLevel, float exp) {
    b = startLevel;
    m = endLevel - startLevel;
    n = exp;
}

Envelope::Envelope() {
    segments.push_back(new Segment(0, 1, 1));
    segments.push_back(new Segment(1, 0.5, 0.5));
    segments.push_back(new Segment(0.5, 0.5, 1));
    segments.push_back(new Segment(0.5, 0, 2));
    onStep = 0;
    offStep = 1;
    isQuantized = false;
}
float Envelope::f(float t) {
    int segmentCount = segments.size();
    int segmentIndex = (int)(t * segmentCount);
    Segment *s = segments.at(segmentIndex);
    return s->f(t * segmentCount - segmentIndex);
}
int Envelope::length() { return offStep - onStep; }

Sequence::Sequence() { envelopes.push_back(new Envelope()); }
float Sequence::f(float t) {
    return envelopes.front()->f(fmod(t, this->length()));
}

struct lengthAccumulator {
    int operator()(int oldvalue, Envelope *e) const {
        return oldvalue + e->length();
    }
};

int Sequence::length() {
    return std::accumulate(envelopes.begin(), envelopes.end(), 0,
                           lengthAccumulator());
}
