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
    onStep = 0;
    offStep = 1;
    isQuantized = false;
}

void Envelope::clearSegments() { segments.clear(); }

void Envelope::addSegment(float startLevel, float endLevel, float exp, float startTime) {
    segments.push_back(new Segment(startLevel, endLevel, exp));
    segmentStarts.push_back(startTime);
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

int Sequence::length() {
    struct lengthAccumulator {
        int operator()(int oldvalue, Envelope *e) const {
            return oldvalue + e->length();
        }
    };
    return std::accumulate(envelopes.begin(), envelopes.end(), 0,
                           lengthAccumulator());
}

void Sequence::clearEnvelopes() { envelopes.clear(); }

void Sequence::addEnvelope(Envelope *envelope) {
    envelopes.push_back(envelope);
}
