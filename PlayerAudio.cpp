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

	readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
	transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
    durationSeconds = reader->lengthInSamples / reader->sampleRate;
    if (reader->metadataValues.containsKey("artist"))
        artistName = reader->metadataValues["artist"];
    else
        artistName = "Unknown Artist";


    return true;
}
void PlayerAudio::skipForward()
{
    double currentPos = transportSource.getCurrentPosition();
    double newPos = currentPos + 10.0;
    if (newPos < transportSource.getLengthInSeconds())
        transportSource.setPosition(newPos);
}

void PlayerAudio::skipBackward()
{
    double currentPos = transportSource.getCurrentPosition();
    double newPos = currentPos - 10.0;
    if (newPos < 0) newPos = 0;
    transportSource.setPosition(newPos);
}
void PlayerAudio::start()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
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
juce::String PlayerAudio::getArtist() const
{
    return artistName;
}

double PlayerAudio::getDuration() const
{
    return durationSeconds;
}
