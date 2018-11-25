#pragma once
#include <math.h>
#include <vector>

class Segment {
  float m;
  float b;
  float n;

public:
  float f(float t) { return m * pow(t, n) + b; };
  Segment() {
    m = 1;
    n = 1.2;
    b = 0;
  }
};