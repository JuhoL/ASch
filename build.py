import sys
import os
import subprocess

sys.path.insert(0, './Scripts')
from BuildUtils import CreateDictionaryFromFile

parameters = CreateDictionaryFromFile("./Build/SCons_UTest/UTestTargets.scons")

if 'clean' in sys.argv:
    clean = ' -c'
else:
    clean = ''

if (len(sys.argv) == 1) or (len(sys.argv) == 2) and clean != '':
    os.system("scons" + clean)
else:
    targetName = sys.argv[1]
    if targetName in parameters.keys():
        targetPath = parameters[sys.argv[1]][0]

        # Run the SCons test build.
        subprocess.run("scons test=" + targetName + clean, shell=True)

        if clean == '':
            # Run the test.
            print ("\nRunning the tests...\n")
            subprocess.run(".\\Build\\Tests\\UTest_" + targetName + ".exe -d yes --order rand --rng-seed time", shell=True)

            print ("\nCreating coverage report...\n")

            # Run gcov
            subprocess.run("gcov " + targetPath + "\\tests\\UTest_" + targetName + ".cpp" + " -r -o .\\Build\\Objects")
            # Run gcovr
            subprocess.run("gcovr -r . --filter=(.+/)?" + targetName + "\\.cpp$ --xml-pretty -o " + targetName + "_TestCoverage.xml")
            
            # Move the XML and cleanup coverage artifacts.
            subprocess.run("move " + targetName + "_TestCoverage.xml TestReports", shell=True)

            print ("\nAll done!\n")
        else:
            print ("\nCleanup coverage files...\n")
            subprocess.run("del Build\\Objects\\*" + targetName + "*.gc*", shell=True)
            print ("\nAll done!\n")
