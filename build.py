import sys
import os
import subprocess

parameters = {'queue': ['ASch_Queue', '.\\Utils']}

if 'clean' in sys.argv:
    clean = ' -c'
else:
    clean = ''

if (len(sys.argv) == 1) or (len(sys.argv) == 2) and clean != '':
    os.system("scons" + clean)
else:
    if sys.argv[1] in parameters.keys():
        target = parameters[sys.argv[1]]

        # Run the SCons test build.
        subprocess.run("scons test=" + target[0] + clean, shell=True)

        if clean == '':
            # Run the test.
            print ("\nRunning the tests...\n")
            subprocess.run(".\\Build\\Tests\\UTest_" + target[0] + ".exe -d yes --order rand --rng-seed time", shell=True)

            print ("\nCreating coverage report...\n")

            # Run gcov
            subprocess.run("gcov " + target[1] + "\\tests\\UTest_" + target[0] + ".cpp" + " -r")
            # Run gcovr
            subprocess.run("gcovr -r . --filter=(.+/)?" + target[0] + "\\.cpp$ --xml-pretty -o " + target[0] + "_TestCoverage.xml")
            
            print ("\nClean up...\n")
            # Move the XML and cleanup coverage artifacts.
            subprocess.run("move " + target[0] + "_TestCoverage.xml TestReports", shell=True)
            subprocess.run("del *.gcov", shell=True)
            subprocess.run("del " + target[1] + "\\tests\\*.gc*", shell=True)
            subprocess.run("del " + target[1] + "\\sources\\*.gc*", shell=True)

            # Cleanup build artifacts.
            subprocess.run("scons test=" + target[0] + " -c", shell=True)

            print ("\nAll done!\n")
