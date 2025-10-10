#include "helpers/test_helpers.h"
#include <PluginGroup.h>
#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

TEST_CASE ("Plugin instance", "[instance]")
{
    PluginProcessor testPlugin;

    SECTION ("name")
    {
        CHECK_THAT (testPlugin.getName().toStdString(),
            Catch::Matchers::Equals ("JoinForces"));
    }
}

TEST_CASE("Formats", "[formats]") {
    auto formatManager = juce::AudioPluginFormatManager();
    formatManager.addDefaultFormats();
    REQUIRE (formatManager.getNumFormats() > 0);
}

#ifdef RUN_ON_MY_COMPUTER
TEST_CASE("Plugin load dry run", "[pldr]") {
    juce::AudioPluginFormatManager pluginmanager;
    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;
    pluginmanager.addDefaultFormats();
    // the single .vst3 file may have multiple plugin types, so we need to deal with an array
    // of PluginDescriptions
    juce::OwnedArray<juce::PluginDescription> descs;
    juce::VST3PluginFormat v3format;
    v3format.findAllTypesForFile(descs,
                                 R"(C:\Program Files\Common Files\VST3\ValhallaDelay.vst3)");
    if (descs.size() > 0)
    {
        juce::String error;
        plugin_instance = pluginmanager.createPluginInstance(*descs[0], 44100, 512, error);
        if (!plugin_instance)
        {
            std::cout << error << "\n";
        }
        else
        {
            std::cout << "created " << descs[0]->descriptiveName << "\n";
        }
    }
}
#endif

TEST_CASE("Audio unchanged with no plugins loaded", "[audio unchanged]") {
    PluginProcessor testPlugin;
    testPlugin.prepareToPlay(44100, 1024);
    auto buffer = juce::AudioBuffer<float>(2, 1024);
    auto midiBuffer = juce::MidiBuffer();
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        buffer.setSample(0, i, 0.5);
    }
    testPlugin.processBlock(buffer, midiBuffer);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        REQUIRE (buffer.getSample(0, i) == 0.5);
    }
}

#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::Equals ("2022.2.0 (r0x42db1a66)"));
}
#endif
