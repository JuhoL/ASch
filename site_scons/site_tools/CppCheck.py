#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Title           : CppCheck.py
# Description     : This is SCons tool for running CppCheck
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolCppCheckWarning(SCons.Warnings.Warning):
    pass

class CppCheckNotFound(ToolCppCheckWarning):
    pass

SCons.Warnings.enableWarningClass(ToolCppCheckWarning)


def DetectCppCheck(env):
    try: 
        return env['CPPCHECK']
    except KeyError: 
        pass

    cppcheck = env.WhereIs('cppcheck', path = os.environ['PATH'])
    if cppcheck:
        return "\"" + cppcheck + "\""

    raise SCons.Errors.StopError(
        CppCheckNotFound,
        "Could not detect CppCheck")
    return None

cppcheck_builder = SCons.Builder.Builder(
        action = '$CPPCHECK $CPPCHECK_FLAGS --output-file=./TestReports/CppCheckLogs/$CPPCHECK_LOG $SOURCES $CPPCHECK_INC',
        suffix = '.log',
        src_suffix = '.cpp')

def generate(env):
    env.SetDefault(CPPCHECK = DetectCppCheck(env),
                   CPPCHECK_FLAGS = ['--language=c++', '--library=gnu', '--enable=all', '--suppress=missingIncludeSystem'])
    env.Append(BUILDERS = {'CppCheck' : cppcheck_builder})

def exists(env):
    return DetectCppCheck(env)
    