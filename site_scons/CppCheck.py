#!/usr/bin/env python
# Title           : BuildUtils.py
# Description     : These are utility functions used for ASch SCons build.
# Author          : Juho Lepistö
# Date            : 18 Aug 2018
# Python version  : 3.7
#==============================================================================

from ListCreator import CreateListFromFile

def CppCheck(env, buildTarget, buildFiles):
    sourceFiles = CreateListFromFile(buildFiles["sources"])
    includePaths = CreateListFromFile(buildFiles["include"])

    cppcheck = env.CppCheck(sourceFiles, CPPCHECK_LOG = buildTarget + '_CppCheck.log', CPPCHECK_INC = " -I " + " -I ".join(includePaths))
    env.Depends(cppcheck, buildTarget + "_executable")
    env.Alias(buildTarget, cppcheck)

    return cppcheck
