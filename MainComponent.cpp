#include "MainComponent.h"

MainComponent::MainComponent() : playerGUI(playerAudio), playlistComponent(playerAudio)
{
    setAudioChannels(0, 2);
    addAndMakeVisible(playerGUI);
    setSize(600, 400);
    addAndMakeVisible(playlistComponent);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    playerAudio.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    playerGUI.setBounds(0, 0, getWidth(), getHeight() / 2);
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}
