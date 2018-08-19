import sys
import os
import subprocess

sys.path.insert(0, './Scripts')
from BuildUtils import CreateDictionaryFromFile

def RunUnitTests(targetName, targetPath, clean):
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
    else:
        print ("\nCleanup coverage files...\n")
        subprocess.run("del Build\\Objects\\*" + targetName + "*.gc*", shell=True)

parameters = CreateDictionaryFromFile("./Build/SCons_UTest/UTestTargets.scons")

if 'clean' in sys.argv:
    clean = ' -c'
else:
    clean = ''

if (len(sys.argv) == 1) or (len(sys.argv) == 2) and clean != '':
    os.system("scons" + clean)
else:
    targetName = sys.argv[1]
    if targetName == "testall":
        print ("\nRun all unit tests...")
        for key in parameters.keys():
            targetPath = parameters[key][0]
            RunUnitTests(key, targetPath, clean)

        print ("\nMerge coverage xmls...")
        subprocess.run("python .\\TestReports\\CoberturaMerger.py -o ASch-coverage.xml -p ./TestReports", shell=True)
        print ("\nMerging complete:")
        subprocess.run("pycobertura show ./TestReports/ASch-coverage.xml", shell=True)
        print ("\nConverting XML into HTML...")
        subprocess.run("pycobertura show --format html --output ./TestReports/ASch-coverage.html ./TestReports/ASch-coverage.xml", shell=True)

        # Run static analyser.
        print ("\nRunning CppCheck for every file...\n")
        sourcePaths = []
        includePaths = []
        for path in parameters.values():
            if path not in sourcePaths:
                sourcePaths.append(path[0] + "\\sources")
                includePaths.append(path[0] + "\\include")
        subprocess.run("cppcheck --language=c++ --library=gnu --enable=all --suppress=missingIncludeSystem " + " ".join(sourcePaths) + " -I " + " -I ".join(includePaths), shell=True)

    elif targetName in parameters.keys():
        targetPath = parameters[targetName][0]
        RunUnitTests(targetName, targetPath, clean)
        # Run static analyser.
        print ("\nRunning CppCheck...\n")
        subprocess.run("cppcheck --language=c++ --library=gnu --enable=all --suppress=missingIncludeSystem " + targetPath + "\\sources\\" + targetName + ".cpp " + targetPath + "\\include\\" + targetName + ".h -I " + targetPath + "\\include\\", shell=True)

print ("\nAll done!\n")
