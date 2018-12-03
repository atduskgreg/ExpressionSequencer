#include "ExpSeqUI.hpp"
#include <iostream>

struct ExpSeqWidget : ModuleWidget {
    ExpSeqWidget(ExpSeq *module) : ModuleWidget(module) {
        setPanel(SVG::load(assetPlugin(plugin, "res/ExpSeq.svg")));

        addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(Widget::create<ScrewSilver>(
            Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(Widget::create<ScrewSilver>(
            Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(Widget::create<ScrewSilver>(
            Vec(box.size.x - 2 * RACK_GRID_WIDTH,
                RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addParam(ParamWidget::create<PlayButton>(
            Vec(250 - 18, 234 - 21), module, ExpSeq::PLAY_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));

        ExpSeq::ParamIds levelParamIds[] = {
            ExpSeq::ParamIds::LEVEL1_PARAM, ExpSeq::ParamIds::LEVEL2_PARAM,
            ExpSeq::ParamIds::LEVEL3_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM,
            ExpSeq::ParamIds::LEVEL5_PARAM};
        for (int i = 0; i < 5; i++) {
            int x = 80 - 11 + 22 * i + 34 * i;
            int y = 125 - 11;
            addParam(ParamWidget::create<RoundSmallBlackKnob>(
                Vec(x, y), module, levelParamIds[i], ExpSeq::MIN_LEVEL_VOLTAGE,
                ExpSeq::MAX_LEVEL_VOLTAGE, ExpSeq::MID_LEVEL_VOLTAGE));
        }

        ExpSeq::ParamIds posParamIds[] = {ExpSeq::ParamIds::POS2_PARAM,
                                          ExpSeq::ParamIds::POS3_PARAM,
                                          ExpSeq::ParamIds::POS4_PARAM};
        for (int i = 0; i < 3; i++) {
            int x = 137 - 11 + 22 * i + 34 * i;
            int y = 169 - 11;
            addParam(ParamWidget::create<RoundSmallBlackKnob>(
                Vec(x, y), module, posParamIds[i], ExpSeq::MIN_VAL,
                ExpSeq::MAX_VAL, ExpSeq::MID_VAL));
        }

        addParam(ParamWidget::create<UpButton>(
            Vec(31 - 11, 125 - 11), module, ExpSeq::ParamIds::UP_SEQUENCE_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
        addParam(ParamWidget::create<DownButton>(
            Vec(31 - 11, 169 - 11), module, ExpSeq::ParamIds::DOWN_SEQUENCE_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));

        addParam(ParamWidget::create<RoundSmallBlackKnob>(
            Vec(303 - 11, 169 - 11), module, ExpSeq::ParamIds::LENGTH_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));
        addParam(ParamWidget::create<RoundSmallBlackKnob>(
            Vec(353 - 11, 238 - 11), module, ExpSeq::ParamIds::TEMPO_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));

        addParam(ParamWidget::create<SmallPot>(
            Vec(340 - 11, 172 - 11), module, ExpSeq::ParamIds::X_QUANT_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));
        addParam(ParamWidget::create<SmallPot>(
            Vec(372 - 11, 172 - 11), module, ExpSeq::ParamIds::Y_QUANT_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MID_VAL));

        addParam(ParamWidget::create<PlusButton>(
            Vec(353 - 11, 125 - 11), module, ExpSeq::ParamIds::ADD_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
        addParam(ParamWidget::create<ForwardButton>(
            Vec(303 - 11, 238 - 11), module,
            ExpSeq::ParamIds::STEP_FORWARD_PARAM, ExpSeq::MIN_VAL,
            ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
        addParam(ParamWidget::create<BackButton>(
            Vec(193 - 11, 238 - 11), module, ExpSeq::ParamIds::STEP_BACK_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
        addParam(ParamWidget::create<RewindButton>(
            Vec(137 - 11, 238 - 11), module, ExpSeq::ParamIds::REWIND_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));
        addParam(ParamWidget::create<RecordButton>(
            Vec(77 - 11, 310 - 11), module, ExpSeq::ParamIds::RECORD_PARAM,
            ExpSeq::MIN_VAL, ExpSeq::MAX_VAL, ExpSeq::MIN_VAL));

        ExpSeq::OutputIds gateOutIds[] = {
            ExpSeq::OutputIds::GATE1_OUTPUT, ExpSeq::OutputIds::GATE2_OUTPUT,
            ExpSeq::OutputIds::GATE3_OUTPUT, ExpSeq::OutputIds::GATE4_OUTPUT};
        for (int i = 0; i < 4; i++) {
            int x = 169 - 11 + 22 * i + 23 * i;
            int y = 310 - 11;
            addOutput(Port::create<PJ301MPort>(Vec(x, y), Port::OUTPUT, module,
                                               gateOutIds[i]));
        }

        ExpSeq::OutputIds cvOutIds[] = {
            ExpSeq::OutputIds::CV1_OUTPUT, ExpSeq::OutputIds::CV2_OUTPUT,
            ExpSeq::OutputIds::CV3_OUTPUT, ExpSeq::OutputIds::CV4_OUTPUT};
        for (int i = 0; i < 4; i++) {
            int x = 169 - 11 + 22 * i + 23 * i;
            int y = 355 - 11;
            addOutput(Port::create<PJ301MPort>(Vec(x, y), Port::OUTPUT, module,
                                               cvOutIds[i]));
        }

        addOutput(Port::create<PJ301MPort>(Vec(353 - 11, 310 - 11),
                                           Port::OUTPUT, module,
                                           ExpSeq::OutputIds::EOC_OUTPUT));

        addOutput(Port::create<PJ301MPort>(Vec(353 - 11, 355 - 11),
                                           Port::OUTPUT, module,
                                           ExpSeq::OutputIds::CLOCK_OUTPUT));

        ExpSeq::InputIds cvInIds[] = {
            ExpSeq::InputIds::RUN_INPUT, ExpSeq::InputIds::RESET_INPUT,
            ExpSeq::InputIds::CLOCK_INPUT, ExpSeq::InputIds::GATE_INPUT};
        for (int i = 0; i < 4; i++) {
            int x = 31 - 11;
            int y = 221 - 11 + 22 * i + 22 * i;
            addInput(Port::create<PJ301MPort>(Vec(x, y), Port::INPUT, module,
                                              cvInIds[i]));
        }

        addInput(Port::create<PJ301MPort>(Vec(75 - 11, 221 - 11 + 22 + 22),
                                          Port::INPUT, module,
                                          ExpSeq::InputIds::RANDOM_INPUT));
        addInput(Port::create<PJ301MPort>(
            Vec(75 - 11, 221 - 11 + 22 * 3 + 22 * 3), Port::INPUT, module,
            ExpSeq::InputIds::CV_INPUT));

        {
            ExpSeqDisplay *display = new ExpSeqDisplay();
            display->module = module;
            display->box.pos = Vec(6, 7);
            display->box.size = Vec(367, 95);
            display->Init();
            addChild(display);
            module->display = display;
        }
    }
};

void ExpSeq::step() {
    if(this->display == nullptr)
    {
        return;
    }
    float bpm = 40.0 + 200.0 * params[TEMPO_PARAM].value;

    Envelope *envelope = new Envelope();
    this->display->loadUserValues(params);
    auto voltages = this->display->getEnvelopeVoltages();

    auto it = voltages.begin();
    auto it2 = voltages.begin();
    it2++;
    float startTime = 0.0;
    while(it2 != voltages.end()) {
        float startLevel = (*it).second;
        float endLevel = (*it2).second;
        envelope->addSegment(startLevel, endLevel, 1, startTime);
        //printf("%f %f %f ", startLevel, endLevel, startTime);
        startTime = (*it).first;
        //printf("( %f )\n", startTime);
        it++;
        it2++;
    }
    //printf("\n");

    //sequence.clearEnvelopes();
    //sequence.addEnvelope(envelope);

    if (resetTrigger.process(inputs[RESET_INPUT].value)) {
        info("RESET!");
        time = 0;
    }

    if (inputs[CLOCK_INPUT].active) {
        clockTrigger.process(inputs[CLOCK_INPUT].value);
    } else {
        clockTrigger.process(0);
    }

    bool wasRunning = runTrigger.isHigh();
    runTrigger.process(params[PLAY_PARAM].value + (inputs[RUN_INPUT].active
                                                       ? inputs[RUN_INPUT].value
                                                       : 0));
    if (!wasRunning && runTrigger.isHigh()) {
        info("ExpSeq starting run at %f BPM", bpm);
    }
    if (wasRunning && !runTrigger.isHigh()) {
        info("ExpSeq stopping run at %f BPM", bpm);
    }
    if (runTrigger.isHigh()) {
        double dTime = 1.0 / static_cast<double>(engineGetSampleRate());
        time += dTime;

        outputs[CV1_OUTPUT].value = clamp(sequence.f(time), 0.0, 10.0);
    }
}

// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelExpSeq = Model::create<ExpSeq, ExpSeqWidget>(
    "ExpressionSequencer", "ExpSeq", "Expression Sequencer", OSCILLATOR_TAG);
