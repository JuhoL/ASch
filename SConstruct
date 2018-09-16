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
import multiprocessing
from ListCreator import CreateDictionaryFromFile
from BuildTarget import *
from CppCheck import *
from RunUnitTest import *
from Coverage import GenerateCoverageReport

if GetOption('linux') == None:
    unitTest = Environment(tools = ['mingw', 'CppCheck', 'Gcov', 'Gcovr', 'Cobertura'])
    release = Environment(tools = ['mingw'])
else:
    unitTest = Environment(CC = 'g++', tools = ['CppCheck', 'Gcov', 'Gcovr', 'Cobertura'])
    release = Environment(CC = 'g++')

parameters = CreateDictionaryFromFile("./Build/SCons_UTest/UTestTargets.scons")

cores = multiprocessing.cpu_count() - 1
if cores == 0:
    cores = 1
SetOption('num_jobs', cores)
print("running with -j %s" % GetOption('num_jobs'))

step = GetOption('step')
if GetOption('step'):
    jenkinsDebugTargets = ["ASch_Queue", "ASch_System"]
    for target in jenkinsDebugTargets:
        buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                       "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                       "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                       "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
                        
        if step == 'build':
            BuildTarget(unitTest, target, buildFiles)
        elif step == 'unit_test':
            testRun = RunUnitTest(unitTest, target)
            unitTest.AlwaysBuild(testRun)
        elif step == 'cppcheck':
            cppcheck = CppCheck(unitTest, target, buildFiles)
            unitTest.AlwaysBuild(cppcheck)
        elif step == 'coverage':
            GenerateCoverageReport(unitTest, target, parameters[target][0])
        else:
            print ("ERROR: Invalid step parameter value " + step)
            sys.exit(1)
else:
    buildFiles = {"sources" : "./Build/SCons_Release/Sources.scons",
                  "include" : "./Build/SCons_Release/Include.scons",
                  "ccFlags" : "./Build/SCons_Release/CcFlags.scons",
                  "ldFlags" : "./Build/SCons_Release/LdFlags.scons"}
    asch = BuildTarget(release, 'ASch', buildFiles)
    release.Default(asch)

    if 'all' in COMMAND_LINE_TARGETS:
        for target in parameters.keys():
            buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                          "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                          "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                          "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
            BuildTarget(unitTest, target, buildFiles)
            
            testRun = RunUnitTest(unitTest, target)
            unitTest.Alias(target, testRun)
            
            cppcheck = CppCheck(unitTest, target, buildFiles)
            unitTest.Alias(target, cppcheck)
            
            GenerateCoverageReport(unitTest, target, parameters[target][0])
    else:
        # Build given targets.
        for target in COMMAND_LINE_TARGETS:
            if target in parameters.keys():
                buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                              "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                              "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                              "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
                BuildTarget(unitTest, target, buildFiles)
                
                testRun = RunUnitTest(unitTest, target)
                unitTest.Alias(target, testRun)
                
                cppcheck = CppCheck(unitTest, target, buildFiles)
                unitTest.Alias(target, cppcheck)
                
                GenerateCoverageReport(unitTest, target, parameters[target][0])
