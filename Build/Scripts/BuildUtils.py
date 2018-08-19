import string

def GenerateList(fileName):
    includes = []
    with open(fileName) as includeFile:
        for line in includeFile:
            includes.append(line.translate(str.maketrans('', '', '\r\n')))
    return includes

def GenerateTargetDictionary():
    targets = {}
    with open("./Build/SCons_UTest/UTestTargets.scons") as targetsFile:
        for line in targetsFile:
            elements = line.split( )
            targets[elements[0]] = [elements[1], elements[2]]
    return targets
