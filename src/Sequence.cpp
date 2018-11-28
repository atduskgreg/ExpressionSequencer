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
    std::vector<float>::iterator upper = std::upper_bound(segmentStarts.begin(), segmentStarts.end(), t);
    float segmentStartTime = *upper;
    float segmentEndTime = 1.0;
    int segmentIndex = upper-segmentStarts.begin();
    if(upper != segmentStarts.end()) {
        ++upper;
        segmentEndTime = *upper;
    }
    printf("Segment idx %d from %f to %f\n", segmentIndex, segmentStartTime, segmentEndTime);
    Segment *s = segments.at(segmentIndex);
    return s->f(t * 4 - segmentIndex);
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
