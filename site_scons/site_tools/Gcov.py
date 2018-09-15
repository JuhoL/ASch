#!/usr/bin/env python
# Title           : Gcov.py
# Description     : This is SCons tool for running gcov.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolGcovWarning(SCons.Warnings.Warning):
    pass

class GcovNotFound(ToolGcovWarning):
    pass

SCons.Warnings.enableWarningClass(ToolGcovWarning)


def DetectGcov(env):
    try: 
        return env['GCOV']
    except KeyError: 
        pass

    gcov = env.WhereIs('gcov', path = os.environ['PATH'])
    if gcov:
        return "\"" + gcov + "\""

    raise SCons.Errors.StopError(
        GcovNotFound,
        "Could not detect gcov")
    return None

gcov_builder = SCons.Builder.Builder(action = '$GCOV $SOURCES -r -o $GCOV_OBJECTS_DIR',
                                     suffix = '.gcov',
                                     src_suffix = '.cpp')

def generate(env):
    env.SetDefault(GCOV = DetectGcov(env))
    env.Append(BUILDERS = {'Gcov' : gcov_builder})

def exists(env):
    return DetectGcov(env)
    