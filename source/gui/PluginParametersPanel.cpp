//
// Created by Arden on 2025-10-12.
//

#include "PluginParametersPanel.h"
#include "JoinForcesLookFeel.h"

PluginParametersPanel::PluginParametersPanel(PluginParameterSet& pps) :
    parameterSet(pps) {
    updateDisplay();

    if (parameterSet.processor.hasEditor()) {
        addAndMakeVisible(openEditorButton);
        openEditorButton.onClick = [&] {
            if (parameterSet.processor.hasEditor()) {
                subpluginWindow = std::make_unique<SubpluginWindow>(parameterSet.processor);
                addAndMakeVisible(subpluginWindow.get());
                subpluginWindow->setVisible(true);
            }
        };
    }
}

PluginParametersPanel::~PluginParametersPanel() = default;

void PluginParametersPanel::resized() {
    int y = 0;
    for (int i = 0; i < parameterRows.size(); i++) {
        parameterRows[i]->setTopLeftPosition(0, y);
        y += parameterRows[i]->getHeight();
    }

    // if (openEditorButton.isVisible()) {
    //     openEditorButton.setBounds(getLocalBounds().withHeight(10).withWidth(10).withBottomY(getHeight()));
    // }
}

void PluginParametersPanel::paint(juce::Graphics &g) {
    g.setColour(JoinForcesLookFeel::getMidColour());
    g.drawRect(getLocalBounds(), 1);
}

void PluginParametersPanel::updateDisplay() {
    for (auto& row : parameterRows) {
        removeChildComponent(row.get());
    }

    parameterRows.clear();

    for (auto& param : parameterSet.parameters) {
        parameterRows.push_back(std::make_unique<ParameterRow>(param));
    }

    for (auto& row : parameterRows) {
        addAndMakeVisible(row.get());
    }

    setSize(JoinForcesLookFeel::getColumnWidth(),
        JoinForcesLookFeel::getProcessorRequiredHeight(parameterSet));
}