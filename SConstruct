#!/usr/bin/env python
# Title           : SConstruct
# Description     : This is a SCons script for building ASch.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
# SCons version   : 3.0.1  
#==============================================================================

import sys
sys.path.insert(0, './Build/Scripts')

import BuildUtils
import re

def AbortBuild():
    print ("\nAborting build...\n")
    sys.exit(2)

# Build in MinGW
env = Environment(tools = ['mingw'])

# Check if test build is activated.
test = ARGUMENTS.get('test')
if test != None:
    print ("\nRun " + test + " unit tests.\n")

    try:
        includePaths = BuildUtils.GenerateList("./Build/SCons_UTest/UTest_" + test + "_Include.scons")
        includePaths.append('./Catch')
    except:
        print ("ERROR: Gathering include directories failed!")
        print ("       Make sure you have created UTest_" + test + "_Include.scons file listing")
        print ("       the include directories neede for the tests!")
        AbortBuild()

    try:
        sourceFiles = BuildUtils.GenerateList("./Build/SCons_UTest/UTest_" + test + "_Sources.scons")
    except:
        print ("ERROR: Gathering source files failed!")
        print ("       Make sure you have created UTest_" + test + "_Sources.scons file listing")
        print ("       the source files needes for the tests!")
        AbortBuild()

    env.Append(CCFLAGS=BuildUtils.GenerateList("./Build/SCons_UTest/UTestCcFlags.scons"))
    env.Append(LINKFLAGS = BuildUtils.GenerateList("./Build/SCons_UTest/UTestLdFlags.scons"))
    env.Append(CPPPATH = includePaths)
    unitTest = env.Program(target = './Build/Tests/UTest_' + test, source = sourceFiles)
    env.NoClean(unitTest)

else:
    print ("\nBuilding target...\n")
    # Parse source files to be built from Sources.scons, include directories from Inlude.scons and CcFlags.scons files.
    sourceFiles = BuildUtils.GenerateList("./Build/SCons_Release/Sources.scons")
    env.Append(CPPPATH=BuildUtils.GenerateList("./Build/SCons_Release/Include.scons"))
    env.Append(CCFLAGS=BuildUtils.GenerateList("./Build/SCons_Release/CcFlags.scons"))
    env.Append(LINKFLAGS = BuildUtils.GenerateList("./Build/SCons_Release/LdFlags.scons"))

    # Build object files into ./Build folder.
    for sourceFile in sourceFiles:
        env.Object(target = "./Build/Objects/" + re.findall("(?<=\/)[^\/]*(?=.cpp)", sourceFile)[0], source = sourceFile)

    # Build the main target from object files.
    release = env.Program(target = './Build/Release/ASch', source = Glob("./Build/Objects/*.o"))
    env.NoClean(release)
