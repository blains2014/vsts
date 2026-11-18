#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BlendMesh5AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit BlendMesh5AudioProcessorEditor (BlendMesh5AudioProcessor&);
    ~BlendMesh5AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BlendMesh5AudioProcessor& audioProcessor;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Image logoImage;

    struct SlotStrip
    {
        juce::TextButton loadBtn { "Load" };
        juce::Label fileLabel;
        juce::ToggleButton polarityBtn { "Invert Ø" };
        juce::ToggleButton fxSendBtn   { "FX Send" };

        juce::Slider levelSlider, pitchSlider;
        juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> polarityAtt, fxSendAtt;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAtt, pitchAtt;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAtt, decayAtt, sustainAtt, releaseAtt;
    };

    std::array<SlotStrip, NUM_SLOTS> strips;

    // Master Blend Section
    juce::Slider timbreSlider, colorSlider, shapeSlider, movementSlider, masterGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timbreAtt, colorAtt, shapeAtt, movementAtt, masterGainAtt;
    juce::TextButton auditionBtn { "Hold to Audition" };

    // FX Rack Controls
    juce::ToggleButton eqOn, compOn, satOn, distOn, phaserOn, delayOn, reverbOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnAtt, compOnAtt, satOnAtt, distOnAtt, phaserOnAtt, delayOnAtt, reverbOnAtt;

    juce::Slider eqLow, eqMid, eqHigh;
    juce::Slider compThresh, compRatio;
    juce::Slider satDrive, satMix;
    juce::Slider distDrive, distTone, distMix;
    juce::Slider phaserRate, phaserDepth, phaserMix;
    juce::Slider delayTime, delayFb, delayMix;
    juce::Slider reverbSize, reverbDamp, reverbMix;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> fxSliderAtts;

    void setupRotary (juce::Slider& s, const juce::String& suffix = {});
    void attachFxSlider (juce::Slider& s, const juce::String& paramId, const juce::String& suffix = {});

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlendMesh5AudioProcessorEditor)
};
