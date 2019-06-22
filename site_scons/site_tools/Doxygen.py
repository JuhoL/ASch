#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Doxygen.py
# Description     : This is SCons builder for Doxygen.
# Author          : Juho Lepistö
# Date            : 22 Jun 2019
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolDoxygenWarning(SCons.Warnings.Warning):
    pass

class DoxygenNotFound(ToolDoxygenWarning):
    pass

SCons.Warnings.enableWarningClass(ToolDoxygenWarning)

def DetectDoxygen(env):
    try: 
        return env['DOXYGEN']
    except KeyError: 
        pass

    doxygen = env.WhereIs('doxygen', path = os.environ['PATH'])
    if doxygen:
        return "\"" + doxygen + "\""

    raise SCons.Errors.StopError(
        DoxygenNotFound,
        "Could not detect Doxygen")
    return None

doxygen_builder = SCons.Builder.Builder(action = "$DOXYGEN $SOURCE",
                                        suffix = '.html',
                                        src_suffix = '.dox')

def generate(env):
    env.SetDefault(DOXYGEN = DetectDoxygen(env))
    env.Append(BUILDERS = {'Doxygen' : doxygen_builder})

def exists(env):
    return DetectDoxygen(env)
    