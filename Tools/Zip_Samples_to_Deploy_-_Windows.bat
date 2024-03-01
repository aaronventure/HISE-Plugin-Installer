@echo off
setlocal enabledelayedexpansion

rem Initialize the counter for zip file names
set "counter=1"
set "maxFileSize=2147483648"  rem 2GB in bytes

rem Set paths for input and output folders
set "inputFolder=..\Samples"
set "outputFolder=..\Deploy"

rem Ensure the output folder exists
if not exist "!outputFolder!" mkdir "!outputFolder!"

rem Initialize variables for the current zip file and its size
set "zipFileName=!outputFolder!\Samples!counter!.zip"
set "currentFileSize=0"

rem Loop through files with extensions .ch1 to .ch99 in the Samples folder
for /L %%i in (1, 1, 99) do (
    set "extension=.ch%%i"

    rem Check if files with the current extension exist
    if exist "!inputFolder!\*!extension!*.*" (
        rem Loop through files with the current extension and add them to the zip
        for %%f in (!inputFolder!\*!extension!*.*) do (
            set /a "fileSize=%%~zf"

            rem Check if adding the current file will exceed the maximum file size
            set /a "newFileSize=!currentFileSize! + !fileSize!"

            if !newFileSize! leq !maxFileSize! (
                rem Add the current file to the zip
                echo Adding "%%f" to "!zipFileName!"...
                "C:\Program Files\7-Zip\7z.exe" a -tzip "!zipFileName!" "%%~f"
                set "currentFileSize=!newFileSize!"
            ) else (
                rem Rename the current zip to .dat before creating a new zip file
                echo Renaming "!zipFileName!" to "Samples!counter!.dat"...
                ren "!zipFileName!" "Samples!counter!.dat"

                rem Prepare for the next zip file
                set /a "counter+=1"
                set "currentFileSize=!fileSize!"
                set "zipFileName=!outputFolder!\Samples!counter!.zip"

                rem Add the current file to the new zip
                echo Creating "!zipFileName!" and adding "%%f"...
                "C:\Program Files\7-Zip\7z.exe" a -tzip "!zipFileName!" "%%~f"
            )
        )
    )
)

rem After the loop, check and rename the last zip file if it exists
if exist "!zipFileName!" (
    echo Renaming the last zip file "!zipFileName!" to "Samples!counter!.dat"...
    ren "!zipFileName!" "Samples!counter!.dat"
)

echo Operation completed.
pause
