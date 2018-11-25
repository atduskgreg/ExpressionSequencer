#include "ExpressionSequencer.hpp"


struct ExpSeq : Module {
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
		TEMPO_PARAM,
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
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void ExpSeq::step() {
}

struct PlayButton : SVGSwitch, ToggleSwitch {
	PlayButton() {
		addFrame(SVG::load(assetGlobal("res/PlayButtonOn.svg")));
		addFrame(SVG::load(assetGlobal("res/PlayButtonOff.svg")));
	}
};

struct ExpSeqWidget : ModuleWidget {
	ExpSeqWidget(ExpSeq *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/ExpSeq.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<PlayButton>(Vec(250, 37), module, ExpSeq::PITCH_PARAM, -3.0, 3.0, 0.0));


		//addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 87), module, ExpSeq::PITCH_PARAM, -3.0, 3.0, 0.0));

		//addInput(Port::create<PJ301MPort>(Vec(33, 186), Port::INPUT, module, ExpSeq::PITCH_INPUT));

		//addOutput(Port::create<PJ301MPort>(Vec(33, 275), Port::OUTPUT, module, ExpSeq::SINE_OUTPUT));

		//addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, ExpSeq::BLINK_LIGHT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelExpSeq = Model::create<ExpSeq, ExpSeqWidget>("ExpressionSequencer", "ExpSeq", "Expression Sequencer", OSCILLATOR_TAG);
