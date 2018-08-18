import string

def GenerateList(fileName):
    includes = []
    with open(fileName) as includeFile:
        for line in includeFile:
            includes.append(line.translate(str.maketrans('', '', '\r\n')))
    return includes
