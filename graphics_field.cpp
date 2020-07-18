#ifndef GRAPHICS_FIELD_CPP
#define GRAPHICS_FIELD_CPP


#include "graphics_field.h"

using Graph_lib :: Color;

//to choose color
Cell :: Type cell_type(const std :: vector<char>& v, int n_elem)
{
    if(v[unsigned(n_elem)] == 'p')
        return Cell :: path;
    else
        return Cell :: wall;
}

//program works here
Field :: Field(Point xy, int m_szw, int m_szh)
    : My_Window{xy, 3*m_szw*(Cell :: size) + 200, 3*m_szh*(Cell :: size) + 80, "Maze"},
      maze_size_w{3*m_szw}, maze_size_h{3*m_szh}, maze(m_szw, m_szh), menu_window{"Maze"},
      horizontal{Point{margin+2*(Cell :: size), margin + 3*m_szh*(Cell :: size)}, (3*m_szw-2)*(Cell :: size), Cell :: size},
      vertical{Point{margin - (Cell :: size), margin}, Cell :: size, (3*m_szh+1)*(Cell :: size)},
      st_rect{Point{x_max() - (170 + (Cell :: size)), margin + (Cell :: size)}, Cell :: size, 2*(Cell :: size)},
      new_maze_button{Point{x_max() - 70, 20}, 70, 20, "New Maze", cb_new_maze},
      show_path_button{Point{x_max() - 70, 0}, 70, 20, "Show Path", cb_show_path},
      hide_path_button{Point{x_max() - 70, 0}, 70, 20, "Hide Path", cb_hide_path},
      theme_menu{Point{0, 0}, 70, 20, Graph_lib :: Menu :: horizontal, "Theme"},
      menu_button{Point{0, 0}, 70, 20, "Themes", cb_menu},
      start{Point{x_max() - 170, margin + (Cell :: size)}, "START"},
      finish{Point{margin - (Cell :: size), y_max() - margin}, "FINISH"}
{
    if(min_val > m_szw || m_szw > wmax_val) throw std :: runtime_error("maze width size error");
    if(min_val > m_szh || m_szh > hmax_val) throw std :: runtime_error("maze height size error");

    //don't allow to change initial resolution
    size_range(maze_size_w*(Cell :: size) + 200, maze_size_h*(Cell :: size) + 80, maze_size_w*(Cell :: size) + 200, maze_size_h*(Cell :: size) + 80);

    //only one window should be open at a time
    this->hide();
    menu_window.show();
    menu_window.wait_for_button();

    if(menu_window.s_pushed() == true)
    {
        this->show();

        //attaching buttons and menu
        attach(show_path_button);
        attach(hide_path_button);
        hide_path_button.hide();
        theme_menu.attach(new Graph_lib :: Button{Point{0, 0}, 0, 0, "Theme #1", cb_theme1});
        theme_menu.attach(new Graph_lib :: Button{Point{0, 0}, 0, 0, "Theme #2", cb_theme2});
        theme_menu.attach(new Graph_lib :: Button{Point{0, 0}, 0, 0, "Theme #3", cb_theme3});
        theme_menu.attach(new Graph_lib :: Button{Point{0, 0}, 0, 0, "Default", cb_default_theme});
        attach(theme_menu);
        theme_menu.hide();
        attach(menu_button);
        attach(new_maze_button);
        attach(quit_button);
        attach(start);
        attach(finish);

        //attaching edges and makes needed start color
        horizontal.set_fill_color(Color(FL_DARK3));
        horizontal.set_color(Color :: invisible);
        vertical.set_fill_color(Color(FL_DARK3));
        vertical.set_color(Color :: invisible);
        st_rect.set_fill_color(FL_BACKGROUND_COLOR);
        st_rect.set_color(Color :: invisible);
        attach(horizontal);
        attach(vertical);
        attach(st_rect);

        //creates cells
        for (int i = 0; i < maze_size_h; ++i)
            for (int j = 0; j < maze_size_w; ++j)
            {
                cells.push_back(new Cell{Point{margin + j*(Cell :: size), margin + (maze_size_h-1 - i)*(Cell :: size)},
                                         cell_type(maze.w_p_vector(), i*maze_size_w + j)});
                cells[i*maze_size_w + j].set_color(Color :: invisible);
                attach(cells[i*maze_size_w + j]);
            }
        right_path = new Graph_lib :: Open_polyline;
        draw_maze();
        default_theme();
    }
    else this->hide();
}

