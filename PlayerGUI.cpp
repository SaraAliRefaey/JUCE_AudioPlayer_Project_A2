#include"PlayerGUI.h"
#include "PlayerGUI.h"
#include "PlayerAudio.h"
#include <JuceHeader.h>



MarkerListBoxModel::MarkerListBoxModel(PlayerAudio& audio, juce::ListBox& lb, juce::TextButton& delBtn)
    : playerAudio(audio), listBox(lb), deleteButton(delBtn)
{
    deleteButton.setEnabled(false);
}


juce::String MarkerListBoxModel::getTimeString(double seconds)
{
    if (seconds < 0) seconds = 0;
    int totalSeconds = (int)seconds;
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}

int MarkerListBoxModel::getNumRows()
{
    return playerAudio.getMarkers().size();
}

void MarkerListBoxModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (rowNumber < getNumRows())
	{
		auto marker = playerAudio.getMarkers()[rowNumber];
		auto timeStr = getTimeString(marker.position);
		auto displayStr = marker.name + " (" + timeStr + ")";

		// Fill background based on selection
		if (rowIsSelected)
		{
			g.setColour(juce::Colour(0xFF00BFA5).withAlpha(0.4f)); // Cyan highlight
			g.fillRect(0, 0, width, height);
			g.setColour(juce::Colours::white);
		}
		else
		{
			g.setColour(juce::Colours::white.withAlpha(0.8f)); // Subtle white text
		}

		g.setFont(15.0f);
		g.drawText(displayStr, 8, 0, width - 16, height, juce::Justification::centredLeft, true);
	}
}
void MarkerListBoxModel::selectedRowsChanged(int lastRowSelected)
{
    int selectedRow = listBox.getSelectedRow();

    // Enable/disable delete button
    deleteButton.setEnabled(selectedRow >= 0);

    if (selectedRow >= 0 && selectedRow < getNumRows())
    {
        auto marker = playerAudio.getMarkers()[selectedRow];
        playerAudio.setPosition(marker.position);
    }
}



juce::String PlayerGUI::getTimeString(double seconds)
{
	if (seconds < 0) seconds = 0;
	int totalSeconds = (int)seconds;
	int minutes = totalSeconds / 60;
	int secs = totalSeconds % 60;
	return juce::String::formatted("%02d:%02d", minutes, secs);
}
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

	g.fillAll(juce::Colour(0xFF1B263B));
}

PlayerGUI::PlayerGUI() : markerListModel(playerAudio, markerListBox, deleteMarkerButton)
{

    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF4A5A7D)); // Dark Blue-Gray
    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF00BFA5)); // Cyan Accent for toggled state
    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::TextButton::textColourOffId, juce::Colours::white);

    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour(0xFF00BFA5));
    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.3f));
    juce::LookAndFeel::getDefaultLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::white.withAlpha(0.2f));

	for (auto* btn : { &loadButton, &restartButton , & stopButton, & replayButton,&setAButton, &setBButton, &toggleLoopButton, &addMarkerButton, &deleteMarkerButton})
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
        btn->setColour(juce::TextButton::textColourOffId, juce::Colours::white.withAlpha(0.8f));
        btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF283747)); // Slightly lighter than background
        btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF00BFA5)); // Cyan for Toggled/Active
        btn->setButtonText(btn->getButtonText().toUpperCase()); // Ensure buttons are capitalized for style
	}

    restartButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF28A745)); // Green for Play
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFDC3545)); // Red for Stop

	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::LinearBarVertical); // Change volume to vertical bar for a modern look
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(volumeSlider);

    // Position Slider
	positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
	positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	positionSlider.setRange(0.0, 1.0, 0.01);
	positionSlider.addListener(this);
	addAndMakeVisible(positionSlider);

    // Position Label
	positionLabel.setFont(juce::Font(18.0f)); // Slightly larger font
	positionLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF00BFA5)); // Cyan for the time display
	positionLabel.setText("00:00 / 00:00", juce::dontSendNotification);
	addAndMakeVisible(positionLabel);


    markerListBox.setModel(&markerListModel);
    markerListBox.setRowHeight(25);
    markerListBox.setOutlineThickness(0);
    markerListBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFF141D2B)); // Even darker list background
    markerListBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xFF00BFA5).withAlpha(0.5f));
    addAndMakeVisible(markerListBox);
	addAndMakeVisible(muteButton);
addAndMakeVisible(infolabel);
	  muteButton.setButtonText("Mute");
	  muteButton.addListener(this);

  addAndMakeVisible(forwardButton);
  addAndMakeVisible(backwardButton);
  forwardButton.onClick = [this] { audioPlayer.skipForward(); };
  backwardButton.onClick = [this] { audioPlayer.skipBackward(); };


	startTimer(40);
	setSize(600, 350);
 }

