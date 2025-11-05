#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"
#include "PlaylistComponent.h" 

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    // Audio models (Multiple Players)
    PlayerAudio player1;
    PlayerAudio player2;

    // GUI views
    PlayerGUI playerGUI1{ player1 };
    PlayerGUI playerGUI2{ player2 };

    // Playlist component
    PlaylistComponent playlistComponent{ player1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};