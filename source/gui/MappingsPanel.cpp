//
// Created by Arden on 2025-10-12.
//

#include "MappingsPanel.h"
#include "../MappingCenter.h"

MappingsPanel::MappingsPanel(MappingCenter& mc) : group(mc.group) {
    currentStateColumn = std::make_unique<CurrentStateColumn>(mc);
    addAndMakeVisible(currentStateColumn.get());
    setSize(400,400);
}

MappingsPanel::~MappingsPanel() {

}

void MappingsPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 5, 3);
}

void MappingsPanel::resized() {
    currentStateColumn->setBounds(getLocalBounds().withTrimmedLeft(10).withTrimmedTop(10).withTrimmedBottom(10).withWidth(100));
}