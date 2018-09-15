#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Title           : BuildUtils.py
# Description     : These are utility functions used for ASch SCons build.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
#==============================================================================

def CreateListFromFile(fileName):
    print ("-------------- Opening " + fileName)
    includes = []
    with open(fileName) as includeFile:
        for line in includeFile:
            includes.append(line.translate(str.maketrans('', '', '\r\n')))
            print ("-------------- Latest include: " + includes[-1])

    return includes

def CreateDictionaryFromFile(fileName):
    print ("-------------- Opening " + fileName)
    targets = {}
    with open(fileName) as targetsFile:
        for line in targetsFile:
            elements = line.split( )
            targets[elements[0]] = elements[1:]
    return targets
