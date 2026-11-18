#include "PluginProcessor.h"
#include "PluginEditor.h"

BlendMesh5AudioProcessorEditor::BlendMesh5AudioProcessorEditor (BlendMesh5AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1080, 680);
    logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_png, BinaryData::logo_pngSize);

    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto& st = strips[i];
        auto prefix = "slot" + juce::String (i + 1) + "_";

        addAndMakeVisible (st.loadBtn);
        st.loadBtn.onClick = [this, i]
        {
            fileChooser = std::make_unique<juce::FileChooser> (
                "Select Audio Sample for Slot " + juce::String (i + 1),
                juce::File{},
                "*.wav;*.aif;*.aiff;*.flac;*.mp3");
            fileChooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [this, i] (const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file.existsAsFile() && audioProcessor.loadSampleIntoSlot (i, file))
                        strips[i].fileLabel.setText (audioProcessor.getSlotFileName (i), juce::dontSendNotification);
                });
        };

        addAndMakeVisible (st.fileLabel);
        st.fileLabel.setText (audioProcessor.getSlotFileName (i), juce::dontSendNotification);
        st.fileLabel.setJustificationType (juce::Justification::centred);
        st.fileLabel.setColour (juce::Label::textColourId, juce::Colours::lightgrey);

        addAndMakeVisible (st.polarityBtn);
        addAndMakeVisible (st.fxSendBtn);
        st.polarityAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, prefix + "polarity", st.polarityBtn);
        st.fxSendAtt   = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, prefix + "fxsend", st.fxSendBtn);

        setupRotary (st.levelSlider);
        setupRotary (st.pitchSlider, " st");
        setupRotary (st.attackSlider, " s");
        setupRotary (st.decaySlider, " s");
        setupRotary (st.sustainSlider);
        setupRotary (st.releaseSlider, " s");

        st.levelAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "level", st.levelSlider);
        st.pitchAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "pitch", st.pitchSlider);
        st.attackAtt  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "attack", st.attackSlider);
        st.decayAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "decay", st.decaySlider);
        st.sustainAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "sustain", st.sustainSlider);
        st.releaseAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, prefix + "release", st.releaseSlider);
    }

    // Blend / Mesh Master
    setupRotary (timbreSlider);
    setupRotary (colorSlider);
    setupRotary (shapeSlider);
    setupRotary (movementSlider);
    setupRotary (masterGainSlider);

    timbreAtt     = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "blend_timbre", timbreSlider);
    colorAtt      = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "blend_color", colorSlider);
    shapeAtt      = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "blend_shape", shapeSlider);
    movementAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "blend_movement", movementSlider);
    masterGainAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "master_gain", masterGainSlider);

    addAndMakeVisible (auditionBtn);
    auditionBtn.onStateChange = [this]
    {
        audioProcessor.triggerAudition (auditionBtn.isDown());
    };

    // FX Rack
    auto addFxToggle = [this] (juce::ToggleButton& btn, const juce::String& text, const juce::String& paramId,
                               std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& att)
    {
        btn.setButtonText (text);
        addAndMakeVisible (btn);
        att = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, paramId, btn);
    };

    addFxToggle (eqOn, "EQ", "eq_on", eqOnAtt);
    addFxToggle (compOn, "COMP", "comp_on", compOnAtt);
    addFxToggle (satOn, "SAT", "sat_on", satOnAtt);
    addFxToggle (distOn, "DIST", "dist_on", distOnAtt);
    addFxToggle (phaserOn, "PHASER", "phaser_on", phaserOnAtt);
    addFxToggle (delayOn, "DELAY", "delay_on", delayOnAtt);
    addFxToggle (reverbOn, "REVERB", "reverb_on", reverbOnAtt);

    attachFxSlider (eqLow, "eq_low", " dB");
    attachFxSlider (eqMid, "eq_mid", " dB");
    attachFxSlider (eqHigh, "eq_high", " dB");

    attachFxSlider (compThresh, "comp_thresh", " dB");
    attachFxSlider (compRatio, "comp_ratio", ":1");

    attachFxSlider (satDrive, "sat_drive");
    attachFxSlider (satMix, "sat_mix");

    attachFxSlider (distDrive, "dist_drive");
    attachFxSlider (distTone, "dist_tone", " Hz");
    attachFxSlider (distMix, "dist_mix");

    attachFxSlider (phaserRate, "phaser_rate", " Hz");
    attachFxSlider (phaserDepth, "phaser_depth");
    attachFxSlider (phaserMix, "phaser_mix");

    attachFxSlider (delayTime, "delay_time", " ms");
    attachFxSlider (delayFb, "delay_fb");
    attachFxSlider (delayMix, "delay_mix");

    attachFxSlider (reverbSize, "reverb_size");
    attachFxSlider (reverbDamp, "reverb_damp");
    attachFxSlider (reverbMix, "reverb_mix");
}

BlendMesh5AudioProcessorEditor::~BlendMesh5AudioProcessorEditor() = default;

void BlendMesh5AudioProcessorEditor::setupRotary (juce::Slider& s, const juce::String& suffix)
{
    s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 58, 16);
    if (suffix.isNotEmpty())
        s.setTextValueSuffix (suffix);
    addAndMakeVisible (s);
}

