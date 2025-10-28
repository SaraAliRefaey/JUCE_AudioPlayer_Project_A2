#pragma once

#include<JuceHeader.h>

class PlayerAudio {

public:
    PlayerAudio();
    ~PlayerAudio();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) ;
    void releaseResources() ;
    bool loadFile(const juce::File& file);
	void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLenght() const;


private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;






};
