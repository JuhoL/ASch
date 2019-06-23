#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : Doxygen.py
# Description     : This is SCons tool for Doxygen.
# Author          : Juho Lepistö
# Date            : 22 Jun 2019
# Python version  : 3.7
#==============================================================================

def Doxygen(env, buildTarget):
    doxygen = env.Doxygen("./Documents/Doxygen/" + buildTarget + ".dox")
    env.Alias(buildTarget, doxygen)
