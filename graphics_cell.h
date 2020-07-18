#ifndef GRAPHICS_CELL_H
#define GRAPHICS_CELL_H


#include <exception>

#include "../Graph_lib/Simple_window.h"
#include "../Graph_lib/Graph.h"

using Graph_lib :: Point;

struct Cell : Graph_lib :: Rectangle
{
    static constexpr int size = 4;

    enum Type
    {
        wall,
        path
    };

    Cell(Point xy, Type t);
    void attach(Graph_lib :: Window& win);
    Type type;
};


#endif // GRAPHICS_CELL_H
