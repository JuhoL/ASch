# ASch

**A**ll-purpose **Sch**eduler, **ASch**, is a simple event-driven single-thread scheduler for embedded devices. Designed mainly for learning embedded C++.

### Prerequisites

- MinGW
- GCC for C++
- ARM GCC
- SCons 3.0.1
- Catch2 (included)
- FakeIt (included)
- CppCheck
- Python 3.7
    - PyCobertura

## Building
### Required Tools
* [MinGW](http://www.mingw.org/) - Build environment for unit tests in Windows
* [ARM-GCC](https://sourceforge.net/projects/mingw-gcc-arm-eabi/) - ARM GCC for MinGW for builduing releases
* [SCons](https://scons.org/) - Build scripting
* [CppCheck](http://cppcheck.sourceforge.net/) - Static analyser

### Building Release
To build all:
```
python ./build.py
```
To clean all
```
python ./build.py clean
```

### Building Modules and Running Unit Tests
To build a module and run its unit tests:
```
python ./build.py <module_name>
```
To run all the tests and generate a coverage report:
```
python ./build.py testall
```
To clean build and unit test artifacts
```
python ./build.py <module_name/tessall> clean
```

## Authors
* **Juho Lepistö** - *Initial work* - [JuhoL](https://github.com/JuhoL)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
