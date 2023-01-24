import xml.etree.ElementTree as ET

xml_file = ET.parse('d9332381-63.xml')

root = xml_file.getroot()

list_annotation = []

for child in root: 
    print(child.tag)
