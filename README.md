Description:

Prerequisites:


How to build project without using included scripts:

First config:
    cmake -S <path/to/CMakeList> -B <output/path>

Set cmake variable:   
    cmake -D<varibiable name>=OFF/ON

Build:
    make -C <output/path>

Run project:
    ./<output/path>/<project name>

How to build project using included scripts:
First config:
    Scripts/config.sh

Build:
    Scripts/build.sh

Run project:
    Scripts/run.sh

Run tests:
    Scripts/runTests.sh

