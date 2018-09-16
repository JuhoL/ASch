#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : RunUnitTest.py
# Description     : This is SCons tool for running unit tests.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

from Utils import GetTestExecutableName

def RunUnitTest(env, buildTarget):
    testRun = env.Command(target = './TestReports/UnitTestLogs/' + buildTarget + '_UTest.log',
                          source = "./Build/Tests/" + buildTarget + "_UTest.exe",
                          action = ["$SOURCE -o $TARGET", "type $TARGET"])
    executable = GetTestExecutableName(buildTarget)
    env.Depends(testRun, executable)
    return testRun
