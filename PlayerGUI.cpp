#include "PlayerGUI.h"

juce::String MarkerListBoxModel::getTimeString(double seconds)
{
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
        auto text = marker.name + " (" + timeStr + ")";

        if (rowIsSelected)
        {
            g.fillAll(juce::Colour(0xFF00BFA5).withAlpha(0.4f));
            g.setColour(juce::Colours::white);

        }
        else
        {
            g.setColour(juce::Colours::white.withAlpha(0.85f));
        }

        g.setFont(15.0f);
        g.drawText(text, 10, 0, width - 20, height, juce::Justification::centredLeft);
    }
}

void MarkerListBoxModel::selectedRowsChanged(int lastRowSelected)
{
    int selected = listBox.getSelectedRow();
    deleteButton.setEnabled(selected >= 0);
    if (selected >= 0 && selected < getNumRows())
    {
        auto marker = playerAudio.getMarkers()[selected];
        playerAudio.setPosition(marker.position);
    }
}

//==============================================================================
// Player GUI Implementation
PlayerGUI::PlayerGUI(PlayerAudio& player)
    : audioPlayer(player), markerListModel(audioPlayer, markerListBox, deleteMarkerButton)
{
    // Buttons setup
    loadButton.setButtonText("Load");
    playPauseButton.setButtonText("► Play");
    stopButton.setButtonText("Stop");
    replayButton.setButtonText("Replay");
    muteButton.setButtonText("Mute");
    forwardButton.setButtonText("10s");
    backwardButton.setButtonText("⏪ 10s");
    toggleLoopButton.setButtonText("Loop OFF");
    setAButton.setButtonText("Set A");
    setBButton.setButtonText("Set B");
    addMarkerButton.setButtonText("Add Marker");
    deleteMarkerButton.setButtonText("Del Marker");

    for (auto* btn : { &loadButton, &playPauseButton, &stopButton, &replayButton, &muteButton,
                       &forwardButton, &backwardButton, &toggleLoopButton, &setAButton,
                       &setBButton, &addMarkerButton, &deleteMarkerButton,
                       &goToStartButton, &goToEndButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    // Sliders
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, 0.01);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    // Labels
    positionLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    positionLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(positionLabel);

    infoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    infoLabel.setText("No file loaded", juce::dontSendNotification);
    addAndMakeVisible(infoLabel);

    // Marker List
    markerListBox.setModel(&markerListModel);
    markerListBox.setRowHeight(25);
    markerListBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFF141D2B));
    addAndMakeVisible(markerListBox);

    setSize(700, 450);
    startTimerHz(30);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1B263B)); // Professional Look Color
}

void PlayerGUI::resized()
{
    int x = 20, y = 10, w = 90, h = 30, margin = 10;
    int longW = 110;

    // Row 1: Load, Sliders (Volume & Speed)
    loadButton.setBounds(x, y, w, h);
    volumeSlider.setBounds(w + 2 * margin, y, (getWidth() - w - 3 * margin) / 2, h);
    speedSlider.setBounds(getWidth() / 2 + 10, y, (getWidth() - w - 3 * margin) / 2, h);

    y += h + margin;
    // Row 2: Playback Controls (Start, Back, Play/Pause, Forward, End)
    goToStartButton.setBounds(x, y, 50, h); x += 60;
    backwardButton.setBounds(x, y, w, h); x += w + margin;
    playPauseButton.setBounds(x, y, longW, h); x += longW + margin;
    forwardButton.setBounds(x, y, w, h); x += w + margin;
    goToEndButton.setBounds(x, y, 50, h);

    x = 20; y += h + margin;
    // Row 3: Position Slider and Label
    positionSlider.setBounds(x, y, getWidth() - 40, 25); y += 30;
    positionLabel.setBounds(x, y, getWidth() - 40, 25); y += 30;

    // Row 4: Mute, Stop, Replay, Loop A/B Controls
    stopButton.setBounds(x, y, w, h); x += w + margin;
    replayButton.setBounds(x, y, w, h); x += w + margin;
    muteButton.setBounds(x, y, w, h); x += w + margin;
    setAButton.setBounds(x, y, 60, h); x += 70;
    setBButton.setBounds(x, y, 60, h); x += 70;
    toggleLoopButton.setBounds(x, y, longW, h);

    x = 20; y += h + margin;
    // Row 5: Info Label
    infoLabel.setBounds(x, y, getWidth() - 40, 30); y += 35;

    // Row 6: Marker Controls and List
    addMarkerButton.setBounds(x, y, 100, h); x += 110;
    deleteMarkerButton.setBounds(x, y, 100, h);
    y += h + margin;
    markerListBox.setBounds(20, y, getWidth() - 40, 100);
}

