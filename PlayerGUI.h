#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"


class PlayerGUI;


class MarkerListBoxModel : public juce::ListBoxModel
{
public:
    MarkerListBoxModel(PlayerAudio& audio, juce::ListBox& lb, juce::TextButton& delBtn)
        : playerAudio(audio), listBox(lb), deleteButton(delBtn)
    {
        deleteButton.setEnabled(false);
    }

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;
    juce::String getTimeString(double seconds);

private:
    PlayerAudio& playerAudio;
    juce::ListBox& listBox;
    juce::TextButton& deleteButton;
};

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI(PlayerAudio& player);
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    // JUCE Listener Callbacks
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    juce::String getTimeString(double seconds);

    PlayerAudio& audioPlayer;

    // Buttons
    juce::TextButton loadButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton replayButton;
    juce::TextButton goToStartButton;
    juce::TextButton goToEndButton;
    juce::TextButton muteButton;
    juce::TextButton forwardButton;
    juce::TextButton backwardButton;
    juce::TextButton toggleLoopButton;
    juce::TextButton setAButton;
    juce::TextButton setBButton;
    juce::TextButton addMarkerButton;
    juce::TextButton deleteMarkerButton;

    // Sliders
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    // Labels
    juce::Label positionLabel;
    juce::Label infoLabel;

    // Marker List
    juce::ListBox markerListBox;
    MarkerListBoxModel markerListModel;

    float lastVolumeValue = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};