
# Expression Sequencer

## TODO

### Authoring

* There is a concept of the current envelope number
* Pressing the add segment button creates a new envelope after the current one
* Pressing the forward and back buttons changes which envelope is displayed
* Pressing the rewind button jumps to the first envelope in the sequence
* There is a way to author all of the sequences

### Playback

* Pressing play runs through all envelopes
* There is a playhead displayed on the screen tracking the playing sequence
* There is an option to set the sequence to loop (another UI button -- near the shuttle controls?)

### Quantization

* Add toggle button for Y quantize
* Label notes on Y-axis for quantize display
* Add knob for X quantize (beats)
* Fix quantize wrap-around bug
* Draw x-quantize grid
* Enforce x quantize
* Enforce quantize on output variables

### UI

* Need a button to toggle y-axis quantization
* Need a knob to control x-axis quantization
* Need a toggle to control looping
* Need controls to switch between sequences
* Need on-screen feedback about:
* * Current sequence num
* * Current location within the sequence

### Outputs

* Do we want an end-of-sequence trigger? (we could do looping through patching that to the reset input)