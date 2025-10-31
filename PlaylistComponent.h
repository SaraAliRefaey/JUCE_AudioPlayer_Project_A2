#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlaylistComponent : public juce::Component,
    public juce::ListBoxModel,
    public juce::Button::Listener
{
public:
   
    PlaylistComponent(PlayerAudio& player);

    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;

    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;



    int getNumRows() override;

    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void loadplaylistFromFiles(const juce::Array<juce::File>& files);

private:
    juce::ListBox playlistBox;
    juce::TextButton addButton, removeButton;
    juce::Array<juce::File> trackFiles;
    int selectedTrackIndex = -1;

    PlayerAudio& audioPlayer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
