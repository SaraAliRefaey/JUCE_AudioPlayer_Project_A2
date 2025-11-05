#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlaylistModel : public juce::ListBoxModel
{
public:
    PlaylistModel(PlayerAudio& p) : player(p) {}

    int getNumRows() override { return playlist.size(); }

    // Definition moved from PlaylistComponent.cpp
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
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

    // Definition moved from PlaylistComponent.cpp
    void selectedRowsChanged(int lastRowSelected) override
    {
        int selected = listBox->getSelectedRow();
        if (selected >= 0 && selected < getNumRows())
        {

            player.loadFile(playlist[selected]);
            player.start();
        }
    }

    void setListBox(juce::ListBox* lb) { listBox = lb; }
    void addTrack(const juce::File& file) { playlist.add(file); }
    void removeTrack(int index) { playlist.remove(index); }
    juce::Array<juce::File>& getPlaylist() { return playlist; }

private:
    juce::Array<juce::File> playlist;
    PlayerAudio& player;
    juce::ListBox* listBox = nullptr;
};

class PlaylistComponent : public juce::Component,
    public juce::Button::Listener
{
public:
    // FIX for LNK2019: Constructor already inline-defined here.
    PlaylistComponent(PlayerAudio& p) :
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

    ~PlaylistComponent() override = default;

    // Definition moved from PlaylistComponent.cpp
    void resized() override
    {
        auto area = getLocalBounds();
        auto buttonArea = area.removeFromBottom(40).reduced(5);

        addButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 3).reduced(5));
        deleteButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(5));
        clearButton.setBounds(buttonArea.reduced(5));

        area.removeFromTop(30);
        playlistListBox.setBounds(area.reduced(5));
    }

    // Definition moved from PlaylistComponent.cpp
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF34495E));
        g.setColour(juce::Colours::white);
        g.setFont(18.0f);
        g.drawText("Audio Playlist", 10, 5, getWidth() - 20, 20, juce::Justification::centredTop, true);
    }

    // Definition moved from PlaylistComponent.cpp
    void buttonClicked(juce::Button* button) override
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

private:
    PlayerAudio& audioPlayer;
    PlaylistModel playlistModel;

    juce::ListBox playlistListBox;
    juce::TextButton addButton{ "Add Tracks" };
    juce::TextButton deleteButton{ "Delete Selected" };
    juce::TextButton clearButton{ "Clear All" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)

public:

    // Definition moved from PlaylistComponent.cpp
    static juce::String getTimeString(double seconds)
    {
        int totalSeconds = (int)seconds;
        int minutes = totalSeconds / 60;
        int secs = totalSeconds % 60;
        return juce::String::formatted("%02d:%02d", minutes, secs);
    }
};
