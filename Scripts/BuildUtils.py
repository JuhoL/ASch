import string

def CreateListFromFile(fileName):
    includes = []
    with open(fileName) as includeFile:
        for line in includeFile:
            includes.append(line.translate(str.maketrans('', '', '\r\n')))
    return includes

def CreateDictionaryFromFile(fileName):
    targets = {}
    with open(fileName) as targetsFile:
        for line in targetsFile:
            elements = line.split( )
            targets[elements[0]] = elements[1:]
    return targets
