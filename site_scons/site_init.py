#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : site_init.py
# Description     : This is a SCons init script for building ASch.
# Author          : Juho Lepistö
# Date            : 16 Sep 2018
# Python version  : 3.7
# SCons version   : 3.0.1  
#==============================================================================

AddOption('--step',
          dest = 'step',
          type = 'string',
          nargs = 1,
          action = 'store',
          help = 'Runs the given step of the build. Available steps: \
                  build, unit_test, cppcheck, coverage')
