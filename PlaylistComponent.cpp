#include "PlaylistComponent.h"
#include "JuceHeader.h"

PlaylistComponent::PlaylistComponent(PlayerAudio& player) : audioPlayer(player)
{
    addAndMakeVisible(playlistBox);
    addAndMakeVisible(removeButton);
    addAndMakeVisible(addButton);

    addButton.setButtonText("Add");
    removeButton.setButtonText("Remove");

    playlistBox.setModel(this);
    addButton.addListener(this);
    removeButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
}

void PlaylistComponent::resized()
{
    juce::Rectangle<int> area = getLocalBounds();
    playlistBox.setBounds(area.removeFromTop(area.getHeight() - 40));
    addButton.setBounds(area.removeFromLeft(area.getWidth() / 2));
    removeButton.setBounds(area);
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        auto chooser = std::make_shared<juce::FileChooser>("Select audio files to add...",
            juce::File{},
            "*.wav;*.mp3;*.aiff");

        chooser->launchAsync(juce::FileBrowserComponent::openMode
            | juce::FileBrowserComponent::canSelectFiles
            | juce::FileBrowserComponent::canSelectMultipleItems,
            [this, chooser](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
                if (!files.isEmpty())
                {
                    for (auto& f : files)
                        trackFiles.add(f);

                    playlistBox.updateContent();
                    repaint();
                    DBG("Added " << files.size() << " files to playlist.");
                }
                else
                {
                    DBG("FileChooser returned no files.");
                }
            });
    }
    else if (button == &removeButton)
    {
        if (selectedTrackIndex >= 0 && selectedTrackIndex < trackFiles.size())
        {
            trackFiles.remove(selectedTrackIndex);
            selectedTrackIndex = -1;
            playlistBox.updateContent();
            DBG("Removed selected track.");
        }
    }
}

// ListBoxModel methods
int PlaylistComponent::getNumRows()
{
    return trackFiles.size();
}

void PlaylistComponent::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    if (rowNumber >= 0 && rowNumber < trackFiles.size())
    {
        juce::String fileName = trackFiles[rowNumber].getFileName();
        g.setColour(juce::Colours::black);
        g.drawText(fileName, 5, 0, width - 10, height, juce::Justification::centredLeft);
    }
}

void PlaylistComponent::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    selectedTrackIndex = row;

    if (row >= 0 && row < trackFiles.size())
    {
        juce::File selectedFile = trackFiles[row];
        audioPlayer.loadFile(selectedFile);
        audioPlayer.play();
    }
}

void PlaylistComponent::loadplaylistFromFiles(const juce::Array<juce::File>& files)
{
    trackFiles = files;
    playlistBox.updateContent();
}
