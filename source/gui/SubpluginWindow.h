//
// Created by Arden on 2025-10-14.
//

#ifndef JOINFORCES_SUBPLUGINWINDOW_H
#define JOINFORCES_SUBPLUGINWINDOW_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

class SubpluginWindow : public juce::DocumentWindow {
public:
    SubpluginWindow(juce::AudioProcessor& ap);
    ~SubpluginWindow() override;
private:
    juce::AudioProcessor& audioProcessor;
};


#endif //JOINFORCES_SUBPLUGINWINDOW_H