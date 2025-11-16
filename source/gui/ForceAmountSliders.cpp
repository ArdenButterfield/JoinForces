//
// Created by Arden on 2025-11-16.
//

#include "ForceAmountSliders.h"
#include "../MappingCenter.h"

ForceAmountSliders::ForceAmountSliders(MappingCenter& mc) : mappingCenter(mc)
{
    addAndMakeVisible (amountPointForceSlider);
    addAndMakeVisible (amountWallForceSlider);
    addAndMakeVisible (pointForceLabel);
    addAndMakeVisible (wallForceLabel);

    amountWallForceSlider.addListener (this);
    amountPointForceSlider.addListener (this);

    pointForceLabel.setText ("Point Force", juce::dontSendNotification);
    wallForceLabel.setText ("Wall Force", juce::dontSendNotification);

    pointForceLabel.setJustificationType (juce::Justification::centred);
    wallForceLabel.setJustificationType (juce::Justification::centred);

    amountPointForceSlider.setRange (
        mappingCenter.amountOfPointFeedback->range.start,
        mappingCenter.amountOfPointFeedback->range.end);
    amountWallForceSlider.setRange (
        mappingCenter.amountOfWallFeedback->range.start,
        mappingCenter.amountOfWallFeedback->range.end);

    amountPointForceSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    amountWallForceSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);

    amountPointForceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    amountWallForceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);

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
    g.setColour (juce::Colours::white);
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