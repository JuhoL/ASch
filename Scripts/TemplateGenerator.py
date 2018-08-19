import sys
import os
from datetime import datetime

def CreateDictionaryFromFile(fileName):
    targets = {}
    with open(fileName) as targetsFile:
        for line in targetsFile:
            elements = line.split( )
            targets[elements[0]] = elements[1:]
    return targets

class Template:
    date = datetime.now()
    
    argumentParameters = {"--author=" : "<__AUTHOR__>", \
                          "--email=" : "<__EMAIL__>", \
                          "--module=" : "<__MODULE__>", \
                          "--namespace=" : "<__NAMESPACE__>", \
                          "--path=" : "<__PATH__>"}

    keywords = {"<__DATE__>": date.strftime("%d %b %Y"), \
                "<__YEAR__>": date.strftime("%Y")}

    # Initialise by constructing the keyword dictionary based on given parameters.
    def __init__(self):
        print ("\nASch automatic template generator and build configurator\n(C) Juho Lepistö 2018\n")
        if sys.argv[1] == "-h":
            print ("\nUsage:")
            print ("    --module=MyModule               - Mandatory field. Name of the module. E.g. module name MyModule results in MyModule.cpp file.")
            print ("                                      Replaces <__MODULE__> tag in the template.")
            print ("    --path=MyModulePath             - Subfolder path of the module. E.g. path MyModulePath creates a subfolders ./MyModulePath/sources,")
            print ("                                      ./MyModulePath/include, and ./MyModulePath/tests")
            print ("    --author=\"John Jackson\"         - Author(s) of the code. Replaces <__AUTHOR__> tag in the template.")
            print ("    --email=\"john.jackson(a)jj.com\" - Author's contact email. Replaces <__EMAIL__> tag in the template.")
            print ("    --namespace=MyNamespace         - Namespace of the module. Replaces <__NAMESPACE__> tag in the template.")
            print ("")
            sys.exit(0)

        for argument in sys.argv[1:]:
            for parameterId in self.argumentParameters.keys():
                if argument.startswith(parameterId):
                    self.keywords[self.argumentParameters[parameterId]] = argument.split(parameterId,1)[1]

        if "<__MODULE__>" not in self.keywords.keys():
            print ("ERROR: Module name is mandatory! See -h for help.")
            sys.exit(1)

        if "<__PATH__>" not in self.keywords.keys():
            self.keywords["<__PATH__>"] = ""

        self.keywords["<__HEADERGUARD__>"] = self.keywords["<__MODULE__>"].upper() + "_H_"
        return

    # Scans the line for keywords and replace them.
    def ParseLine(self, line):
        for tag in self.keywords.keys():
            line = line.replace(tag, self.keywords[tag])
        return line

    # Generates a file from given template.
    def GenerateFromTemplate(self, targetFileName, templateFileName):
        if not os.path.exists(os.path.dirname(targetFileName)):
            os.makedirs(os.path.dirname(targetFileName))

        targetFile = open(targetFileName, 'w')
        with open(templateFileName) as templateFile:
            for line in templateFile:
                line = self.ParseLine(line)
                targetFile.write(line)
        return

    # Generates SCons files used for building.
    def GenerateSconsFiles(self):
        # Unit test include SCons
        sconsFile = open("./Build/SCons_UTest/UTest_" + self.keywords["<__MODULE__>"] + "_Include.scons", 'w')
        sconsFile.write("./" + self.keywords["<__PATH__>"] + "/sources\n")
        sconsFile.write("./" + self.keywords["<__PATH__>"] + "/include")
        sconsFile.close()

        # Unit test sources SCons
        sconsFile = open("./Build/SCons_UTest/UTest_" + self.keywords["<__MODULE__>"] + "_Sources.scons", 'w')
        sconsFile.write("./" + self.keywords["<__PATH__>"] + "/sources/" + self.keywords["<__MODULE__>"] + ".cpp\n")
        sconsFile.write("./" + self.keywords["<__PATH__>"] + "/tests/UTest_" + self.keywords["<__MODULE__>"] + ".cpp")
        sconsFile.close()

        # Unit test target names SCons
        sconsFile = open("./Build/SCons_UTest/UTestTargets.scons", 'r')
        targetConfig = self.keywords["<__MODULE__>"] + " " + ".\\" + self.keywords["<__PATH__>"]
        if targetConfig not in sconsFile.read():
            sconsFile.close()
            sconsFile = open("./Build/SCons_UTest/UTestTargets.scons", 'a')
            sconsFile.write("\n" + targetConfig)
        sconsFile.close()

        # Release include SCons
        sconsFile = open("./Build/SCons_Release/Include.scons", 'r')
        includePath = "./" + self.keywords["<__PATH__>"] + "/include"
        if includePath not in sconsFile.read():
            sconsFile.close()
            sconsFile = open("./Build/SCons_Release/Include.scons", 'a')
            sconsFile.write("\n" + includePath)
        sconsFile.close()

        # Release sources SCons
        sconsFile = open("./Build/SCons_Release/Sources.scons", 'r')
        sourceFile = "./" + self.keywords["<__PATH__>"] + "/sources/" + self.keywords["<__MODULE__>"] + ".cpp"
        if sourceFile not in sconsFile.read():
            sconsFile.close()
            sconsFile = open("./Build/SCons_Release/Sources.scons", 'a')
            sconsFile.write("\n" + sourceFile)
        sconsFile.close()
        return

    # Generates all files
    def GenerateAll(self): 
        self.GenerateFromTemplate("./" + self.keywords['<__PATH__>'] + "/sources/" + self.keywords['<__MODULE__>'] + ".cpp", "./Templates/cpptemplate.tmp")
        self.GenerateFromTemplate("./" + self.keywords['<__PATH__>'] + "/include/" + self.keywords['<__MODULE__>'] + ".h", "./Templates/htemplate.tmp")
        self.GenerateFromTemplate("./" + self.keywords['<__PATH__>'] + "/tests/UTest_" + self.keywords['<__MODULE__>'] + ".cpp", "./Templates/utesttemplate.tmp")
        self.GenerateSconsFiles()

try:
    template = Template()
    template.GenerateAll()
except Exception as e:
    print("\nERROR: Failed to generate template.")
    print (e)
