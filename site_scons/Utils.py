#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Utils.py
# Description     : These are utility tools for SCons build.
# Author          : Juho Lepistö
# Date            : 16 Sep 2018
# Python version  : 3.7
#==============================================================================

def GetTestExecutableName(buildTarget):
    return "Build\\Tests\\" + buildTarget + "_UTest.exe"