void Field :: draw_maze()
{
    //needs when drawing new maze
    if(times_drawed != 0)
    {
        maze.clean_maze();
        hide_path();

        //delete old
        delete right_path;
        //and write new path
        right_path = new Graph_lib :: Open_polyline;
    }
    maze.build_maze();

    //sets color to our cells
    for(int i = 0; i < maze_size_h; ++i)
        for(int j = 0; j < maze_size_w; ++j)
            cells[i*maze_size_w + j].type = cell_type(maze.w_p_vector(), i*maze_size_w + j);
    theme_color(theme_col1, theme_col2);

    //draws and attaches right path
    for(int i = 0; i < maze.path_vector().size(); ++i)
    {
        right_path->add(Point{margin + (3*maze.path_vector()[i].first+1)*(Cell :: size),
                              y_max() - 50 - (3*maze.path_vector()[i].second + 1)*(Cell :: size)});
    }
    right_path->set_color(Color::invisible);
    attach(*right_path);

    Fl :: redraw();
    ++times_drawed;
    return ;
}


//changes cells colors
void Field :: theme_color(Graph_lib::Color wall_col, Graph_lib::Color path_col)
{
    //remember last color
    theme_col1 = wall_col;
    theme_col2 = path_col;

    //change color
    for(int i = 0; i < maze_size_h; ++i)
        for(int j = 0; j < maze_size_w; ++j)
        {
            if(cell_type(maze.w_p_vector(), i*maze_size_w + j) == (Cell :: wall))
                cells[i*maze_size_w+j].set_fill_color(Color(wall_col));
            else
                cells[i*maze_size_w+j].set_fill_color(Color(path_col));
        }
    //of the edges too
    horizontal.set_fill_color(Color(wall_col));
    vertical.set_fill_color(Color(wall_col));

    return ;
}


void Field :: cb_show_path(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.show_path();
}

void Field :: cb_hide_path(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.hide_path();
}

void Field :: cb_theme1(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.theme1();
}

void Field :: cb_theme2(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.theme2();
}

void Field :: cb_theme3(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.theme3();
}

void Field :: cb_menu(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.menu_pressed();
}

void Field :: cb_default_theme(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.default_theme();
}

void Field :: cb_new_maze(Graph_lib::Address, Graph_lib::Address addr)
{
    Field& win = Graph_lib :: reference_to<Field>(addr);
    win.draw_maze();
}


void Field :: theme1()
{
    theme_color(FL_BLACK, FL_WHITE);
    theme_menu.hide();
    menu_button.show();
    hide_path();
    put_on_top(st_rect);
    return ;
}

void Field :: theme2()
{
    theme_color(FL_DARK_BLUE, FL_BLACK);
    theme_menu.hide();
    menu_button.show();
    hide_path();
    put_on_top(st_rect);
    return ;
}

void Field :: theme3()
{
    theme_color(FL_DARK_MAGENTA, FL_LIGHT2);
    theme_menu.hide();
    menu_button.show();
    hide_path();
    put_on_top(st_rect);
    return ;
}

void Field :: default_theme()
{
    theme_color(FL_DARK3, FL_LIGHT1);
    theme_menu.hide();
    menu_button.show();
    hide_path();
    put_on_top(st_rect);
    return ;
}

void Field :: show_path()
{
    right_path->set_color(Color(FL_RED));
    attach(*right_path);
    show_path_button.hide();
    hide_path_button.show();
    return ;
}

void Field :: hide_path()
{
    right_path->set_color(Color :: invisible);
    hide_path_button.hide();
    show_path_button.show();
    return ;
}


#endif // GRAPHICS_FIELD_CPP
