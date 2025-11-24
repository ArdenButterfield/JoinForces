//
// Created by Arden on 2025-11-16.
//

#include "ForceAmountSliders.h"

#include "../MappingCenter.h"
#include "JoinForcesLookFeel.h"

ForceAmountSliders::ForceAmountSliders(MappingCenter& mc) : mappingCenter(mc)
{
    for (auto label : {&pointForceLabel, &wallForceLabel}) {
        addAndMakeVisible (label);
        label->setColour (juce::Label::textColourId, JoinForcesLookFeel::getTextColour());
        label->setJustificationType (juce::Justification::centred);
    }

    for (auto slider : {&amountPointForceSlider, &amountWallForceSlider}) {
        addAndMakeVisible (slider);

        slider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        slider->addListener (this);
        slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);

        slider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        slider->setColour (juce::Slider::textBoxTextColourId, JoinForcesLookFeel::getTextColour());
        slider->setColour (juce::Slider::thumbColourId, JoinForcesLookFeel::getBorderColour());
        slider->setColour (juce::Slider::trackColourId, JoinForcesLookFeel::getBorderColour().withAlpha (0.2f));
    }

    pointForceLabel.setText ("Point Force", juce::dontSendNotification);
    wallForceLabel.setText ("Wall Force", juce::dontSendNotification);

    amountPointForceSlider.setRange (
        mappingCenter.amountOfPointFeedback->range.start,
        mappingCenter.amountOfPointFeedback->range.end);
    amountWallForceSlider.setRange (
        mappingCenter.amountOfWallFeedback->range.start,
        mappingCenter.amountOfWallFeedback->range.end);

    startTimerHz (30);
}
ForceAmountSliders::~ForceAmountSliders()
{
}
void ForceAmountSliders::resized()
{
    auto halfwidth = getWidth() / 2;
    pointForceLabel.setBounds (getLocalBounds().withWidth (halfwidth).withHeight (40));
    wallForceLabel.setBounds (pointForceLabel.getBounds().withRightX (getWidth()));
    amountPointForceSlider.setBounds(getLocalBounds().withWidth (halfwidth).withTop (pointForceLabel.getBottom()));
    amountWallForceSlider.setBounds (amountPointForceSlider.getBounds().withRightX (getWidth()));
}

void ForceAmountSliders::timerCallback()
{
    amountWallForceSlider.setValue (*mappingCenter.amountOfWallFeedback);
    amountPointForceSlider.setValue (*mappingCenter.amountOfPointFeedback);
}

void ForceAmountSliders::paint (juce::Graphics& g)
{
    g.setColour (JoinForcesLookFeel::getMidColour());
    g.drawRect (getLocalBounds());
}
void ForceAmountSliders::sliderValueChanged (juce::Slider*)
{
    *mappingCenter.amountOfPointFeedback = amountPointForceSlider.getValue();
    *mappingCenter.amountOfWallFeedback = amountWallForceSlider.getValue();
}

void ForceAmountSliders::sliderDragEnded (juce::Slider* slider)
{
}
void ForceAmountSliders::sliderDragStarted (juce::Slider* slider)
{
}