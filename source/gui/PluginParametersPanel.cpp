//
// Created by Arden on 2025-10-12.
//

#include "PluginParametersPanel.h"

PluginParametersPanel::PluginParametersPanel(MappingCenter::PluginParameterSet& pps) :
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
    for (int i = 0; i < parameterRows.size(); i++) {
        parameterRows[i]->setBounds(getLocalBounds()
            .withHeight(parameterRowHeight)
            .withY(parameterRowHeight * i)
            .withTrimmedLeft(15));
    }

    if (openEditorButton.isVisible()) {
        openEditorButton.setBounds(getLocalBounds().withHeight(10).withWidth(10).withBottomY(getHeight()));
    }
}

void PluginParametersPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkblue);
    g.setColour(juce::Colours::lightblue);
    g.drawRect(getLocalBounds(), 3);
}

void PluginParametersPanel::updateDisplay() {
    for (auto& row : parameterRows) {
        removeChildComponent(row.get());
    }

    parameterRows.clear();

    for (auto& param : parameterSet.parameters) {
        parameterRows.push_back(std::make_unique<ParameterRow>(param));
    }

    for (auto& param : parameterSet.processor->getParameters()) {
        if (param->isAutomatable()) {
            parameterRows.push_back(std::make_unique<ParameterRow>(*processor->getProcessor(), *param));
        }
    }
    for (auto& row : parameterRows) {
        addAndMakeVisible(row.get());
    }
    resized();
}

int PluginParametersPanel::getDesiredHeight() const {
    return static_cast<int>(parameterRows.size()) * parameterRowHeight;
}
