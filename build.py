import sys
import os
import subprocess

parameters = {'queue': ['ASch_Queue', '.\\Build\\Tests\\', '.\\Utils\\tests\\']}

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
            subprocess.run(target[1] + "UTest_" + target[0] + ".exe", shell=True)

            print ("\nCreating coverage report...\n")

            # Run gcov
            subprocess.run("gcov " + target[2] + "UTest_" + target[0] + ".cpp" + " -r")
            # Run gcovr
            subprocess.run("gcovr -r . --filter=(.+/)?" + target[0] + "\\.cpp$ --xml-pretty -o " + target[0] + "_TestCoverage.xml")
            
            # Copy the XML and cleanup artifacts.
            #subprocess.run("cp " + target[0] + "_TestCoverage.xml ./Build/Tests/", shell=True)
            #subprocess.run('rm *.gcov', shell=True, encoding='utf-8')
