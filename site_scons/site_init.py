#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Title           : site_init.py
# Description     : This is a SCons init script for building ASch.
# Author          : Juho Lepistö
# Date            : 16 Sep 2018
# Python version  : 3.7
# SCons version   : 3.0.1  
#==============================================================================

AddOption('--test',
          dest = 'test',
          nargs = 0,
          help = 'Runs unit tests to the target.')

AddOption('--linux',
          dest = 'linux',
          nargs = 0,
          help = 'Disables mingw tool.')

AddOption('--cpp_check',
          dest = 'cpp_check',
          nargs = 0,
          help = 'Enables CppCheck.')

AddOption('--coverage',
          dest = 'coverage',
          nargs = 0,
          help = 'Enables Coverage report generation.')
