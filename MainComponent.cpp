#include "MainComponent.h"

MainComponent::MainComponent()
    : playerGUI1(player1),
    playerGUI2(player2),
    playlistComponent(player1)
{

    setAudioChannels(0, 2);

    addAndMakeVisible(playerGUI1);
    addAndMakeVisible(playerGUI2);
  
    addAndMakeVisible(playlistComponent);

    setSize(800, 600);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{ bufferToFill.clearActiveBufferRegion();

 juce::AudioBuffer<float> temp(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
 juce::AudioSourceChannelInfo tempInfo(&temp, 0, bufferToFill.numSamples);

      
 player1.getNextAudioBlock(tempInfo);
 for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
     bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, temp, ch, 0, bufferToFill.numSamples);

 
 temp.clear();
 player2.getNextAudioBlock(tempInfo);
 for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
     bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, temp, ch, 0, bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
     player2.releaseResources();

}


void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    auto top = area.removeFromTop(getHeight() / 2);
    playerGUI1.setBounds(top.removeFromLeft(getWidth() / 2));
    playerGUI2.setBounds(top);

   
    playlistComponent.setBounds(area);
}
