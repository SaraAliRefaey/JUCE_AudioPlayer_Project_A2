#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"



class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;

private:
    PlayerAudio player1;
   PlayerAudio player2;

   PlayerGUI playerGUI1{ player1 };
   PlayerGUI playerGUI2{ player2 };


   PlaylistComponent playlistComponent{ player1 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
