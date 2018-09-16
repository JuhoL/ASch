#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : CppCheck.py
# Description     : These is SCons tool for running CppCheck.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
#==============================================================================

from ListCreator import CreateListFromFile

def CppCheck(env, buildTarget, buildFiles):
    sourceFiles = CreateListFromFile(buildFiles["sources"])
    includePaths = CreateListFromFile(buildFiles["include"])
    cppcheck = env.CppCheck(sourceFiles, CPPCHECK_LOG = buildTarget + '_CppCheck.log', CPPCHECK_INC = " -I " + " -I ".join(includePaths))
    return cppcheck
