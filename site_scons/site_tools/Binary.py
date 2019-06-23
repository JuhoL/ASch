#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Binary.py
# Description     : This is SCons tool for creating ARM binary from ELF.
# Author          : Juho Lepistö
# Date            : 16 Jun 2019
# Python version  : 3.7
#==============================================================================

import os
import SCons.Action
import SCons.Builder
import SCons.Util

class ToolObjcopyWarning(SCons.Warnings.Warning):
    pass

class ObjcopyNotFound(ToolObjcopyWarning):
    pass

SCons.Warnings.enableWarningClass(ToolObjcopyWarning)


def DetectObjcopy(env):
    try: 
        return env['OBJCOPY']
    except KeyError: 
        pass

    objcopy = env.WhereIs('arm-eabi-objcopy', path = os.environ['PATH'])
    if objcopy:
        return "\"" + objcopy + "\""

    raise SCons.Errors.StopError(
        ObjcopyNotFound,
        "Could not detect arm-eabi-objcopy.")
    return None

binary_builder = SCons.Builder.Builder(action = '$OBJCOPY -O binary $SOURCE $TARGET',
                                     suffix = '.bin',
                                     src_suffix = '.elf')

def generate(env):
    env.SetDefault(OBJCOPY = DetectObjcopy(env))
    env.Append(BUILDERS = {'Binary' : binary_builder})

def exists(env):
    return DetectObjcopy(env)
    