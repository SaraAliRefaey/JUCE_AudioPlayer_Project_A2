#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;
	void resized() override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void paint(juce::Graphics& g) override;
 void buttonClicked(juce::Button* button) override;
 void sliderValueChanged(juce::Slider* slider) override;
	void infolabelSet(const juce::String& infoText);

private:
	PlayerAudio playerAudio;
	// GUI elements
	juce::TextButton loadButton{ "Load File "};
	juce::TextButton restartButton{ "Restart"};
	juce::TextButton stopButton{ "Stop"};
	juce::TextButton playPauseButton{ "Pause" };
	juce::TextButton goToStartButton{ "Start" };
	juce::TextButton goToEndButton{ "End" };

	juce::Slider volumeSlider;
	std::unique_ptr<juce::FileChooser> fileChooser;
	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
juce::TextButton forwardButton{ "Skip Forward 10s" }, backwardButton{ "Skip Backward 10s" };
juce::Label infolabel;
bool isMuted = false;
float previousGain = 0.5f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
