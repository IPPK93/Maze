#ifndef GRAPHICS_CELL_CPP
#define GRAPHICS_CELL_CPP


#include "graphics_cell.h"

Cell :: Cell(Point xy, Type t)
    :  Rectangle{xy, size, size}, type{t}
{ }

void Cell :: attach(Graph_lib::Window &win)
{
    attach(win);
}


#endif // GRAPHICS_CELL_CPP
