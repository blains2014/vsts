#pragma once

#include <JuceHeader.h>
#include <array>
#include <atomic>

static constexpr int NUM_SLOTS = 5;

struct SampleSlot
{
    juce::AudioBuffer<float> buffer;
    double sourceSampleRate = 44100.0;
    juce::String fileName { "Empty Slot" };
    bool loaded = false;

    // Playback state (monophonic/retriggered blend voice per slot)
    double playPosition = 0.0;
    bool noteActive = false;
    float velocity = 1.0f;
    int currentMidiNote = 60;
    juce::ADSR adsr;
};

class BlendMesh5AudioProcessor : public juce::AudioProcessor
{
public:
    BlendMesh5AudioProcessor();
    ~BlendMesh5AudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void getStateInformation (juce::MemoryBlock& destData, bool) { getStateInformation(destData); }
    void setStateInformation (const void* data, int sizeInBytes) override;

    bool loadSampleIntoSlot (int slotIndex, const juce::File& file);
    juce::String getSlotFileName (int slotIndex) const;
    bool isSlotLoaded (int slotIndex) const;
    void triggerAudition (bool noteOn);

    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    std::array<SampleSlot, NUM_SLOTS> slots;
    juce::AudioFormatManager formatManager;
    juce::CriticalSection sampleLock;

    std::atomic<bool> auditionPendingOn { false };
    std::atomic<bool> auditionPendingOff { false };

    double currentSampleRate = 44100.0;
    double movementPhase = 0.0;

    // Color & Timbre DSP
    juce::dsp::IIR::Filter<float> colorLowShelfL, colorLowShelfR;
    juce::dsp::IIR::Filter<float> colorHighShelfL, colorHighShelfR;

    // FX Rack DSP
    // 1. EQ (3-band)
    juce::dsp::IIR::Filter<float> eqLowL, eqLowR;
    juce::dsp::IIR::Filter<float> eqMidL, eqMidR;
    juce::dsp::IIR::Filter<float> eqHighL, eqHighR;

    // 2. Compressor
    juce::dsp::Compressor<float> compressor;

    // 3. Distortion post-tone filter
    juce::dsp::IIR::Filter<float> distToneL, distToneR;

    // 4. Phaser
    juce::dsp::Phaser<float> phaser;

    // 5. Stereo Delay
    static constexpr int maxDelaySamples = 192000;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLineL { maxDelaySamples };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLineR { maxDelaySamples };
    float delayFeedbackL = 0.0f;
    float delayFeedbackR = 0.0f;

    // 6. Reverb
    juce::dsp::Reverb reverb;

    juce::AudioBuffer<float> fxBusBuffer;
    juce::AudioBuffer<float> dryBusBuffer;

    void updateFilters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlendMesh5AudioProcessor)
};
