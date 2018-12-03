#pragma once
#import "ExpSeq.hpp"
#import "Quantizer.hpp"
#include <array>
#include <cassert>

#define DECLARE_BUTTON(ButtonName, ButtonType) \
struct ButtonName : SVGSwitch, ButtonType {\
    ButtonName() {\
        addFrame(SVG::load(assetPlugin(plugin, "res/"#ButtonName"Off.svg")));\
	    addFrame(SVG::load(assetPlugin(plugin, "res/"#ButtonName"On.svg"))); \
    }\
};

#define DECLARE_POT(PotName, MinAngle, MaxAngle) \
struct PotName : SVGKnob {\
    PotName() {\
		minAngle = MinAngle*M_PI;\
		maxAngle = MaxAngle*M_PI;\
		setSVG(SVG::load(assetPlugin(plugin, "res/"#PotName".svg")));\
    }\
};

DECLARE_BUTTON(PlayButton, 	 	ToggleSwitch);
DECLARE_BUTTON(RecordButton, 	ToggleSwitch);
DECLARE_BUTTON(PlusButton,		MomentarySwitch);
DECLARE_BUTTON(ForwardButton, 	MomentarySwitch);
DECLARE_BUTTON(BackButton, 		MomentarySwitch);
DECLARE_BUTTON(RewindButton, 	MomentarySwitch);
DECLARE_BUTTON(UpButton,        MomentarySwitch);
DECLARE_BUTTON(DownButton,      MomentarySwitch);
DECLARE_POT(SmallPot, -0.75, 0.75);

struct ExpSeqDisplay : TransparentWidget {
	ExpSeq *module;
    float playheadPos = 0.0f; // 0.0 - 1.0
    static const int numSegments = 5;
    std::array<float, numSegments> realPositions;
    std::array<float, numSegments> constrainedPositions;

    Quantizer xQuantizer;
    Quantizer yQuantizer;

    void Init()
    {
        info("ExpSeqDisplay::Init()");
        setQuantizerX(Quantizer::Scale::BEATS_PER_NOTE);
        setQuantizerY(Quantizer::Scale::VOLT_PER_OCTAVE);
    }

    void setPlayhead(float p)
    {
        playheadPos = p;
    }

    void updateQuantizer(float xVal, float yVal)
    {
        if(xVal < 1e-4)
        {
            setQuantizerX(Quantizer::Scale::NONE);
        }
        else
        {
            setQuantizerX(Quantizer::Scale::BEATS_PER_NOTE);
        }

        if(yVal < 1e-4)
        {
            setQuantizerY(Quantizer::Scale::NONE);
        }
        else
        {
            setQuantizerY(Quantizer::Scale::VOLT_PER_OCTAVE);
        }
    }

    void setQuantizerY(Quantizer::Scale scale)
    {
        if(yQuantizer.getScale() != scale)
        {
            yQuantizer.setScale(scale, 0, box.size.y);
        }
    }
    
    void setQuantizerX(Quantizer::Scale scale)
    {
        if(xQuantizer.getScale() != scale)
        {
            xQuantizer.setScale(scale, 69, 295, 4); // TODO: Magic numbers! (this is hard-coded first and last stage x)
        }
    }

    std::vector<std::pair<float, float>> getEnvelopeVoltages()
    {
        std::vector<std::pair<float, float>> result;
        for(int i = 0; i < numSegments; i++)
        {
            std::pair<float, float> pointData;
            pointData.first = getVoltageX(i);
            pointData.second = getVoltageY(i);
            result.push_back(pointData);
        }

        return result;
    }

    void loadUserValues(std::vector<rack::Param> params)
    {
        updateQuantizer(params[ExpSeq::ParamIds::X_QUANT_PARAM].value, params[ExpSeq::ParamIds::Y_QUANT_PARAM].value);
        loadRealPositions();
        constrainPositions();
    }

    void loadRealPositions()
    {
        int posStartingX[] = {69, 126, 183, 239, 295}; // TODO: store X-coords somewhere global
		ExpSeq::ParamIds posParamIds[] = {ExpSeq::ParamIds::POS2_PARAM, ExpSeq::ParamIds::POS3_PARAM, ExpSeq::ParamIds::POS4_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM};

        for(int i = 0; i < numSegments; i++)
        {
            realPositions[i] = box.pos.x + posStartingX[i];

		    if(i >= 1 && i <= 3)
		    {
                float knobValue = module->params[posParamIds[i-1]].value;
			    realPositions[i] += -200 + 400 * (knobValue - ExpSeq::MIN_VAL) / ExpSeq::MAX_VAL;
		    }  
        }
    }

    void constrainPositions()
    {
        constrainedPositions[0] = realPositions[0];
        constrainedPositions[numSegments-1] = realPositions[numSegments-1];

        for(int i = 1; i < numSegments-1; i++)
        {
            float prevVal = constrainedPositions[i-1];
            constrainedPositions[i] = realPositions[i];

            if(constrainedPositions[i] < prevVal)
            {
                constrainedPositions[i] = prevVal;
            }
            
            if(constrainedPositions[i] > constrainedPositions[numSegments-1])
            {
                constrainedPositions[i] = constrainedPositions[numSegments-1];
            }
        }
    }


    float getVoltageX(int segmentNum)
    {
        float range = getSegmentX(numSegments - 1) - getSegmentX(0);
        float voltage = (getSegmentX(segmentNum) - getSegmentX(0)) / range;
        // assert(isfinite(voltage));
        return voltage;
    }

	float getSegmentX(int segmentNum)
	{
		return constrainedPositions[segmentNum];
	}

    float getVoltageY(int segmentNum)
    {
		ExpSeq::ParamIds levelParamIds[] = {ExpSeq::ParamIds::LEVEL1_PARAM, ExpSeq::ParamIds::LEVEL2_PARAM, ExpSeq::ParamIds::LEVEL3_PARAM, ExpSeq::ParamIds::LEVEL4_PARAM, ExpSeq::ParamIds::LEVEL5_PARAM};
		return module->params[levelParamIds[segmentNum]].value;
    }

	float getSegmentY(int segmentNum)
	{
		float val = box.size.y - box.size.y * (getVoltageY(segmentNum) - ExpSeq::MIN_LEVEL_VOLTAGE) / ExpSeq::MAX_LEVEL_VOLTAGE;
    
        if(yQuantizer.getScale() != Quantizer::Scale::NONE)
        {
            return yQuantizer.quantize(val);
        }
        else
        {
            return val;
        }
    }

    void drawPlayhead(NVGcontext *vg)
    {
        float minX = getSegmentX(0);
        float maxX = getSegmentX(numSegments - 1);

        float playheadX = playheadPos * (maxX - minX) + minX;

        nvgBeginPath(vg);
        nvgMoveTo(vg, playheadX, box.pos.y);
		nvgLineTo(vg, playheadX, box.pos.y + box.size.y);
		nvgStrokeColor(vg, nvgRGBA(255,0,0,200));
		nvgStrokeWidth(vg, 3.0);
	    nvgStroke(vg);
    }

    void drawQuantizeGridY(NVGcontext *vg)
    {
        if(yQuantizer.getScale() != Quantizer::Scale::NONE)
        {
            std::vector<float> yVals = yQuantizer.getValidValues();
            nvgBeginPath(vg);
 
            for(auto y : yVals)
            {
                nvgMoveTo(vg, 55, box.pos.y +y);
                nvgLineTo(vg, 315, box.pos.y + y);
            }
            nvgStrokeColor(vg, nvgRGBA(255,0,255,125));
		    nvgStrokeWidth(vg, 1.0);
	        nvgStroke(vg);
        }
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
       //loadUserValues(); // this happens in step() if it doesn't it would need to happen here.

        drawQuantizeGridY(vg);

		for(int i = 0; i < 5; i++){
			if(i < 4)
			{
				drawSegmentBody(vg, i, i+1);
			}
			drawSegmentEdge(vg, i);
		}

        drawPlayhead(vg);

		// subtle gray border
        nvgBeginPath(vg);
		nvgRect(vg, box.pos.x,box.pos.y, box.size.x,box.size.y);
		nvgStrokeColor(vg, nvgRGBA(165,165,165,255));
		nvgStroke(vg);
	}
};