void BlendMesh5AudioProcessorEditor::attachFxSlider (juce::Slider& s, const juce::String& paramId, const juce::String& suffix)
{
    setupRotary (s, suffix);
    fxSliderAtts.push_back (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, paramId, s));
}

void BlendMesh5AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff16181f));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f, juce::Font::bold));
    if (logoImage.isValid())
        g.drawImageWithin (logoImage, 14, 4, 38, 38, juce::RectanglePlacement::centred);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (juce::FontOptions ("Segoe UI Symbol", 19.0f, juce::Font::bold)));
    const juce::String mathTitle (juce::CharPointer_UTF8 ("\xe2\x8a\x99 \xe2\x88\xab \xe2\x8a\x99 \xe2\x8a\xa4 \xe2\x84\x9d \xe2\x88\xaa \xc3\x97 \xe2\x88\x83"));
    g.drawText (mathTitle, 58, 4, 360, 22, juce::Justification::left);

    g.setColour (juce::Colour (0xffffd166));
    g.setFont (juce::FontOptions (12.0f, juce::Font::bold));
    g.drawText ("SnoopyChu  |  5-Layer Timbre & FX Synthesizer", 58, 25, 420, 16, juce::Justification::left);

    g.setColour (juce::Colour (0xff222631));
    g.fillRoundedRectangle (12, 46, getWidth() - 24, 300, 8.0f);
    g.fillRoundedRectangle (12, 356, getWidth() - 24, 110, 8.0f);
    g.fillRoundedRectangle (12, 476, getWidth() - 24, 192, 8.0f);

    g.setColour (juce::Colour (0xff8fa3bf));
    g.setFont (12.0f);
    g.drawText ("TIMBRE        COLOR         SHAPE       MOVEMENT      MASTER", 24, 362, 540, 18, juce::Justification::left);
    g.drawText ("SELECTIVE FX RACK (Processes samples with FX Send enabled)", 24, 480, 500, 18, juce::Justification::left);
}

void BlendMesh5AudioProcessorEditor::resized()
{
    const int slotW = (getWidth() - 48) / NUM_SLOTS;
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto& st = strips[i];
        int x = 20 + i * (slotW + 2);
        st.loadBtn.setBounds (x + 6, 54, slotW - 12, 24);
        st.fileLabel.setBounds (x + 4, 80, slotW - 8, 18);
        st.polarityBtn.setBounds (x + 6, 102, 90, 22);
        st.fxSendBtn.setBounds (x + 100, 102, 90, 22);

        st.levelSlider.setBounds   (x + 8,   132, 86, 66);
        st.pitchSlider.setBounds   (x + 102, 132, 86, 66);
        st.attackSlider.setBounds  (x + 8,   204, 86, 66);
        st.decaySlider.setBounds   (x + 102, 204, 86, 66);
        st.sustainSlider.setBounds (x + 8,   274, 86, 66);
        st.releaseSlider.setBounds (x + 102, 274, 86, 66);
    }

    timbreSlider.setBounds     (24,  382, 92, 76);
    colorSlider.setBounds      (124, 382, 92, 76);
    shapeSlider.setBounds      (224, 382, 92, 76);
    movementSlider.setBounds   (324, 382, 92, 76);
    masterGainSlider.setBounds (424, 382, 92, 76);
    auditionBtn.setBounds      (getWidth() - 180, 396, 150, 38);

    // FX Rack 7 modules across bottom
    const int fxY = 504;
    eqOn.setBounds (24, fxY, 70, 22);
    eqLow.setBounds (20, fxY + 24, 56, 64);
    eqMid.setBounds (76, fxY + 24, 56, 64);
    eqHigh.setBounds (132, fxY + 24, 56, 64);

    compOn.setBounds (200, fxY, 80, 22);
    compThresh.setBounds (196, fxY + 24, 58, 64);
    compRatio.setBounds (256, fxY + 24, 58, 64);

    satOn.setBounds (326, fxY, 70, 22);
    satDrive.setBounds (322, fxY + 24, 58, 64);
    satMix.setBounds (382, fxY + 24, 58, 64);

    distOn.setBounds (452, fxY, 70, 22);
    distDrive.setBounds (448, fxY + 24, 56, 64);
    distTone.setBounds (504, fxY + 24, 56, 64);
    distMix.setBounds (560, fxY + 24, 56, 64);

    phaserOn.setBounds (628, fxY, 80, 22);
    phaserRate.setBounds (624, fxY + 24, 56, 64);
    phaserDepth.setBounds (680, fxY + 24, 56, 64);
    phaserMix.setBounds (736, fxY + 24, 56, 64);

    delayOn.setBounds (804, fxY, 75, 22);
    delayTime.setBounds (800, fxY + 24, 56, 64);
    delayFb.setBounds (856, fxY + 24, 56, 64);
    delayMix.setBounds (912, fxY + 24, 56, 64);

    reverbOn.setBounds (978, fxY, 80, 22);
    reverbSize.setBounds (974, fxY + 24, 48, 64);
    reverbDamp.setBounds (1022, fxY + 24, 48, 64);
    reverbMix.setBounds (974, fxY + 94, 96, 64);
}
