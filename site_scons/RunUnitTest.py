#!/usr/bin/env python
# Title           : BuildUtils.py
# Description     : This is SCons tool for running unit tests.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

def RunUnitTest(env, buildTarget):
    testRun = env.Command(target = buildTarget + '_UTest.log',
                          source = "./Build/Tests/" + buildTarget + "_UTest.exe",
                          action = ["$SOURCE -o ./TestReports/UnitTestLogs/$TARGET", "type .\\TestReports\\UnitTestLogs\\$TARGET"])
    env.Depends(testRun, buildTarget + "_executable")
    env.Alias(buildTarget, testRun)
    return
