#pragma once

#include<JuceHeader.h>

class PlayerAudio {

public:
    PlayerAudio();
    ~PlayerAudio();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) ;
    void releaseResources()override ;
    bool loadFile(const juce::File& file);
	void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLenght() const;
    juce::String getArtist() const;
    double getDuration() const;
    void skipForward();
    void skipBackward();



private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::String artistName;
    double durationSeconds = 0.0;
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
   






};
