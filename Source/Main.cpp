#include <JuceHeader.h>
#include "MainComponent.h"
#include "CustomLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================

// Assuming Info is a class that can be constructed from a var object
class Info {
public:
    Info(const juce::var& jsonData)
        : jsonData(jsonData)
    {
    }

    juce::String getProject() const {
        return jsonData.getProperty("Project", "Unknown");
    }

    juce::String getVersion() const {
        return jsonData.getProperty("Version", "Unknown");
    }

    juce::String getCompany() const {
        return jsonData.getProperty("Company", "Unknown");
    }

    juce::var jsonData;
private:
    
};

// Load the Info object from the zip file and return it
// Use this to get Project name, Company name, Version etc.
Info loadInfo() {
    juce::File appDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
    juce::File configFile = appDirectory.getChildFile("Config.dat");

    DBG("Looking for Config.dat at: " << configFile.getFullPathName());

    if (configFile.exists()) {
        juce::ZipFile zip(configFile);

        int index = zip.getIndexOfFileName("info.json");
        if (index < 0) {
            throw std::runtime_error("Could not find Info.json in zip file.");
        }

        std::unique_ptr<juce::InputStream> stream(zip.createStreamForEntry(index));
        if (!stream) {
            throw std::runtime_error("Could not create stream for Info.json.");
        }

        juce::var jsonData = juce::JSON::parse(*stream);
        if (!jsonData.isObject()) {
            throw std::runtime_error("Invalid JSON data in Info.json.");
        }

        // Check that the JSON data contains the required properties
        juce::DynamicObject* json = jsonData.getDynamicObject();
        if (!json->hasProperty("Project") || !json->hasProperty("Company") || !json->hasProperty("Version")) {
            throw std::runtime_error("Invalid JSON data in Info.json: Missing Project, Company, or Version property.");
        }

        return Info(jsonData);
    }
    else {
        throw std::runtime_error("Config.dat not found");
    }
}

//============================================================================================================================================


class InstallerApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
        auto mainComponent = new MainComponent();
        mainWindow->setContentOwned(mainComponent, true);

        try {
            Info info = loadInfo();
            // Now you can use the info object

            // Get the Project, Version, and Company properties from the Info object
            juce::String project = info.jsonData.getProperty("Project", "Unknown");
            juce::String version = info.jsonData.getProperty("Version", "Unknown");
            juce::String company = info.jsonData.getProperty("Company", "Unknown");

            // Get the HideTitle property from the Info object
            int hideTitle = info.jsonData.getProperty("HideTitle", 0);

            // Set the title of the MainWindow
            if (hideTitle == 1) {
                mainWindow->setName("");
            }
            else {
                juce::String title = company + " - " + project + " (" + version + ")";
                mainWindow->setName(title);
            }

            // Check for background.png in Config.dat
            juce::File appDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            juce::File configFile = appDirectory.getChildFile("Config.dat");
            juce::ZipFile zip(configFile);
            int index = zip.getIndexOfFileName("background.png");
            if (index >= 0) {
                std::unique_ptr<juce::InputStream> stream(zip.createStreamForEntry(index));
                juce::Image background = juce::ImageFileFormat::loadFrom(*stream);
                if (!background.isNull()) {
                    // Assuming `mainComponent` is a Component
                    mainComponent->setBackgroundImage(background);
                }
            }

            // Check for Plugins.dat
            juce::File pluginsFile = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getChildFile("Plugins.dat");
            if (!pluginsFile.exists()) {
                throw std::runtime_error("Plugins.dat not found.");
            }

            //===========================

            // Check for Samples archives and set up the directory browser
            // Set up an array to store the results
            juce::Array<juce::File> results;

            // Find .dat files with "Samples" or "samples" in their names
            appDirectory.findChildFiles(results, juce::File::findFiles, false, "*Samples*.dat");
            appDirectory.findChildFiles(results, juce::File::findFiles, false, "*samples*.dat");


            // Get the width and height of the UI
            int uiWidth = mainComponent->getWidth();
            int uiHeight = mainComponent->getHeight();

            // Initialize hasSamples to 0
            int hasSamples = 0;

            // If such files are found, set hasSamples to 1
            if (results.size() > 0) {
                hasSamples = 1;

                // Initialize the current path
#if JUCE_MAC
                currentPath = "~/Music/Audio Music Apps/Samples/" + company + "/" + project;
#else
                currentPath = juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory).getFullPathName() + "/" + company + "/" + project;
#endif

                // Create a LookAndFeel object
                directorySelectButtonLAF = new DirectorySelectButtonLookAndFeel(currentPath);
                installButtonLAF = new InstallButtonLookAndFeel();


                // Create a button to select the directory
                directorySelectButton = mainComponent->createAndDisplayButton("DirectorySelect", 100, 50, uiWidth-200, 40, directorySelectButtonLAF);

                // Add a callback for when the button is clicked
                directorySelectButton->onClick = [this, project]() {
                    // Create a FileChooser object
                    juce::FileChooser chooser("Select a directory",
#if JUCE_MAC
                        juce::File("~/Music/Audio Music Apps/Samples/"),
#else
                        juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory),
#endif
                        "", // wildcard filter
                        true); // use native version if available

                    // Show the directory selection dialog
                    if (chooser.browseForDirectory()) {
                        // If a directory was selected, get the selected directory
                        juce::File selectedDirectory = chooser.getResult();

                        // Append the project name to the selected directory
                        juce::File finalDirectory = selectedDirectory.getChildFile(project);

                        // Store the final directory path in currentPath
                        currentPath = finalDirectory.getFullPathName();

                        // You can now use currentPath in your code
                    }
                    };

            }

            // Install button width
            int installWidth = 200;

            // Create a button to install and store the returned pointer in a variable
            installButton = mainComponent->createAndDisplayButton("Install", (uiWidth - installWidth) / 2, 200, installWidth, 60, installButtonLAF);

            // Add a callback for when the button is clicked
            installButton->onClick = [this, company, project]() {
                DBG("Install button clicked!");

                loadInfo(); // check for Config.dat again

                // Check for Plugins.dat
                juce::File pluginsFile = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getChildFile("Plugins.dat");
                if (!pluginsFile.exists()) {
                    throw std::runtime_error("Plugins.dat not found.");
                }

                // Create a ZipFile object
                juce::ZipFile zip(pluginsFile);

                // Determine the destination directory
                juce::File vst3Destination;
#if JUCE_WINDOWS
                vst3Destination = juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory)
                    .getChildFile("Common Files")
                    .getChildFile("VST3")
                    .getChildFile(company)
                    .getChildFile(project);
