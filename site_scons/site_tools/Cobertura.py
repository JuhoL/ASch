#!/usr/bin/env python
# Title           : Cobertura.py
# Description     : This is SCons tool for running pycobertura.
# Author          : Juho Lepistö
# Date            : 15 Sep 2018
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolCoberturaWarning(SCons.Warnings.Warning):
    pass

class CoberturaNotFound(ToolCoberturaWarning):
    pass

SCons.Warnings.enableWarningClass(ToolCoberturaWarning)

def DetectCobertura(env):
    try: 
        return env['COBERTURA']
    except KeyError: 
        pass

    cobertura = env.WhereIs('pycobertura', path = os.environ['PATH'])
    if cobertura:
        return "\"" + cobertura + "\""

    raise SCons.Errors.StopError(
        CoberturaNotFound,
        "Could not detect pycobertura")
    return None

cobertura_builder = SCons.Builder.Builder(action = ["$COBERTURA show $SOURCES"], src_suffix = '.xml')

def generate(env):
    env.SetDefault(COBERTURA = DetectCobertura(env))
    env.Append(BUILDERS = {'Cobertura' : cobertura_builder})

def exists(env):
    return DetectCobertura(env)
    