#include "PlaylistComponent.h"


void PlaylistModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        auto track = playlist[rowNumber];
        juce::String text = track.getFileName();

        if (rowIsSelected)
            g.fillAll(juce::Colour(0xFF757575).withAlpha(0.5f));

        g.setColour(rowIsSelected ? juce::Colours::white : juce::Colours::lightgrey);
        g.setFont(15.0f);
        g.drawText(text, 10, 0, width - 20, height, juce::Justification::centredLeft);
    }
}

void PlaylistModel::selectedRowsChanged(int lastRowSelected)
{
    int selected = listBox->getSelectedRow();
    if (selected >= 0 && selected < getNumRows())
    {

        player.loadFile(playlist[selected]);
        player.start();
    }
}

PlaylistComponent::PlaylistComponent(PlayerAudio& p) :
    audioPlayer(p),
    playlistModel(p)
{
    playlistModel.setListBox(&playlistListBox);
    playlistListBox.setModel(&playlistModel);
    playlistListBox.setRowHeight(30);
    playlistListBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFF2C3E50));
    addAndMakeVisible(playlistListBox);

    for (auto* btn : { &addButton, &deleteButton, &clearButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF34495E));
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Audio Playlist", 10, 5, getWidth() - 20, 20, juce::Justification::centredTop, true);
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds();
    auto buttonArea = area.removeFromBottom(40).reduced(5);

    addButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 3).reduced(5));
    deleteButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(5));
    clearButton.setBounds(buttonArea.reduced(5));

    area.removeFromTop(30);
    playlistListBox.setBounds(area.reduced(5));
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Add audio files to playlist...", juce::File{}, ".wav;.mp3;*.aiff");

        chooser->launchAsync(

            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,

            [this, chooser](const juce::FileChooser& chooserResult)
            {

                if (chooserResult.getResults().size() > 0)
                {
                    auto results = chooserResult.getResults();
                    for (auto& file : results)
                    {
                        if (file.existsAsFile())
                        {
                            playlistModel.addTrack(file);
                        }
                    }
                    playlistListBox.updateContent();
                }
            });
    }
    else if (button == &deleteButton)
    {
        int selected = playlistListBox.getSelectedRow();
        if (selected >= 0)
        {
            playlistModel.removeTrack(selected);
            playlistListBox.updateContent();
        }
    }
    else if (button == &clearButton)
    {
        playlistModel.getPlaylist().clear();
        playlistListBox.updateContent();
    }
}

juce::String PlaylistComponent::getTimeString(double seconds)
{
    int totalSeconds = (int)seconds;
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}