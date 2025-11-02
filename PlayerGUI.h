#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"


class MarkerListBoxModel : public juce::ListBoxModel
{
public:
    MarkerListBoxModel(PlayerAudio& audio, juce::ListBox& lb, juce::TextButton& delBtn);
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;

private:
    PlayerAudio& playerAudio;
    juce::ListBox& listBox;
    juce::TextButton& deleteButton;
    juce::String getTimeString(double seconds);
};


class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
    public juce::Timer
{
public:
	PlayerGUI();
	~PlayerGUI() override;
	void resized() override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void paint(juce::Graphics& g) override;
private:
	PlayerAudio playerAudio;
	juce::TextButton loadButton{ "LOAD FILE "};
	juce::TextButton restartButton{ "RESART"};
	juce::TextButton stopButton{ "STOP"};
	juce::TextButton replayButton{ "REPLAY"};
	juce::TextButton playPauseButton{ "PAUSE" };
	juce::TextButton goToStartButton{ "START" };
	juce::TextButton goToEndButton{ "END" };
	juce::TextButton setAButton{ "Set A" };
	juce::TextButton setBButton{ "Set B" };
	juce::TextButton toggleLoopButton{ "LOOP OFF" };
    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::TextButton deleteMarkerButton{ "Delete Marker" };
    juce::ListBox markerListBox;
    MarkerListBoxModel markerListModel;
	juce::Slider volumeSlider;
	juce::Slider positionSlider;
	juce::Label positionLabel;
	std::unique_ptr<juce::FileChooser> fileChooser;
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void timerCallback() override;
	juce::String getTimeString(double seconds);
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};