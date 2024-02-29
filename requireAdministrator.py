import xml.etree.ElementTree as ET

def fix_namespace_and_set_uac_level(file_path, uac_level='requireAdministrator'):
    # Register the namespace
    ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')

    # Parse the project file
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Fixing namespace: remove 'ns0' prefix
    for elem in root.iter():
        if '}' in elem.tag:
            elem.tag = elem.tag.split('}', 1)[1]  # Remove namespace

    # Set UAC Execution Level
    for link in root.findall('.//Link'):
        uac_elem = link.find('UACExecutionLevel')
        if uac_elem is not None:
            uac_elem.text = uac_level
        else:
            ET.SubElement(link, 'UACExecutionLevel').text = uac_level

    # Write back to file
    tree.write(file_path, encoding='utf-8', xml_declaration=True)

# Specify the path to your project file
project_file_path = 'Builds/VisualStudio2022/Installer_App.vcxproj'

# Run the function on the provided project file
fix_namespace_and_set_uac_level(project_file_path)

print("Project file updated successfully.")