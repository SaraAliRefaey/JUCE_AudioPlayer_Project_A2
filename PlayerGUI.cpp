#include"PlayerGUI.h"
#include "PlayerAudio.h"
#include <JuceHeader.h>

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()

{
	playerAudio.releaseResources();
}
void PlayerGUI::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::darkgrey);
}

PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &restartButton , & stopButton , &playPauseButton, &goToStartButton,&goToEndButton})
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}
    addAndMakeVisible(playPauseButton);
    playPauseButton.setButtonText("Play/Pause");
    playPauseButton.addListener(this);
    addAndMakeVisible(goToStartButton);
    goToStartButton.setButtonText("start");
    goToStartButton.addListener(this);
    addAndMakeVisible(goToEndButton);
    goToEndButton.setButtonText("End");
    goToEndButton.addListener(this);
	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);

	 addAndMakeVisible(muteButton);
  addAndMakeVisible(infolabel);
	 muteButton.setButtonText("Mute");
	  addAndMakeVisible(forwardButton);
  addAndMakeVisible(backwardButton);
  forwardButton.onClick = [this] { audioPlayer.skipForward(); };
  backwardButton.onClick = [this] { audioPlayer.skipBackward(); };

	setSize(500, 250);
 }

void PlayerGUI::resized()
{
	int y = 20;
	loadButton.setBounds(20, y, 100, 40);
	restartButton.setBounds(140, y, 80, 40);
	stopButton.setBounds(240, y, 80, 40);
    playPauseButton.setBounds(340, y, 80, 40);
    goToStartButton.setBounds(440, y, 80, 40);
    goToEndButton.setBounds(540, y, 80, 40);
	 muteButton.setBounds(340, y, 80, 40);

   
	/*prevButton.setBounds(340, y, 80, 40);
	nextButton.setBounds(440, y, 80, 40);*/
	 infolabel.setBounds(20, 140, getWidth() - 40, 30);
 backwardButton.setBounds(100, 60, 120, 30);
 forwardButton.setBounds(230, 60, 120, 30);

	volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

PlayerGUI::~PlayerGUI() 
{
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
       if (button == &loadButton)
    {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.aiff"
        );

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    bool ok = audioPlayer.loadFile(file);
                    if (ok)
                    {
                        juce::String info;
                        info << "Loaded file: " << file.getFileName() << "\n";

                        double totalSeconds = audioPlayer.getDuration();
                        int hours = static_cast<int>(totalSeconds) / 3600;
                        int minutes = (static_cast<int>(totalSeconds) % 3600) / 60;
                        int seconds = static_cast<int>(totalSeconds) % 60;

                        juce::String durationString;
                        if (hours > 0)
                            durationString = juce::String::formatted("%02d:%02d:%02d", hours, minutes, seconds);
                        else
                            durationString = juce::String::formatted("%02d:%02d", minutes, seconds);

                        info << "Duration: " << durationString;
                        infolabelSet(info);
                    }
                    else
                    {
                        juce::AlertWindow::showMessageBoxAsync(
                            juce::AlertWindow::WarningIcon,
                            "Load failed",
                            "Failed to open the selected audio file (unsupported format?)"
                        );
                    }
                }
            });
    }

   else if (button == &restartButton)
    {
        playerAudio.start();
    }

   else if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }
    else if (button = &playPauseButton) 
    {
        if (playPauseButton.getButtonText() == "Play") {

            playerAudio.start();
            playPauseButton.setButtonText("Pause");

        }
        else {
            playerAudio.stop();
            playPauseButton.setButtonText("Play");
        }

    }

   else if (button == &goToStartButton) 
    {
        playerAudio.setPosition(0.0);

    }
    else if (button == &goToEndButton)
    {
        double length = playerAudio.getLenght();
        playerAudio.setPosition(length);

    }
	 else if (button == &muteButton)
 {
     if (isMuted)
     {
         audioPlayer.setGain(previousGain);
         muteButton.setButtonText("Mute");
         isMuted = false;
     }
     else
     {
         previousGain = volumeSlider.getValue();
         audioPlayer.setGain(0.0f);
         muteButton.setButtonText("Unmute");
         isMuted = true;
     }
 }
     
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}
void PlayerGUI::infolabelSet(const juce::String& infoText)
{
    infolabel.setText(infoText, juce::dontSendNotification);
}

