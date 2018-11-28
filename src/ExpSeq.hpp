#pragma once

#include "ExpressionSequencer.hpp"
#import "ExpSeqUI.hpp"

struct ExpSeqDisplay;

struct ExpSeq : Module {
	static float MIN_LEVEL_VOLTAGE;
	static float MAX_LEVEL_VOLTAGE;
	static float MID_LEVEL_VOLTAGE;

	static float MAX_VAL;
	static float MIN_VAL;
	static float MID_VAL;

	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		LEVEL3_PARAM,
		LEVEL4_PARAM,
		LEVEL5_PARAM,
		POS2_PARAM,
		POS3_PARAM,
		POS4_PARAM,
		LENGTH_PARAM,
		ADD_PARAM,
		TEMPO_PARAM,
		PLAY_PARAM,
		STEP_BACK_PARAM,
		STEP_FORWARD_PARAM,
		REWIND_PARAM,
		RECORD_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		RUN_INPUT,
		RANDOM_INPUT,
		RESET_INPUT,
		CLOCK_INPUT,
		RECORD_INPUT,
		GATE_INPUT,
		CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		GATE1_OUTPUT,
		CV1_OUTPUT,
		GATE2_OUTPUT,
		CV2_OUTPUT,
		GATE3_OUTPUT,
		CV3_OUTPUT,
		GATE4_OUTPUT,
		CV4_OUTPUT,
		CLOCK_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	ExpSeq() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        Envelope *envelope = new Envelope();
        envelope->addSegment(0, 1, 1, 0.0);
        envelope->addSegment(1, 0.5, 0.5, 0.25);
        envelope->addSegment(0.5, 0.5, 1, 0.5);
        envelope->addSegment(0.5, 0, 2, 0.75);
                
        sequence.clearEnvelopes();
        sequence.addEnvelope(envelope);
    }
    SchmittTrigger runTrigger;
    SchmittTrigger clockTrigger;
    SchmittTrigger resetTrigger;
    ExpSeqDisplay *display;
    double time = 0.0;
    Sequence sequence;
	void step() override;
};

float ExpSeq::MAX_LEVEL_VOLTAGE = 10.0;
float ExpSeq::MIN_LEVEL_VOLTAGE = 0.0;
float ExpSeq::MID_LEVEL_VOLTAGE = (ExpSeq::MAX_LEVEL_VOLTAGE - ExpSeq::MIN_LEVEL_VOLTAGE)/2;	

float ExpSeq::MAX_VAL = 1.0;
float ExpSeq::MIN_VAL = 0.0;
float ExpSeq::MID_VAL = (ExpSeq::MAX_VAL - ExpSeq::MID_VAL)/2;
