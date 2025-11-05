
#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    // Core AudioSource methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Playback control
    bool loadFile(const juce::File& file);
    void start();
    void stop();
    void replay();
    void togglePlayPause();
    void goToStart();
    void goToEnd();
    bool isPlaying() const;

    // Volume & Mute
    void setGain(float gain);
    void toggleMute();
    bool isMuted() const { return isMutedState; }

    // Position
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    // Playback speed
    void setSpeed(double speed);
    double getSpeed() const { return playbackSpeed; }

    // Skipping
    void skipForward(double seconds = 10.0);
    void skipBackward(double seconds = 10.0);

    // Looping
    void setLoopPoints(double startPos, double endPos);
    void enableLooping(bool shouldLoop);
    bool isLooping() const;
    double getLoopStart() const;
    double getLoopEnd() const;

    // Metadata
    juce::String getArtist() const;
    juce::String getTitle() const { return trackTitle; }
    double getDuration() const;
    juce::File getCurrentFile() const { return currentFile; }

    // Markers
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
    std::unique_ptr<juce::ResamplingAudioSource> resamplerSource;
    juce::AudioTransportSource transportSource;

    // Looping
    double loopStartPos = 0.0;
    double loopEndPos = 0.0;
    bool isLoopingEnabled = false;

    // State & Metadata
    juce::File currentFile;
    juce::Array<Marker> markers;
    juce::String artistName;
    juce::String trackTitle;
    double durationSeconds = 0.0;
    int markerCount = 0;

    // Speed
    double playbackSpeed = 1.0;

    // Mute
    float lastGain = 1.0f;
    bool isMutedState = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};