#ifndef GRAPHICS_FIELD_H
#define GRAPHICS_FIELD_H


//generate new maze and start/quit buttons in menu

#include "generating_algorithm.h"
#include "graphics_windows.h"


class Field : My_Window
{
public:
    Field(Point xy, int m_szw, int m_szh);

private:
    int maze_size_w;
    int maze_size_h;
    int times_drawed{0};
    static constexpr int margin = 30;
    static constexpr int min_val = 5; // m_sz min_val
    static constexpr int wmax_val = 100; // m_szw max_val
    static constexpr int hmax_val = 60; // m_szh max_val

    Maze maze;  //engine

    Menu_Window menu_window;
    Graph_lib :: Vector_ref<Cell> cells;
    Graph_lib :: Rectangle horizontal;
    Graph_lib :: Rectangle vertical;
    Graph_lib :: Rectangle st_rect;
    Graph_lib :: Open_polyline* right_path;


    Graph_lib :: Button new_maze_button;
    Graph_lib :: Button show_path_button;
    Graph_lib :: Button hide_path_button;
    Graph_lib :: Menu theme_menu;
    Graph_lib :: Button menu_button;
    Graph_lib :: Text start;
    Graph_lib :: Text finish;

    void theme_color(Graph_lib :: Color, Graph_lib :: Color);

    //callback functions
    static void cb_show_path(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_hide_path(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_theme1(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_theme2(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_theme3(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_default_theme(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_menu(Graph_lib :: Address, Graph_lib :: Address addr);
    static void cb_new_maze(Graph_lib :: Address, Graph_lib :: Address addr);

    //functions for callback
    void theme1();
    void theme2();
    void theme3();
    void default_theme();
    void menu_pressed() { menu_button.hide(); theme_menu.show(); }
    void show_path();
    void hide_path();
    void hide_menu() { theme_menu.hide(); menu_button.show(); }
    void draw_maze();

    //theme checkers
    Graph_lib :: Color theme_col1{0};
    Graph_lib :: Color theme_col2{0};
};


#endif // GRAPHICS_FIELD_H
