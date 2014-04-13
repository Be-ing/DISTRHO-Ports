/*
 * Power Juice Plugin
 * Copyright (C) 2014 Andre Sklenar <andre.sklenar@gmail.com>, www.juicelab.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "PowerJuiceUI.hpp"



using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

PowerJuiceUI::PowerJuiceUI()
    : UI(),
      fAboutWindow(this)
{
    // background
    fImgBackground = Image(PowerJuiceArtwork::backgroundData, PowerJuiceArtwork::backgroundWidth, PowerJuiceArtwork::backgroundHeight, GL_BGR);

    // about
    Image imageAbout(PowerJuiceArtwork::aboutData, PowerJuiceArtwork::aboutWidth, PowerJuiceArtwork::aboutHeight, GL_BGR);
    fAboutWindow.setImage(imageAbout);

    // knobs
    Image knobImage(PowerJuiceArtwork::knobData, PowerJuiceArtwork::knobWidth, PowerJuiceArtwork::knobHeight);

    // knob Attack
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(37, 213);
    fKnobAttack->setRange(0.1f, 1000.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(20.0f);
    fKnobAttack->setRotationAngle(270);
    fKnobAttack->setCallback(this);

    // knob Release
    fKnobRelease = new ImageKnob(this, knobImage);
    fKnobRelease->setPos(136, 213);
    fKnobRelease->setRange(0.1f, 1000.0f);
    fKnobRelease->setValue(0.1f);
    fKnobRelease->setRotationAngle(270);
    fKnobRelease->setCallback(this);

    // knob Threshold
    fKnobThreshold = new ImageKnob(this, knobImage);
    fKnobThreshold->setPos(235, 213);
    fKnobThreshold->setRange(-60.0f, 0.0f);
    fKnobThreshold->setValue(0.0f);
    fKnobThreshold->setRotationAngle(270);
    fKnobThreshold->setCallback(this);

    // knob Ratio
    fKnobRatio = new ImageKnob(this, knobImage);
    fKnobRatio->setPos(334, 213);
    fKnobRatio->setRange(1.0f, 10.0f);
    fKnobRatio->setValue(1.0f);
    fKnobRatio->setRotationAngle(270);
    fKnobRatio->setCallback(this);

    // knob Make-Up
    fKnobMakeup = new ImageKnob(this, knobImage);
    fKnobMakeup->setPos(433, 213);
    fKnobMakeup->setRange(0.0f, 20.0f);
    fKnobMakeup->setValue(0.0f);
    fKnobMakeup->setRotationAngle(270);
    fKnobMakeup->setCallback(this);

    // knob Mix
    fKnobMix = new ImageKnob(this, knobImage);
    fKnobMix->setPos(532, 213);
    fKnobMix->setRange(0.0f, 1.0f);
    fKnobMix->setValue(1.0f);
    fKnobMix->setRotationAngle(270);
    fKnobMix->setCallback(this);

    // about button
    Image aboutImageNormal(PowerJuiceArtwork::aboutButtonNormalData, PowerJuiceArtwork::aboutButtonNormalWidth, PowerJuiceArtwork::aboutButtonNormalHeight);
    Image aboutImageHover(PowerJuiceArtwork::aboutButtonHoverData, PowerJuiceArtwork::aboutButtonHoverWidth, PowerJuiceArtwork::aboutButtonHoverHeight);
    fButtonAbout = new ImageButton(this, aboutImageNormal, aboutImageHover, aboutImageHover);
    fButtonAbout->setPos(502, 17);
    fButtonAbout->setCallback(this);

	for (int i=0; i<563; i++) {
		input.push_back(0.0f);
	}

	shm_obj = new boost::interprocess::shared_memory_object(boost::interprocess::open_only,
													"sharedmemory",
													boost::interprocess::read_only);
	//shm_obj->truncate(sizeof(float)*563);
	region = new boost::interprocess::mapped_region(*shm_obj, boost::interprocess::read_only);
}

PowerJuiceUI::~PowerJuiceUI()
{
    delete fKnobAttack;
    delete fKnobRelease;
    delete fKnobThreshold;
    delete fKnobRatio;
    delete fKnobMakeup;
    delete fKnobMix;
    delete fButtonAbout;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void PowerJuiceUI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case PowerJuicePlugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    case PowerJuicePlugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case PowerJuicePlugin::paramThreshold:
        fKnobThreshold->setValue(value);
        break;
    case PowerJuicePlugin::paramRatio:
        fKnobRatio->setValue(value);
        break;
    case PowerJuicePlugin::paramMakeup:
        fKnobMakeup->setValue(value);
        break;
    case PowerJuicePlugin::paramMix:
        fKnobMix->setValue(value);
        break;
    }
}

void PowerJuiceUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(20.0f);
    fKnobRelease->setValue(200.0f);
    fKnobThreshold->setValue(0.0f);
    fKnobRatio->setValue(1.0f);
    fKnobMakeup->setValue(0.0f);
    fKnobMix->setValue(1.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void PowerJuiceUI::imageButtonClicked(ImageButton* button, int)
{
    if (button != fButtonAbout)
        return;

    fAboutWindow.exec();
}

void PowerJuiceUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(PowerJuicePlugin::paramAttack, true);
    else if (knob == fKnobRelease)
        d_editParameter(PowerJuicePlugin::paramRelease, true);
    else if (knob == fKnobThreshold)
        d_editParameter(PowerJuicePlugin::paramThreshold, true);
    else if (knob == fKnobRatio)
        d_editParameter(PowerJuicePlugin::paramRatio, true);
    else if (knob == fKnobMakeup)
        d_editParameter(PowerJuicePlugin::paramMakeup, true);
    else if (knob == fKnobMix)
        d_editParameter(PowerJuicePlugin::paramMix, true);
}

void PowerJuiceUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(PowerJuicePlugin::paramAttack, false);
    else if (knob == fKnobRelease)
        d_editParameter(PowerJuicePlugin::paramRelease, false);
    else if (knob == fKnobThreshold)
        d_editParameter(PowerJuicePlugin::paramThreshold, false);
    else if (knob == fKnobRatio)
        d_editParameter(PowerJuicePlugin::paramRatio, false);
    else if (knob == fKnobMakeup)
        d_editParameter(PowerJuicePlugin::paramMakeup, false);
    else if (knob == fKnobMix)
        d_editParameter(PowerJuicePlugin::paramMix, false);
}

void PowerJuiceUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
	if (knob == fKnobAttack)
        d_setParameterValue(PowerJuicePlugin::paramAttack, value);
    else if (knob == fKnobRelease)
        d_setParameterValue(PowerJuicePlugin::paramRelease, value);
    else if (knob == fKnobThreshold)
        d_setParameterValue(PowerJuicePlugin::paramThreshold, value);
    else if (knob == fKnobRatio)
        d_setParameterValue(PowerJuicePlugin::paramRatio, value);
    else if (knob == fKnobMakeup)
        d_setParameterValue(PowerJuicePlugin::paramMakeup, value);
    else if (knob == fKnobMix)
        d_setParameterValue(PowerJuicePlugin::paramMix, value);

}

void PowerJuiceUI::onDisplay()
{
    fImgBackground.draw();

	int w = 563; //waveform array size, size of the plane in pixels;
	int h = 60; //waveform plane height
	int x = 28; //waveform plane positions
	int y = 51;
	int dc = 113; //0DC line y position
	printf("boo\n");
	//std::memcpy(&input, region->get_address(), sizeof(float)*100);
	std::deque<float> *mem = static_cast<std::deque<float>*>(region->get_address());
	input = *mem;
	printf("la\n");
	//draw waveform
	for (int i=1; i<w; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
		glLineWidth(1.0f); 
		glBegin(GL_LINES);
			glVertex2i(x+(i-1), input[i-1]*h+dc);
			glVertex2i(x+i, input[i]*h+dc);
		glEnd();
		// reset color
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	}
	printf("na\n");
	//draw shits
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new PowerJuiceUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO