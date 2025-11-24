//
// Created by Arden on 2025-11-02.
//

#include "ParameterNamesColumn.h"
#include "../MappingCenter.h"
#include "JoinForcesLookFeel.h"
#include "juce_dsp/maths/juce_FastMathApproximations.h"

ParameterNamesColumn::ParameterNamesColumn(MappingCenter& mc) : mappingCenter(mc) {
    startTimerHz(60);
}

ParameterNamesColumn::~ParameterNamesColumn() {
}

void ParameterNamesColumn::resized() {

}

void ParameterNamesColumn::paint(juce::Graphics &g) {
    g.fillAll(JoinForcesLookFeel::getBackgroundColour());
    g.setColour(JoinForcesLookFeel::getTextColour());
    auto y = JoinForcesLookFeel::getPositionVizHeight();
    auto currentMapping = mappingCenter.getCurrentMapping();
    auto textBounds = juce::Rectangle<int>(
        JoinForcesLookFeel::getDefaultMargin(), 0,
        getWidth() - 2 * JoinForcesLookFeel::getDefaultMargin(),
        JoinForcesLookFeel::getParameterRowHeight());

    for (auto& plugin : currentMapping.pluginParameters) {
        int yy = 0;
        auto name = plugin.processor.getName();
        juce::GlyphArrangement ga;
        ga.addLineOfText (juce::Font(juce::FontOptions(14.f)), name, 0, 0);
        juce::Path p;
        ga.createPath (p);

        auto pathBounds = p.getBounds();

        p.applyTransform(juce::AffineTransform()
                         .rotated(juce::MathConstants<float>::pi * 0.5f,
                                  pathBounds.getCentreX(),
                                  pathBounds.getCentreY())
                         .translated(textBounds.getX() - pathBounds.getCentreX() + pathBounds.getHeight() / 2,
                             y + pathBounds.getWidth() / 2 + 10)
                         );


        g.fillPath (p);
        for (auto& parameter : plugin.parameters) {
            g.drawText(parameter.parameter.getName(1024), textBounds.withY(y + yy), juce::Justification::right);
            yy += JoinForcesLookFeel::getParameterRowHeight();
        }
        y += JoinForcesLookFeel::getProcessorRequiredHeight(plugin);
        g.drawHorizontalLine (y, textBounds.getX(), textBounds.getRight());
    }
}

void ParameterNamesColumn::timerCallback() {
    auto h = JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter);
    if (h != getHeight()) {
        setSize(JoinForcesLookFeel::getNamesColumnWidth(), h);
    }
}