void PlayerGUI::resized()
{
	int margin = 10;
	int buttonW = 90;
	int buttonH = 35;
	int x = margin;
	int y = margin;
	int height = getHeight();
	int width = getWidth();

	loadButton.setBounds(x, y, buttonW, buttonH);
	x += buttonW + margin;
	restartButton.setBounds(x, y, buttonW, buttonH);
	x += buttonW + margin;
	stopButton.setBounds(x, y, buttonW, buttonH);
	x += buttonW + margin;
	replayButton.setBounds(x, y, buttonW, buttonH);

	x = margin;
	y += buttonH + margin;
	setAButton.setBounds(x, y, 60, 25);
	x += 60 + margin;
	setBButton.setBounds(x, y, 60, 25);
	x += 60 + margin;
	toggleLoopButton.setBounds(x, y, 90, 25);
	x += 90 + margin * 2;
	addMarkerButton.setBounds(x, y, 90, 25);
	x += 90 + margin;
	deleteMarkerButton.setBounds(x, y, 100, 25);


	y += 25 + margin * 2;
	positionLabel.setBounds(margin, y, 100, 25);
	positionSlider.setBounds(110, y, width - 110 - margin - 40, 25); // Make room for volume slider on the right

	int volumeW = 30;
	int volumeH = height - y - buttonH - margin;
	volumeSlider.setBounds(width - volumeW - margin, y, volumeW, height - y - margin);


	y += 25 + margin;
	markerListBox.setBounds(margin, y, width - volumeW - margin * 3, height - y - margin);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            ".wav;.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            ".wav;.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
					bool ok =playerAudio.loadFile(file);
					if (ok){
						 juce::String info;
 info << "Loaded file: " << file.getFileName() << "\n";

 double totalSeconds = PlayerAudio.getDuration();
 int hours = static_cast<int>(totalSeconds) / 3600;
 int minutes = (static_cast<int>(totalSeconds) % 3600) / 60;
 int seconds = static_cast<int>(totalSeconds) % 60;

 juce::String durationString;
                	double duration = playerAudio.getLenght();
					positionSlider.setRange(0.0, duration, 0.01);
					positionSlider.setValue(0.0);
                	playerAudio.setLoopPoints(0.0, duration);
					playerAudio.enableLooping(false);
					toggleLoopButton.setButtonText("Loop OFF");
                    playerAudio.clearMarkers();
                    markerListBox.updateContent();    if (hours > 0)
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
	 else if (button == &replayButton) {
		playerAudio.replay();
	}

   else  if (button == &addMarkerButton)
    {
        if (playerAudio.getLenght() > 0.0)
        {
            playerAudio.addMarker(playerAudio.getPosition());
            markerListBox.updateContent();
            markerListBox.selectRow(playerAudio.getMarkers().size() - 1);
        }
    }

    else if (button == &deleteMarkerButton)
    {
        int selectedRow = markerListBox.getSelectedRow();
        if (selectedRow >= 0)
        {
            playerAudio.deleteMarker(selectedRow);
            markerListBox.updateContent();
            markerListBox.selectRow(-1);
        }
    }

	double currentPos = playerAudio.getPosition();
	double length = playerAudio.getLenght();

	else if (button == &setAButton && length > 0.0)
	{
		double currentB = playerAudio.getLoopEnd();
		playerAudio.setLoopPoints(currentPos, currentB);
	}

	 else if (button == &setBButton && length > 0.0)
	{
		double currentA = playerAudio.getLoopStart();
		playerAudio.setLoopPoints(currentA, currentPos);
	}

	else if (button == &toggleLoopButton)
	{
		bool newLoopState = !playerAudio.isLooping();
		playerAudio.enableLooping(newLoopState);
		toggleLoopButton.setButtonText(newLoopState ? "Loop ON" : "Loop OFF");
		if (newLoopState)
		{
			playerAudio.setPosition(playerAudio.getLoopStart());
		}
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
	if (slider == &positionSlider)
	{
		playerAudio.setPosition(positionSlider.getValue());
	}
}

void PlayerGUI::timerCallback()
{
	double totalLength = playerAudio.getLenght();
	double currentPos = playerAudio.getPosition();

	if (totalLength > 0.0)
	{
		if (playerAudio.isLooping() && currentPos >= playerAudio.getLoopEnd())
		{

			playerAudio.setPosition(playerAudio.getLoopStart());
			currentPos = playerAudio.getLoopStart();
		}

		if (! positionSlider.isMouseButtonDown())
		{
			positionSlider.setValue(currentPos, juce::dontSendNotification);
		}
		juce::String current = getTimeString(currentPos);
		juce::String total = getTimeString(totalLength);
		positionLabel.setText(current + " / " + total, juce::dontSendNotification);
	}
	else
	{
		positionSlider.setRange(0.0, 1.0, 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);
		positionLabel.setText("00:00 / 00:00", juce::dontSendNotification);
		if (playerAudio.isLooping())
		{
			playerAudio.enableLooping(false);
			toggleLoopButton.setButtonText("Loop OFF");
		}
	}

}

void PlayerGUI::infolabelSet(const juce::String& infoText)
{
    infolabel.setText(infoText, juce::dontSendNotification);
}
