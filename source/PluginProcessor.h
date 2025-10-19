#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginGroup.h"
#include "MappingCenter.h"
#include "ForceFeedbackInterface.h"

#if (MSVC)
#include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    PluginGroup* getPluginGroup() const {
        return pluginGroup.get();
    }

    MappingCenter* getMappingCenter() const {
        return mappingCenter.get();
    }

    ForceFeedbackInterface* getForceFeedbackInterface() const {
        return forceFeedbackInterface.get();
    }

private:
    std::unique_ptr<MappingCenter> mappingCenter;
    std::unique_ptr<PluginGroup> pluginGroup;
    std::unique_ptr<ForceFeedbackInterface> forceFeedbackInterface;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
