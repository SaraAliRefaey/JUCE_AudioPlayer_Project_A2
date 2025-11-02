#include"PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
	formatManager.registerBasicFormats();
}
PlayerAudio::~PlayerAudio()
{
	//transportSource.setSource(nullptr);
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()

{
	transportSource.releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // ?? Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }
    }
    return true;
}
void PlayerAudio::start()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
}
void PlayerAudio::replay()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
    transportSource.start();
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getLenght() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setLoopPoints(double startPos, double endPos)
{
    double length = getLenght();

    if (length == 0.0)
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

bool PlayerAudio::isLooping() const
{
    return isLoopingEnabled;
}

double PlayerAudio::getLoopStart() const
{
    return loopStartPos;
}

double PlayerAudio::getLoopEnd() const
{
    return loopEndPos;
}


void PlayerAudio::addMarker(double position)
{
    if (getLenght() <= 0.0)
        return;
    double pos = juce::jlimit(0.0, getLenght(), position);
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

void PlayerAudio::clearMarkers()
{
    markers.clear();
    markerCount = 0;
}

void PlayerAudio::deleteMarker(int markerIndex)
{
    if (markerIndex >= 0 && markerIndex < markers.size())
    {
        markers.remove(markerIndex);
    }
}

const juce::Array<PlayerAudio::Marker>& PlayerAudio::getMarkers() const
{
    return markers;
}