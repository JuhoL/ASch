#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Coverage.py
# Description     : This is SCons tool for generating coverage reports.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

from Utils import GetTestExecutableName

def Gcov(env, buildTarget, targetPath):
    testSource = targetPath + "/tests/UTest_" + buildTarget + ".cpp"
    objectsDir = "./Build/Objects/" + buildTarget

    gcov = env.Gcov(testSource, GCOV_OBJECTS_DIR = objectsDir)

    return gcov

def GenerateCoverageXml(env, buildTarget, targetPath):
    testSource = targetPath[2:] + "/tests/UTest_" + buildTarget + ".cpp"
    reportFile = "./TestReports/CoverageLogs/" + buildTarget + "_TestCoverage"

    gcovXml = env.Gcovr(reportFile, testSource, GCOVR_SOURCE = testSource)

    return gcovXml

def ShowModuleCoverage(env, buildTarget):
    sourceXml = "./TestReports/CoverageLogs/" + buildTarget + "_TestCoverage"
    htmlFile = "./TestReports/CoverageLogs/" + buildTarget + "_TestCoverage"

    coverageHtml = env.Cobertura(htmlFile, sourceXml)

    return coverageHtml

def GenerateCoverageReport(env, buildTarget, targetPath):
    executable = GetTestExecutableName(buildTarget)

    gcov = Gcov(env, buildTarget, targetPath)
    env.Depends(gcov, executable)
    env.Alias(buildTarget, gcov)

    gcovXml = GenerateCoverageXml(env, buildTarget, targetPath)
    env.Depends(gcovXml, gcov)
    env.Alias(buildTarget, gcovXml)
    env.AlwaysBuild(gcovXml)

    coverageHtml = ShowModuleCoverage(env, buildTarget)
    env.Depends(coverageHtml, gcovXml)
    env.Alias(buildTarget, coverageHtml)
    env.AlwaysBuild(coverageHtml)
