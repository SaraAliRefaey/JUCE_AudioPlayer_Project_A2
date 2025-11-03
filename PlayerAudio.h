#pragma once

#include<JuceHeader.h>

class PlayerAudio {

public:
    PlayerAudio();
    ~PlayerAudio();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources()override ;
    bool loadFile(const juce::File& file);
    void start();
    void replay();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLenght() const;

    void setLoopPoints(double startPos, double endPos);
    void enableLooping(bool shouldLoop);
    bool isLooping() const;
    double getLoopStart() const;
    double getLoopEnd() const;
 juce::String getArtist() const;
 double getDuration() const;
 void skipForward();
 void skipBackward();


    struct Marker
    {
        double position;
        juce::String name;
    };

    void addMarker(double position);
    const juce::Array<Marker>& getMarkers() const;
    void clearMarkers();
    void deleteMarker(int markerIndex);


private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    double loopStartPos = 0.0;
    double loopEndPos = 0.0;
    bool isLoopingEnabled = false;
    juce::Array<Marker> markers;
    int markerCount = 0;
 juce::String artistName;
 double durationSeconds = 0.0;

 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)

};
