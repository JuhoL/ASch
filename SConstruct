#!/usr/bin/env python
# Title           : SConstruct
# Description     : This is a SCons script for building ASch.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
# SCons version   : 3.0.1  
#==============================================================================

import sys
sys.path.insert(0, './Scons')

import build
import re

# Build in MinGW
env = Environment(tools = ['mingw'])

# Parse source files to be built from Sources.scons, include directories from Inlude.scons and CcFlags.scons files.
sourceFiles = build.GenerateList("./Scons/Sources.scons")
env.Append(CPPPATH=build.GenerateList("./Scons/Include.scons"))
env.Append(CCFLAGS=build.GenerateList("./Scons/CcFlags.scons"))

# Build object files into ./Build folder.
for sourceFile in sourceFiles:
    env.Object(target = "./Build/" + re.findall("(?<=\/)[^\/]*(?=.cpp)", sourceFile)[0], source = sourceFile)

# Build the main target from object files.
env.Program(target = './Build/testi', source = Glob("./Build/*.o"))
