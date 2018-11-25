#include "ExpressionSequencer.hpp"
#include "dsp/digital.hpp"
#include "Sequence.hpp"

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

	ExpSeq() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
    SchmittTrigger clockTrigger;
    double time = 0.0;
    Sequence sequence;
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

float ExpSeq::MAX_LEVEL_VOLTAGE = 10.0;
float ExpSeq::MIN_LEVEL_VOLTAGE = 0.0;
float ExpSeq::MID_LEVEL_VOLTAGE = (ExpSeq::MAX_LEVEL_VOLTAGE - ExpSeq::MIN_LEVEL_VOLTAGE)/2;	

float ExpSeq::MAX_VAL = 1.0;
float ExpSeq::MIN_VAL = 0.0;
float ExpSeq::MID_VAL = (ExpSeq::MAX_VAL - ExpSeq::MID_VAL)/2;	


void ExpSeq::step() {
    double dTime = 1.0 / static_cast<double>(engineGetSampleRate());
    time += dTime;
    
    outputs[CV1_OUTPUT].value = clamp(sequence.f(time), 0.0, 10.0);
}

struct PlayButton : SVGSwitch, ToggleSwitch {
	PlayButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/PlayButtonOff.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/PlayButtonOn.svg")));
	}
};

struct PlusButton : SVGSwitch, MomentarySwitch {
	PlusButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/PlusButton.svg")));
	}
};

struct ForwardButton : SVGSwitch, MomentarySwitch {
	ForwardButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/ForwardButton.svg")));
	}
};

struct BackButton : SVGSwitch, MomentarySwitch {
	BackButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/BackButton.svg")));
	}
};

struct RewindButton : SVGSwitch, MomentarySwitch {
	RewindButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/RewindButton.svg")));
	}
};

struct RecordButton : SVGSwitch, ToggleSwitch {
	RecordButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/RecordButtonOff.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/RecordButtonOn.svg")));
	}
};

struct ExpSeqWidget : ModuleWidget {
	ExpSeqWidget(ExpSeq *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/ExpSeq.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<PlayButton>(Vec(250-18, 234-21), module, ExpSeq::PLAY_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
		
		ExpSeq::ParamIds levelParamIds[] = {ExpSeq::ParamIds::LEVEL1_PARAM, ExpSeq::ParamIds::LEVEL2_PARAM, ExpSeq::ParamIds::LEVEL3_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM, ExpSeq::ParamIds::LEVEL5_PARAM};
		for(int i = 0; i < 5; i++)
		{
			int x = 80-11 + 22*i + 34*i;
			int y = 125-11;
			addParam(ParamWidget::create<RoundSmallBlackKnob>(Vec(x, y), module, levelParamIds[i], ExpSeq::MIN_LEVEL_VOLTAGE, ExpSeq::MAX_LEVEL_VOLTAGE, ExpSeq::MID_LEVEL_VOLTAGE));
		}

		ExpSeq::ParamIds posParamIds[] = {ExpSeq::ParamIds::POS2_PARAM, ExpSeq::ParamIds::POS3_PARAM,ExpSeq::ParamIds::POS4_PARAM};
		for(int i = 0; i < 3; i++)
		{
			int x = 137-11 + 22*i + 34*i;
			int y = 169-11;
			addParam(ParamWidget::create<RoundSmallBlackKnob>(Vec(x, y), module, posParamIds[i], ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));
		}

		addParam(ParamWidget::create<RoundSmallBlackKnob>(Vec(303-11, 169-11), module, ExpSeq::ParamIds::LENGTH_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));
		addParam(ParamWidget::create<RoundSmallBlackKnob>(Vec(353-11, 238-11), module, ExpSeq::ParamIds::TEMPO_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));

		addParam(ParamWidget::create<PlusButton>(Vec(353-11, 169-11), module, ExpSeq::ParamIds::ADD_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
		addParam(ParamWidget::create<ForwardButton>(Vec(303-11, 238-11), module, ExpSeq::ParamIds::STEP_FORWARD_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
		addParam(ParamWidget::create<BackButton>(Vec(193-11, 238-11), module, ExpSeq::ParamIds::STEP_BACK_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
		addParam(ParamWidget::create<RewindButton>(Vec(137-11, 238-11), module, ExpSeq::ParamIds::REWIND_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
		addParam(ParamWidget::create<RecordButton>(Vec(77-11, 310-11), module, ExpSeq::ParamIds::RECORD_PARAM, ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));

		ExpSeq::OutputIds gateOutIds[] = {ExpSeq::OutputIds::GATE1_OUTPUT, ExpSeq::OutputIds::GATE2_OUTPUT, ExpSeq::OutputIds::GATE3_OUTPUT, ExpSeq::OutputIds::GATE4_OUTPUT};
		for(int i = 0; i < 4; i++)
		{
			int x = 169-11 + 22*i + 23*i;
			int y = 310-11;
			addOutput(Port::create<PJ301MPort>(Vec(x, y), Port::OUTPUT, module, gateOutIds[i]));
		}

		ExpSeq::OutputIds cvOutIds[] = {ExpSeq::OutputIds::CV1_OUTPUT, ExpSeq::OutputIds::CV2_OUTPUT, ExpSeq::OutputIds::CV3_OUTPUT, ExpSeq::OutputIds::CV4_OUTPUT};
		for(int i = 0; i < 4; i++)
		{
			int x = 169-11 + 22*i + 23*i;
			int y = 355-11;
			addOutput(Port::create<PJ301MPort>(Vec(x, y), Port::OUTPUT, module, cvOutIds[i]));
		}

		addOutput(Port::create<PJ301MPort>(Vec(353-11, 355-11), Port::OUTPUT, module, ExpSeq::OutputIds::CLOCK_OUTPUT));

		ExpSeq::InputIds cvInIds[] = {ExpSeq::InputIds::RUN_INPUT, ExpSeq::InputIds::RESET_INPUT, ExpSeq::InputIds::CLOCK_INPUT, ExpSeq::InputIds::GATE_INPUT};
		for(int i = 0; i < 4; i++)
		{
			int x = 31-11;
			int y = 221-11  + 22*i + 22*i;
			addInput(Port::create<PJ301MPort>(Vec(x, y), Port::INPUT, module, cvInIds[i]));
		}

		addInput(Port::create<PJ301MPort>(Vec(75-11, 221-11  + 22 + 22), Port::INPUT, module, ExpSeq::InputIds::RANDOM_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(75-11, 221-11  + 22*3 + 22*3), Port::INPUT, module, ExpSeq::InputIds::CV_INPUT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelExpSeq = Model::create<ExpSeq, ExpSeqWidget>("ExpressionSequencer", "ExpSeq", "Expression Sequencer", OSCILLATOR_TAG);