void PlayerGUI::timerCallback()
{
    double currentPos = audioPlayer.getPosition();
    double length = audioPlayer.getLength();
    bool isPlaying = audioPlayer.isPlaying();

    // 1. Position Slider Update
    if (positionSlider.isMouseButtonDown() == false)
    {
        if (length > 0.0)
        {
            positionSlider.setRange(0.0, length, 0.01);
            positionSlider.setValue(currentPos, juce::dontSendNotification);
        }
        else
        {
            positionSlider.setRange(0.0, 1.0, 0.01);
            positionSlider.setValue(0.0, juce::dontSendNotification);
        }
    }

    // 2. Button/Label Text Update
    juce::String timeStr = getTimeString(currentPos) + " / " + getTimeString(length);
    positionLabel.setText(timeStr, juce::dontSendNotification);
    playPauseButton.setButtonText(isPlaying ? "❚❚ Pause" : "► Play");
    muteButton.setButtonText(audioPlayer.isMuted() ? "Unmute" : "Mute");
    toggleLoopButton.setButtonText(audioPlayer.isLooping() ? "Loop ON" : "Loop OFF");

    // 3. Marker List Update
    markerListBox.updateContent();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    // === LOAD BUTTON ===
    if (button == &loadButton)
    {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select an audio file...", juce::File{}, ".wav;.mp3;*.aiff");

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& c)
            {
                auto file = c.getResult();
                if (file.existsAsFile() && audioPlayer.loadFile(file))
                {
                    positionSlider.setRange(0.0, audioPlayer.getLength(), 0.01);

                    // Metadata Display
                    juce::String infoText = "🎶 " + audioPlayer.getTitle() + " — Artist: " + audioPlayer.getArtist()
                        + "  (" + getTimeString(audioPlayer.getLength()) + ")";

                    infoLabel.setText(infoText, juce::dontSendNotification);
                    audioPlayer.start();
                }
                else
                {
                    infoLabel.setText("❌ Failed to load file", juce::dontSendNotification);
                }
            });
    }

    // === PLAYBACK CONTROLS ===
    else if (button == &playPauseButton) { audioPlayer.togglePlayPause(); }
    else if (button == &stopButton) { audioPlayer.stop(); }
    else if (button == &replayButton) { audioPlayer.replay(); }
    else if (button == &goToStartButton) { audioPlayer.goToStart(); }
    else if (button == &goToEndButton) { audioPlayer.goToEnd(); }
    else if (button == &forwardButton) { audioPlayer.skipForward(10.0); }
    else if (button == &backwardButton) { audioPlayer.skipBackward(10.0); }

    // === MUTE ===
    else if (button == &muteButton)
    {
        audioPlayer.toggleMute();
        if (audioPlayer.isMuted())
        {
            lastVolumeValue = volumeSlider.getValue();
            volumeSlider.setValue(0.0f, juce::dontSendNotification);
        }
        else
        {
            volumeSlider.setValue(lastVolumeValue, juce::dontSendNotification);
        }
    }

    // === A-B LOOP ===
    else if (button == &setAButton)
    {
        double pos = audioPlayer.getPosition();
        audioPlayer.setLoopPoints(pos, audioPlayer.getLoopEnd());
    }
    else if (button == &setBButton)
    {
        double pos = audioPlayer.getPosition();
        audioPlayer.setLoopPoints(audioPlayer.getLoopStart(), pos);
    }
    else if (button == &toggleLoopButton)
    {
        bool newLoopState = !audioPlayer.isLooping();
        if (newLoopState && audioPlayer.getLoopEnd() == 0.0 && audioPlayer.getLength() > 0.0)
            audioPlayer.setLoopPoints(0.0, audioPlayer.getLength());

        audioPlayer.enableLooping(newLoopState);
    }

    // === MARKERS ===
    else if (button == &addMarkerButton)
    {
        audioPlayer.addMarker(audioPlayer.getPosition());
        markerListBox.updateContent();
    }
    else if (button == &deleteMarkerButton)
    {
        int selectedRow = markerListBox.getSelectedRow();
        if (selectedRow >= 0)
        {
            audioPlayer.deleteMarker(selectedRow);
            markerListBox.updateContent();
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        float newGain = (float)volumeSlider.getValue();
        audioPlayer.setGain(newGain);

        if (newGain > 0.0f && audioPlayer.isMuted())
        {
            audioPlayer.toggleMute();
        }
        lastVolumeValue = newGain;
    }
    else if (slider == &speedSlider)
    {
        audioPlayer.setSpeed(slider->getValue());
    }
    else if (slider == &positionSlider && !positionSlider.isMouseButtonDown())
    {
        audioPlayer.setPosition(slider->getValue());
    }
}