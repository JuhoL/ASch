#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Gcovr.py
# Description     : This is SCons tool for running gcovr.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolGcovrWarning(SCons.Warnings.Warning):
    pass

class GcovrNotFound(ToolGcovrWarning):
    pass

SCons.Warnings.enableWarningClass(ToolGcovrWarning)

def DetectGcovr(env):
    try: 
        return env['GCOVR']
    except KeyError: 
        pass

    gcovr = env.WhereIs('gcovr', path = os.environ['PATH'])
    if gcovr:
        return "\"" + gcovr + "\""

    raise SCons.Errors.StopError(
        GcovrNotFound,
        "Could not detect gcovr")
    return None

gcovr_builder = SCons.Builder.Builder(action = "$GCOVR -r . --filter=$GCOVR_SOURCE $GCOVR_FLAGS $TARGET",
                                      suffix = '.xml',
                                      src_suffix = '.cpp')

def generate(env):
    env.SetDefault(GCOVR = DetectGcovr(env),
                   GCOVR_FLAGS = '--xml-pretty -o')
    env.Append(BUILDERS = {'Gcovr' : gcovr_builder})

def exists(env):
    return DetectGcovr(env)
    