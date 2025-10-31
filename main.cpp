#include <JuceHeader.h>
#include "MainComponent.h"

class AudioPlayerApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow("Audio Player", *this));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

   
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name, juce::JUCEApplication& app)
            : DocumentWindow(name,
                juce::Colours::lightgrey,
                DocumentWindow::allButtons),
            ownerApp(app)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true); 
            centreWithSize(400, 200);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            ownerApp.systemRequestedQuit();
        }

    private:
        juce::JUCEApplication& ownerApp;
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AudioPlayerApplication)
