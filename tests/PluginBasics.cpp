#include "helpers/test_helpers.h"
#include <PluginGroup.h>
#include <PluginProcessor.h>
#include <PluginEditor.h>
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

#define RUN_ON_MY_COMPUTER
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

TEST_CASE("Import", "[import]") {
    auto str = R"(<?xml version="1.0" encoding="UTF-8"?>

<state>
  <mapping>
    <current-state>
      <position x="0.0" y="0.0" z="0.0"/>
      <plugins>
        <plugin path="C:\Program Files\Common Files\VST3\ValhallaDelay.vst3">
          <params param_0="0.5" param_1="0.0" param_2="0.25" param_3="0.2000000029802322"
                  param_4="0.300000011920929" param_5="0.25" param_6="0.2000000029802322"
                  param_7="0.300000011920929" param_8="0.5" param_9="0.5" param_10="0.6141414046287537"
                  param_11="1.0" param_12="1.0" param_13="1.0" param_14="1.0" param_15="1.0"
                  param_16="0.3499999940395355" param_17="1.0" param_18="0.0" param_19="0.5"
                  param_20="0.0" param_21="1.0" param_22="0.0" param_23="1.0" param_24="0.2738341093063354"
                  param_25="0.5" param_26="0.1842105239629745" param_27="0.5" param_28="0.5"
                  param_29="0.5799999833106995" param_30="0.5" param_31="0.5" param_32="0.0416666679084301"
                  param_33="0.01315789483487606" param_34="0.0" param_35="0.0"
                  param_36="0.0" param_37="0.0" param_38="0.0"/>
        </plugin>
        <plugin path="C:\Program Files\Common Files\VST3\kiloHearts\kHs Comb Filter.vst3">
          <params param_0="0.0" param_1="0.4459431767463684" param_2="1.0" param_3="1.0"
                  param_4="0.0"/>
        </plugin>
      </plugins>
    </current-state>
    <map-positions>
      <map-point>
        <position x="0.0" y="0.0" z="0.0"/>
        <plugins>
          <plugin path="C:\Program Files\Common Files\VST3\ValhallaDelay.vst3">
            <params param_0="0.5" param_1="0.0" param_2="0.25" param_3="0.2000000029802322"
                    param_4="0.300000011920929" param_5="0.25" param_6="0.2000000029802322"
                    param_7="0.300000011920929" param_8="0.5" param_9="0.5" param_10="0.6141414046287537"
                    param_11="1.0" param_12="1.0" param_13="1.0" param_14="1.0" param_15="1.0"
                    param_16="0.3499999940395355" param_17="1.0" param_18="0.0" param_19="0.5"
                    param_20="0.0" param_21="1.0" param_22="0.0" param_23="1.0" param_24="0.2738341093063354"
                    param_25="0.5" param_26="0.1842105239629745" param_27="0.5" param_28="0.5"
                    param_29="0.5799999833106995" param_30="0.5" param_31="0.5" param_32="0.0416666679084301"
                    param_33="0.01315789483487606" param_34="0.0" param_35="0.0"
                    param_36="0.0" param_37="0.0" param_38="0.0"/>
          </plugin>
          <plugin path="C:\Program Files\Common Files\VST3\kiloHearts\kHs Comb Filter.vst3">
            <params param_0="0.0" param_1="0.4459431767463684" param_2="1.0" param_3="1.0"
                    param_4="0.0"/>
          </plugin>
        </plugins>
      </map-point>
      <map-point>
        <position x="0.0" y="0.0" z="0.0"/>
        <plugins>
          <plugin path="C:\Program Files\Common Files\VST3\ValhallaDelay.vst3">
            <params param_0="0.5" param_1="0.0" param_2="0.25" param_3="0.2000000029802322"
                    param_4="0.300000011920929" param_5="0.25" param_6="0.2000000029802322"
                    param_7="0.300000011920929" param_8="0.5" param_9="0.5" param_10="0.6141414046287537"
                    param_11="1.0" param_12="1.0" param_13="1.0" param_14="1.0" param_15="1.0"
                    param_16="0.3499999940395355" param_17="1.0" param_18="0.0" param_19="0.5"
                    param_20="0.4736842215061188" param_21="1.0" param_22="0.4736842215061188"
                    param_23="1.0" param_24="0.2738341093063354" param_25="0.5" param_26="0.8552631735801697"
                    param_27="0.5" param_28="0.5" param_29="0.5799999833106995" param_30="0.5"
                    param_31="0.5" param_32="0.0416666679084301" param_33="0.8815789222717285"
                    param_34="0.0" param_35="0.0" param_36="0.0" param_37="0.0" param_38="0.0"/>
          </plugin>
          <plugin path="C:\Program Files\Common Files\VST3\kiloHearts\kHs Comb Filter.vst3">
            <params param_0="0.0" param_1="0.4459431767463684" param_2="1.0" param_3="0.0"
                    param_4="0.0"/>
          </plugin>
        </plugins>
      </map-point>
    </map-positions>
  </mapping>
</state>
)";

    PluginProcessor testPlugin;
    auto element = juce::XmlDocument::parse(str);
    testPlugin.prepareToPlay(44100, 1024);
    testPlugin.getMappingCenter()->importFromXml(*element);
    auto buffer = juce::AudioBuffer<float>(2, 1024);
    auto midiBuffer = juce::MidiBuffer();
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        buffer.setSample(0, i, 0.5);
    }
    testPlugin.processBlock(buffer, midiBuffer);

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

TEST_CASE("Mappings", "[mappings]") {
    PluginProcessor testPlugin;
    testPlugin.prepareToPlay(44100, 1024);
    auto editor = static_cast<PluginEditor*>(testPlugin.createEditor());
    for (int iter = 0; iter < 20; ++iter) {
        auto buffer = juce::AudioBuffer<float>(2, 1024);
        auto midiBuffer = juce::MidiBuffer();
        for (int i = 0; i < 2000; ++i) {
            testPlugin.processBlock(buffer, midiBuffer);
        }
        editor->createMappingButton.triggerClick();
    }
}

TEST_CASE("Normalizable range", "[normrange]")
{
    auto n = juce::NormalisableRange<float>(0, 12, 1);
    auto a = n.snapToLegalValue ( n.convertFrom0to1 (0.5f/12.f - 0.001f));
    auto b = n.snapToLegalValue ( n.convertFrom0to1 (0.5f/12.f + 0.001f));
    auto c = n.snapToLegalValue ( n.convertFrom0to1 (1.5f/12.f - 0.001f));
    auto d = n.snapToLegalValue ( n.convertFrom0to1 (1.5f/12.f + 0.001f));
    REQUIRE( a == 0 );
    REQUIRE( b == 1 );
    REQUIRE( c == 1 );
    REQUIRE( d == 2 );
}

#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::Equals ("2022.2.0 (r0x42db1a66)"));
}
#endif
