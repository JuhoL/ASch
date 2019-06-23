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
from Doxygen import *
from RunUnitTest import *
from Coverage import GenerateCoverageReport

# This script builds given target
def BuildTest(target, env, parameters, buildAll):
    buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                  "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                  "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                  "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
    
    if GetOption('test') != None:
        # Build and run the tests
        BuildTarget(env, target, buildFiles)
        testRun = RunUnitTest(env, target)
        env.AlwaysBuild(testRun)
        if buildAll == True:
            env.Alias("all", testRun)
        else:
            env.Alias(target, testRun)
    
    if GetOption('cpp_check') != None:
        cppcheck = CppCheck(env, target, buildFiles)
        env.AlwaysBuild(cppcheck)
        if buildAll == True:
            env.Alias("all", cppcheck)
        else:
            env.Alias(target, cppcheck)
    
    if GetOption('coverage') != None:
        GenerateCoverageReport(env, target, parameters[target][0])

#------------ SCons script run starts here ------------
if GetOption('linux') != None:
    unitTest = Environment(tools = ['gcc', 'CppCheck', 'Gcov', 'Gcovr', 'Cobertura'])
    release = Environment(tools = ['gcc', 'Binary', 'Doxygen'])
else:
    unitTest = Environment(tools = ['mingw', 'CppCheck', 'Gcov', 'Gcovr', 'Cobertura'])
    release = Environment(tools = ['mingw', 'Binary', 'Doxygen'])

# Configure ARM GCC for release
release['AR'] = 'arm-eabi-ar'
release['AS'] = 'arm-eabi-as'
release['CC'] = 'arm-eabi-gcc'
release['CXX'] = 'arm-eabi-g++'
release['LINK'] = 'arm-eabi-g++'
release['RANLIB'] = 'arm-eabi-ranlib'
release['OBJCOPY'] = 'arm-eabi-objcopy'
release['PROGSUFFIX'] = '.elf'

parameters = CreateDictionaryFromFile("./Build/SCons_UTest/UTestTargets.scons")

cores = multiprocessing.cpu_count() - 1
if cores == 0:
    cores = 1
SetOption('num_jobs', cores)
print("running with -j %s" % GetOption('num_jobs'))

if GetOption('test') != None or GetOption('cpp_check') != None or GetOption('coverage') != None:
    # Create list of targets depending on comman line arguments
    targetList = []
    buildAll = False
    if 'all' in COMMAND_LINE_TARGETS or COMMAND_LINE_TARGETS == []:
        targetList = list(parameters.keys())
        buildAll = True
    else:
        targetList = COMMAND_LINE_TARGETS

    # Build all targets from the list.
    for target in targetList:
        buildFiles = {"sources" : "./Build/SCons_UTest/UTest_" + target + "_Sources.scons",
                       "include" : "./Build/SCons_UTest/UTest_" + target + "_Include.scons",
                       "ccFlags" : "./Build/SCons_UTest/UTestCcFlags.scons",
                       "ldFlags" : "./Build/SCons_UTest/UTestLdFlags.scons"}
        BuildTest(target, unitTest, parameters, buildAll)
elif GetOption('doxygen') != None:
    targetList = COMMAND_LINE_TARGETS
    for target in targetList:
        Doxygen(release, target)
else:
    buildFiles = {"sources"  : "./Build/SCons_Release/Sources.scons",
                  "include"  : "./Build/SCons_Release/Include.scons",
                  "ccFlags"  : "./Build/SCons_Release/CcFlags.scons",
                  "cxxFlags" : "./Build/SCons_Release/CxxFlags.scons",
                  "ldFlags"  : "./Build/SCons_Release/LdFlags.scons",
                  "cmsis"    : "./Build/SCons_Release/CmsisSources.scons"}
    asch = BuildTarget(release, 'ASch', buildFiles)
    release.Default(asch)
