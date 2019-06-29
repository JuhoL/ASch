#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : BuildTarget.py
# Description     : These is target builder utility script for SCons.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

import re
import sys
from ListCreator import CreateListFromFile

def GetFiles(fileGroup, buildFiles):
    try:
        fileList = CreateListFromFile(buildFiles[fileGroup])
    except Exception as e:
        if fileGroup == "sources":
            # Only sources are mandatory
            print (e)
            print ("ERROR: Gathering source files failed!")
            print ("       Make sure you have created .scons file listings under ./Build/SCons_UTest and ./Build/SCons_Release.")
            sys.exit(1)
        else:
            fileList = []
    return fileList

def BuildTarget(env, buildTarget, buildFiles):
    # Parse source files to be built from Sources.scons, include directories from Inlude.scons and CcFlags.scons files.
    sourceFiles = GetFiles("sources", buildFiles)
    includeFiles = GetFiles("include", buildFiles)
    sourceFiles += GetFiles("generalSources", buildFiles)
    includeFiles += GetFiles("generalInclude", buildFiles)

    env.AppendUnique(CPPPATH = includeFiles)

    if "ccFlags" in buildFiles:
        env.AppendUnique(CCFLAGS = CreateListFromFile(buildFiles["ccFlags"]))
        if env.GetOption('debug') != None:
            env.AppendUnique(CCFLAGS = '-DDEBUG')
    if "cxxFlags" in buildFiles:
        env.AppendUnique(CXXFLAGS = CreateListFromFile(buildFiles["cxxFlags"]))
        if env.GetOption('debug') != None:
            env.AppendUnique(CCFLAGS = '-DDEBUG')
    if "ldFlags" in buildFiles:
        env.AppendUnique(LINKFLAGS = CreateListFromFile(buildFiles["ldFlags"]))
    if "asmFlags" in buildFiles:
        env.AppendUnique(ASFLAGS = CreateListFromFile(buildFiles["asmFlags"]))

    if buildTarget == 'ASch':
        targetString = "./Build/Release/ASch"
    else:
        targetString = "./Build/Tests/" + buildTarget + "_UTest"
        env.AppendUnique(CPPPATH = ['./Catch', './FakeIt'])

    # Build object files into ./Build/Objects folder.
    objectFiles = []
    for sourceFile in sourceFiles:
        objectFiles.append(env.Object(target = "./Build/Objects/" + buildTarget + "/" + re.findall("(?<=\/)[^\/]*(?=.cpp)", sourceFile)[0], source = sourceFile))
    if buildTarget == 'ASch':
        objectFiles.append(CreateListFromFile(buildFiles["cmsis"]))

    output = env.Program(target = targetString, source = objectFiles)
    env.Alias(buildTarget, output)

    if buildTarget == 'ASch':
        binary = env.Binary(targetString, output)
        env.Requires(binary, output)
        env.Alias(buildTarget, binary)
        env.NoClean(binary)

    env.NoClean(output)

    return output
