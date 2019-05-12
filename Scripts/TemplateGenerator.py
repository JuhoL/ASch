#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : TemplateGenerator.py
# Description     : This script generates .cpp and .h files for given module
#                   and its unit tests, and it adds the unit tests into SCons
#                   build and the module into the release build.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
#==============================================================================

import sys
import os
import re
from datetime import datetime

class Template:
    date = datetime.now()
    
    argumentParameters = {"--author=" : "<__AUTHOR__>", \
                          "--email=" : "<__EMAIL__>", \
                          "--module=" : "<__MODULE__>", \
                          "--namespace=" : "<__NAMESPACE__>", \
                          "--path=" : "<__PATH__>"}

    keywords = {"<__DATE__>": date.strftime("%d %b %Y"), \
                "<__YEAR__>": date.strftime("%Y")}

    halMode = ""

    # Initialise by constructing the keyword dictionary based on given parameters.
    def __init__(self):
        print ("\nASch automatic template generator and build configurator\n(C) Juho Lepistö 2018\n")
        if sys.argv[1] == "-h":
            print ("\nUsage:")
            print ("    --module=MyModule                       - Mandatory field. Name of the module. E.g. module name MyModule results in MyModule.cpp file.")
            print ("                                              Replaces <__MODULE__> tag in the template.")
            print ("    --path=MyModulePath                     - Subfolder path of the module. E.g. path MyModulePath creates a subfolders ./MyModulePath/sources,")
            print ("                                              ./MyModulePath/include, and ./MyModulePath/tests")
            print ("    --author=\"John Jackson\"               - Author(s) of the code. Replaces <__AUTHOR__> tag in the template.")
            print ("    --email=\"john.jackson(a)jj.com\"       - Author's contact email. Replaces <__EMAIL__> tag in the template.")
            print ("    --namespace=MyNamespace                 - Namespace of the module. Replaces <__NAMESPACE__> tag in the template.")
            print ("    --hal=<source/header/header_with_tests> - Optional tag. Add this when you are creating HAL sources or API headers.")
            print ("")
            sys.exit(0)

        for argument in sys.argv[1:]:
            for parameterId in self.argumentParameters.keys():
                if argument.startswith(parameterId):
                    self.keywords[self.argumentParameters[parameterId]] = argument.split(parameterId,1)[1]
            if argument.startswith("--hal="):
                self.halMode = argument.split("--hal=",1)[1]

        if "<__MODULE__>" not in self.keywords.keys():
            print ("ERROR: Module name is mandatory! See -h for help.")
            sys.exit(1)

        if "<__PATH__>" not in self.keywords.keys():
            self.keywords["<__PATH__>"] = ""

        self.keywords["<__HEADERGUARD__>"] = self.keywords["<__MODULE__>"].upper() + "_HPP_"
        self.keywords["<__CLASS__>"] = re.split('[^a-zA-Z]', self.keywords["<__MODULE__>"])[-1]
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
        # Do not create build scripts if only header is created.
        if self.halMode != "header":
            # Unit test include SCons
            sconsFile = open("./Build/SCons_UTest/UTest_" + self.keywords["<__MODULE__>"] + "_Include.scons", 'w')
            sconsFile.write("./Config/include\n")
            sconsFile.write("./" + self.keywords["<__PATH__>"] + "/sources\n")
            # If only source file is created, no need to add the include path.
            if self.halMode != "source":
                sconsFile.write("./" + self.keywords["<__PATH__>"] + "/include")
            sconsFile.close()

            # Unit test sources SCons
            sconsFile = open("./Build/SCons_UTest/UTest_" + self.keywords["<__MODULE__>"] + "_Sources.scons", 'w')
            # Skip the module source if only header with tests is created.
            if self.halMode != "header_with_tests":
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

            if self.halMode != "header_with_tests":
                # Release sources SCons
                sconsFile = open("./Build/SCons_Release/Sources.scons", 'r')
                sourceFile = "./" + self.keywords["<__PATH__>"] + "/sources/" + self.keywords["<__MODULE__>"] + ".cpp"
                if sourceFile not in sconsFile.read():
                    sconsFile.close()
                    sconsFile = open("./Build/SCons_Release/Sources.scons", 'a')
                    sconsFile.write("\n" + sourceFile)
                sconsFile.close()

        # Release include SCons
        sconsFile = open("./Build/SCons_Release/Include.scons", 'r')
        includePath = "./" + self.keywords["<__PATH__>"] + "/include"
        if includePath not in sconsFile.read():
            sconsFile.close()
            sconsFile = open("./Build/SCons_Release/Include.scons", 'a')
            sconsFile.write("\n" + includePath)
        sconsFile.close()
        return

    # Generates all files
    def GenerateAll(self):
        targetSourceFile = "./" + self.keywords['<__PATH__>'] + "/sources/" + self.keywords['<__MODULE__>'] + ".cpp"
        targetHeaderFile = "./" + self.keywords['<__PATH__>'] + "/include/" + self.keywords['<__MODULE__>'] + ".hpp"

        # Check that the .cpp and .hpp files do not exist yet.
        if (self.halMode != "header" and self.halMode != "header_with_tests" and os.path.isfile(targetSourceFile)) or (self.halMode != "sources" and os.path.isfile(targetHeaderFile)):
            print ("Module " + self.keywords['<__MODULE__>'] + " in ./" + self.keywords['<__PATH__>'] + " already exists!")
            response = input("Answer YES if you want to overwrite (WARNING: the existing file(s) will be lost): ")
            if response.upper()[:3] != "YES":
                print ("Aborting file generation...\n")
                sys.exit(1)
        else:
            print ("Are you sure you want to create a module name " + self.keywords['<__MODULE__>'] + " to ./" + self.keywords['<__PATH__>'] + "?")
            response = input("Answer y/Y to confirm: ")
            if response.upper()[0] != "Y":
                print ("Aborting file generation...\n")
                sys.exit(1)

        print ("\nGenerating template files...")
        if self.halMode != "header":
            if self.halMode != "header_with_tests":
                self.GenerateFromTemplate(targetSourceFile, "./Templates/cpptemplate.tmp")
            self.GenerateFromTemplate("./" + self.keywords['<__PATH__>'] + "/tests/UTest_" + self.keywords['<__MODULE__>'] + ".cpp", "./Templates/utesttemplate.tmp")
        if self.halMode != "source":
            self.GenerateFromTemplate("./" + self.keywords['<__PATH__>'] + "/include/" + self.keywords['<__MODULE__>'] + ".hpp", "./Templates/hpptemplate.tmp")
        
        print ("Reconfiguring SCons...")
        self.GenerateSconsFiles()
        print ("\nAll done!\n")

        return

try:
    template = Template()
    template.GenerateAll()
except Exception as e:
    print("\nERROR: Failed to generate template.")
    print (e)
