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

def BuildTarget(env, buildTarget, buildFiles):
    # Parse source files to be built from Sources.scons, include directories from Inlude.scons and CcFlags.scons files.
    try:
        sourceFiles = CreateListFromFile(buildFiles["sources"])
    except Exception as e:
        print (e)
        print ("ERROR: Gathering source files failed!")
        print ("       Make sure you have created .scons file listing in Build/Scons_UTest folder.")
        sys.exit(1)

    try:
        env.AppendUnique(CPPPATH = CreateListFromFile(buildFiles["include"]))
    except Exception as e:
        print (e)
        print ("ERROR: Gathering include directories failed!")
        print ("       Make sure you have created .scons directory listing in Build/Scons_UTest folder.")
        sys.exit(1)

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

    env.NoClean(output)
    env.NoClean(binary)

    return output
