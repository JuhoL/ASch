#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : SConstruct
# Description     : This is a SCons script for building ASch.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
# SCons version   : 3.0.1  
#==============================================================================

import sys
from ListCreator import CreateDictionaryFromFile
from BuildTarget import *
from CppCheck import *
from RunUnitTest import *
from Coverage import GenerateCoverageReport

# Build in MinGW
unitTest = Environment(tools = ['mingw', 'CppCheck', 'Gcov', 'Gcovr', 'Cobertura'])
release = Environment(tools = ['mingw'])

parameters = CreateDictionaryFromFile("./Build/SCons_UTest/UTestTargets.scons")

buildFiles = {"sources" : "./Build/SCons_Release/Sources.scons",
              "include" : "./Build/SCons_Release/Include.scons",
              "ccFlags" : "./Build/SCons_Release/CcFlags.scons",
              "ldFlags" : "./Build/SCons_Release/LdFlags.scons"}
asch = BuildTarget(release, 'ASch', buildFiles)
release.Default(asch)

# Check if test build is activated.
for target in COMMAND_LINE_TARGETS:
    if target in parameters.keys():
        buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                      "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                      "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                      "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
        BuildTarget(unitTest, target, buildFiles)
        RunUnitTest(unitTest, target)
        CppCheck(unitTest, target, buildFiles)
        GenerateCoverageReport(unitTest, target, parameters[target][0])
