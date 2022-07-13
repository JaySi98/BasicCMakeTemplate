#include <iostream>
#include <Include/Config.h>
#include <SimpleClass.h>

int main(int argc, char* argv[])
{
    std::cout << argv[0] << " Version: " 
              << Template_VERSION_MAJOR << "."
              << Template_VERSION_MINOR << std::endl;

    // testing if files are included correctly
    SimpleClass simpleClass;
    simpleClass.PrintNumber();

   return EXIT_SUCCESS;
}
