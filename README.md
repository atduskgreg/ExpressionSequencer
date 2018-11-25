
# Expression Sequencer

Sequence:

* List of Envelopes
* Step length

Envelope:
* OnStep int
* OffStep int
* IsQuantized bool
* List of Segments

Segment:
* m float
* n int
* b float
* f(t) = m*t^n +b
