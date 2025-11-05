#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    resamplerSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio()
{
    resamplerSource.reset();
    transportSource.setSource(nullptr);
    readerSource.reset();
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    if (resamplerSource)
        resamplerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    if (resamplerSource)
        resamplerSource->getNextAudioBlock(bufferToFill);
    else
        transportSource.getNextAudioBlock(bufferToFill);

    // A-B Loop Logic
    if (isLoopingEnabled && loopEndPos > 0.0)
    {
        double curPos = transportSource.getCurrentPosition();
        const double epsilon = 1.0 / 1000.0;
        if (curPos + epsilon >= loopEndPos)
            transportSource.setPosition(loopStartPos);
    }
}

void PlayerAudio::releaseResources()
{
    if (resamplerSource)
        resamplerSource->releaseResources();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& audioFile)
{
    if (!audioFile.existsAsFile())
        return false;

    auto* reader = formatManager.createReaderFor(audioFile);
    if (reader == nullptr)
        return false;

    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    // Reset State
    clearMarkers();
    currentFile = audioFile;
    durationSeconds = reader->lengthInSamples / reader->sampleRate;
    loopStartPos = 0.0;
    loopEndPos = durationSeconds;

    readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

    // Metadata extraction
    artistName = reader->metadataValues.containsKey("artist") ? reader->metadataValues["artist"] : "Unknown Artist";
    trackTitle = reader->metadataValues.containsKey("title") ? reader->metadataValues["title"] : audioFile.getFileNameWithoutExtension();

    if (resamplerSource)
        resamplerSource->setResamplingRatio(playbackSpeed);

    return true;
}


void PlayerAudio::start() { transportSource.start(); }
void PlayerAudio::stop() { transportSource.stop(); }

void PlayerAudio::replay()
{
    transportSource.setPosition(0.0);
    transportSource.start();
}

void PlayerAudio::togglePlayPause()
{
    if (transportSource.isPlaying())
        transportSource.stop();
    else
        transportSource.start();
}

void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd()
{
    transportSource.setPosition(getLength());
    transportSource.stop();
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}

// ==================== GAIN / MUTE ====================
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::toggleMute()
{
    if (isMutedState)
    {
        transportSource.setGain(lastGain);
        isMutedState = false;
    }
    else
    {
        if (transportSource.getGain() > 0.0f)
            lastGain = transportSource.getGain();

        transportSource.setGain(0.0f);
        isMutedState = true;
    }
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return durationSeconds;
}

void PlayerAudio::setSpeed(double speed)
{
    if (speed <= 0.0)
        return;

    playbackSpeed = speed;

    if (resamplerSource)
        resamplerSource->setResamplingRatio(playbackSpeed);
}

void PlayerAudio::skipForward(double seconds)
{
    double newPos = transportSource.getCurrentPosition() + seconds;
    if (newPos > getLength())
        newPos = getLength();
    transportSource.setPosition(newPos);
}

void PlayerAudio::skipBackward(double seconds)
{
    double newPos = transportSource.getCurrentPosition() - seconds;
    if (newPos < 0.0)
        newPos = 0.0;
    transportSource.setPosition(newPos);
}

// ==================== LOOP ====================
void PlayerAudio::setLoopPoints(double startPos, double endPos)
{
    double length = getLength();
    if (length <= 0.0)
    {
        loopStartPos = 0.0;
        loopEndPos = 0.0;
        return;
    }

    loopStartPos = juce::jlimit(0.0, length, startPos);
    loopEndPos = juce::jlimit(0.0, length, endPos);

    if (loopStartPos > loopEndPos)
        std::swap(loopStartPos, loopEndPos);
}

void PlayerAudio::enableLooping(bool shouldLoop)
{
    isLoopingEnabled = shouldLoop;
}

bool PlayerAudio::isLooping() const { return isLoopingEnabled; }
double PlayerAudio::getLoopStart() const { return loopStartPos; }
double PlayerAudio::getLoopEnd() const { return loopEndPos; }

juce::String PlayerAudio::getArtist() const { return artistName; }
double PlayerAudio::getDuration() const { return durationSeconds; }


void PlayerAudio::addMarker(double position)
{
    if (getLength() <= 0.0)
        return;

    double pos = juce::jlimit(0.0, getLength(), position);
    markerCount++;

    Marker newMarker;
    newMarker.position = pos;
    newMarker.name = "Marker " + juce::String(markerCount);

    int insertIndex = 0;
    for (int i = 0; i < markers.size(); ++i)
    {
        if (markers[i].position > pos)
        {
            insertIndex = i;
            break;
        }
        insertIndex = i + 1;
    }

    markers.insert(insertIndex, newMarker);
}

const juce::Array<PlayerAudio::Marker>& PlayerAudio::getMarkers() const
{
    return markers;
}

void PlayerAudio::clearMarkers()
{
    markers.clear();
    markerCount = 0;
}

void PlayerAudio::deleteMarker(int markerIndex)
{
    if (markerIndex >= 0 && markerIndex < markers.size())
        markers.remove(markerIndex);
}