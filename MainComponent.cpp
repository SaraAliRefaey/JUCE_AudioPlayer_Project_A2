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
{
    bufferToFill.clearActiveBufferRegion();

    juce::AudioBuffer<float> temp(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    juce::AudioSourceChannelInfo tempInfo(&temp, 0, bufferToFill.numSamples);

    // 1. Get audio from Player 1 and mix it into the destination buffer
    temp.clear();
    player1.getNextAudioBlock(tempInfo);
    for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
        bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, temp, ch, 0, bufferToFill.numSamples);

    // 2. Get audio from Player 2 and mix it into the destination buffer
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
    g.fillAll(juce::Colour(0xFF1B263B));
    g.setColour(juce::Colours::lightgrey.withAlpha(0.3f));
    g.drawLine((float)getWidth() / 2, 0.0f, (float)getWidth() / 2, (float)getHeight(), 1.0f);
    g.drawLine(0.0f, (float)getHeight() * 0.6f, (float)getWidth(), (float)getHeight() * 0.6f, 1.0f);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    // Top 60% for the two players
    auto playerArea = area.removeFromTop(getHeight() * 0.6f);

    // Player 1 (Left Half)
    playerGUI1.setBounds(playerArea.removeFromLeft(getWidth() / 2));

    // Player 2 (Right Half)
    playerGUI2.setBounds(playerArea);

    // Bottom 40% for the playlist component
    playlistComponent.setBounds(area);
}