#elif JUCE_MAC
                vst3Destination = juce::File("~").getChildFile("Library/Audio/Plug-Ins/VST3");
#elif JUCE_LINUX
                vst3Destination = juce::File("$HOME/.vst3");
#endif

                // Extract .vst3 files
                for (int i = 0; i < zip.getNumEntries(); ++i) {
                    const juce::ZipFile::ZipEntry* entry = zip.getEntry(i);
                    if (entry && entry->filename.endsWith(".vst3")) {
                        std::unique_ptr<juce::InputStream> stream(zip.createStreamForEntry(i));
                        if (stream) {
                            juce::File outputFile = vst3Destination.getChildFile(entry->filename);
                            outputFile.getParentDirectory().createDirectory(); // Create the directory
                            juce::FileOutputStream outputStream(outputFile);
                            if (outputStream.openedOk()) {
                                outputStream.writeFromInputStream(*stream, -1);
                                DBG("Extracted file: " + outputFile.getFullPathName());
                            }
                            else {
                                DBG("Failed to open output stream for file: " + outputFile.getFullPathName());
                                DBG("Error message: " + outputStream.getStatus().getErrorMessage());
                            }
                        }
                    }
                }

                };
            
        }
        catch (const std::runtime_error& e) {
            // Handle error
            mainComponent->setMessage(e.what());
        }
    }


    

    void shutdown() override
    {
        mainWindow = nullptr; // (deletes our window

        delete directorySelectButtonLAF; // Delete the LookAndFeel object
        directorySelectButtonLAF = nullptr; // Set the pointer to nullptr

        delete installButtonLAF; // Delete the LookAndFeel object
        installButtonLAF = nullptr; // Set the pointer to nullptr
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.

        quit();
    }

    //============================================================================================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Colour(0xff222222),
                              DocumentWindow::closeButton | DocumentWindow::minimiseButton)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);

            setResizable (false, false);
            centreWithSize (getWidth(), getHeight());

            setLookAndFeel(&customLookAndFeel);

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.

            setLookAndFeel(nullptr); // clear the look and feel
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        CustomLookAndFeel customLookAndFeel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;

    DirectorySelectButtonLookAndFeel* directorySelectButtonLAF = nullptr;
    InstallButtonLookAndFeel* installButtonLAF = nullptr;
    
    juce::String currentPath;


    juce::TextButton* directorySelectButton = nullptr;
    juce::TextButton* installButton = nullptr;
};


//============================================================================================================================================


//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (InstallerApplication)
