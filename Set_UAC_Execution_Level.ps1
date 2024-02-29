# Define the function to fix namespace and set UAC level
function Fix-NamespaceAndSetUACLevel {
    param (
        [string]$FilePath,
        [string]$UACLevel = 'requireAdministrator'
    )

    # Load the XML file
    [xml]$XmlContent = Get-Content $FilePath

    # Define the namespace to handle the 'msb' prefix properly
    $NamespaceManager = New-Object System.Xml.XmlNamespaceManager($XmlContent.NameTable)
    $NamespaceManager.AddNamespace('msb', 'http://schemas.microsoft.com/developer/msbuild/2003')

    # Fixing namespace: This is handled differently in PowerShell. Adjustments may not be necessary as in Python.

    # Set UAC Execution Level
    $XmlContent.SelectNodes('//msb:Link', $NamespaceManager) | ForEach-Object {
        $UACExecutionLevel = $_.SelectSingleNode('msb:UACExecutionLevel', $NamespaceManager)
        if ($UACExecutionLevel -eq $null) {
            $UACExecutionLevel = $XmlContent.CreateElement('UACExecutionLevel', 'http://schemas.microsoft.com/developer/msbuild/2003')
            $_.AppendChild($UACExecutionLevel)
        }
        $UACExecutionLevel.InnerText = $UACLevel
    }

    # Write back to file
    $XmlContent.Save($FilePath)
}

# Specify the path to your project file
$projectFilePath = 'Builds/VisualStudio2022/Installer_App.vcxproj'

# Run the function on the provided project file
Fix-NamespaceAndSetUACLevel -FilePath $projectFilePath

Write-Host "Project file updated successfully."
