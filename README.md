# HISE Plugin Installer

This is a cross-platform installer for HISE-based plugins. It's meant to be a simple app that works with accompanying files, allowing quick and light multiplatform distribution within a single download.

The installer looks for accompanying .dat files to do its thing so you can only build this once (or use a pre-built) and ship the same .exe forever, never again having to worry about building SmartScreen reputation etc.

The app was built with JUCE.

## Usage

1. Build on your system. Change the icon or font if you want, you'll have to do it in Projucer. Create a deployment directory and drop the built .exe in there.
2. Create Config.dat in the deployment directory. It's a zip file with a renamed extension to prevent automatic unarchiving. Config.dat should contain:
   -  info.json \* {
      "Project": "ProductName",
      "Company": "CompanyName",
      "Version": "1.0.0"
      }
   -  optionally background.png (this allows for dynamic GUI backgorunds)
3. Zip your plugins into Plugins.dat.
4. If you have samples, zip the .chx monoliths into Samples\*.dat files, each up to 2GB max.
5. Zip your AppData payload into AppData.dat,
6. Run the Installer.exe to test it.
