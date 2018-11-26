#import "ExpSeq.hpp"
#import "kiwi/kiwi.h"


#define DECLARE_BUTTON(ButtonName, ButtonType) \
struct ButtonName : SVGSwitch, ButtonType {\
    ButtonName() {\
        addFrame(SVG::load(assetPlugin(plugin, "res/"#ButtonName"Off.svg")));\
	    addFrame(SVG::load(assetPlugin(plugin, "res/"#ButtonName"On.svg"))); \
    }\
};

DECLARE_BUTTON(PlayButton, 	 	ToggleSwitch);
DECLARE_BUTTON(RecordButton, 	ToggleSwitch);
DECLARE_BUTTON(PlusButton,		MomentarySwitch);
DECLARE_BUTTON(ForwardButton, 	MomentarySwitch);
DECLARE_BUTTON(BackButton, 		MomentarySwitch);
DECLARE_BUTTON(RewindButton, 	MomentarySwitch);

struct ExpSeqDisplay : TransparentWidget {
	ExpSeq *module;

	float getSegmentX(int segmentNum)
	{
		int posStartingX[] = {69, 126, 183, 239, 295}; // TODO: store X-coords somewhere global

		float result = box.pos.x + posStartingX[segmentNum];
		if(segmentNum >= 1 && segmentNum <= 3)
		{
			ExpSeq::ParamIds posParamIds[] = {ExpSeq::ParamIds::POS2_PARAM, ExpSeq::ParamIds::POS3_PARAM, ExpSeq::ParamIds::POS4_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM};
			float posValue = module->params[posParamIds[segmentNum-1]].value;
			result += -100 + 200 * (posValue - ExpSeq::MIN_VAL) / ExpSeq::MAX_VAL;
		}

		return result;
	}

	float getSegmentY(int segmentNum)
	{
		ExpSeq::ParamIds levelParamIds[] = {ExpSeq::ParamIds::LEVEL1_PARAM, ExpSeq::ParamIds::LEVEL2_PARAM, ExpSeq::ParamIds::LEVEL3_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM, ExpSeq::ParamIds::LEVEL5_PARAM};
		float lvlValue = module->params[levelParamIds[segmentNum]].value;
		return box.size.y - box.size.y * (lvlValue - ExpSeq::MIN_LEVEL_VOLTAGE) / ExpSeq::MAX_LEVEL_VOLTAGE;
	}

	void drawSegmentEdge(NVGcontext *vg, int segmentNum)
	{
		float xPos = getSegmentX(segmentNum);
		float yPos = getSegmentY(segmentNum);

		nvgBeginPath(vg);
		nvgMoveTo(vg, xPos, box.pos.y);
		nvgLineTo(vg, xPos, box.pos.y + box.size.y);
		nvgStrokeColor(vg, nvgRGBA(255,255,255,125));
		nvgStrokeWidth(vg, 2.0);
	    nvgStroke(vg);
		
		nvgBeginPath(vg);
		nvgCircle(vg, xPos, yPos, 4.0f);
		nvgFillColor(vg, nvgRGBA(255,255,255,255));
		nvgFill(vg);
	}

	void drawSegmentBody(NVGcontext *vg, int startSegment, int endSegment)
	{
		nvgBeginPath(vg);
		nvgMoveTo(vg, getSegmentX(startSegment), getSegmentY(startSegment));
		nvgLineTo(vg, getSegmentX(endSegment), getSegmentY(endSegment));
		nvgStrokeWidth(vg, 3.0);
		nvgStrokeColor(vg, nvgRGBA(0, 255,0, 255));
		nvgStroke(vg);
	}

	void draw(NVGcontext *vg) override 
	{
		// subtle gray border
		nvgBeginPath(vg);
		nvgRect(vg, box.pos.x,box.pos.y, box.size.x,box.size.y);
		nvgStrokeColor(vg, nvgRGBA(165,165,165,255));
		nvgStroke(vg);
		for(int i = 0; i < 5; i++){
			if(i < 4)
			{
				drawSegmentBody(vg, i, i+1);
			}
			drawSegmentEdge(vg, i);
		}
	}
};