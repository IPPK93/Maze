#ifndef MAIN_CPP
#define MAIN_CPP


#include <iostream>
#include "graphics_field.h"

int main()
try
{
    srand(unsigned(time(nullptr)));
    Field field(Point{240, 0}, 100, 60); //min_size of maze is 15, max width 100, max height 60
    return Fl :: run();
}
catch(std::exception& e)
{
    std :: cerr << "Exception: " << e.what() << std :: endl;
    return 1;
}
catch(...)
{
    std :: cerr << "Unknown error" << std :: endl;
    return 2;
}


#endif // MAIN_CPP
