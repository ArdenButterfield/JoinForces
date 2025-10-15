//
// Created by Arden on 2025-10-14.
//

#include "SubpluginWindow.h"

SubpluginWindow::SubpluginWindow(juce::AudioProcessor& ap) :
    DocumentWindow(ap.getName(), juce::Colours::black, juce::DocumentWindow::allButtons),
    audioProcessor(ap) {

    setSize (400, 300);

    auto* ui = audioProcessor.createEditorIfNeeded();
    setContentOwned (ui, true);
    setResizable (ui->isResizable(), false);
}

SubpluginWindow::~SubpluginWindow() {
    std::cout << "byebye\n";
}
