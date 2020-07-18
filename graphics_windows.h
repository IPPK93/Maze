#ifndef GRAPHICS_WINDOWS_H
#define GRAPHICS_WINDOWS_H


#include "graphics_cell.h"

struct My_Window : Graph_lib :: Window
{
    My_Window(Point xy, int w, int h, const std::string& title)
        : Window{xy, w, h, title},
          quit_button{Point{x_max() - 70, 40}, 70, 20, "Quit", cb_quit},
          max_acceptedx{disp_xres - w}, max_acceptedy{disp_yres - h}
    {
        attach(quit_button);

        if((0 > xy.x || xy.x > max_acceptedx) || (0 > xy.y || xy.y > max_acceptedy)) throw std :: runtime_error("Uncorrect Point");
    }

    Graph_lib :: Button quit_button;

    static constexpr int disp_xres = 1920;
    static constexpr int disp_yres = 1000;

private:
    const int max_acceptedx;
    const int max_acceptedy;

    static void cb_quit(Graph_lib :: Address, Graph_lib :: Address addr)
    {
        My_Window& win = Graph_lib :: reference_to<My_Window> (addr);
        win.quit();
    }
    void quit() { hide(); }
};

//our menu
struct Menu_Window : Graph_lib :: Window
{
    Menu_Window(const std :: string& title)
        : Window{Point{(My_Window :: disp_xres)/3, (My_Window :: disp_yres)/4}, 300, 200, title},
          start_button{Point{0, 0}, 300, 100, "Start", cb_start},
          quit_button{Point{0, 100}, 300, 100, "Quit", cb_quit}
    {
        size_range(300, 200, 300, 200);

        attach(start_button);
        attach(quit_button);
    }


    Graph_lib :: Button start_button;
    Graph_lib :: Button quit_button;

    void wait_for_button()
    {
        while (!quit_pushed && Fl::wait());
    }

    bool q_pushed() const { return quit_pushed; }
    bool s_pushed() const { return start_pushed; }

private:
    bool quit_pushed{ false };
    bool start_pushed{ false };

    static void cb_start(Graph_lib :: Address, Graph_lib :: Address addr)
    {
        Menu_Window& win = Graph_lib :: reference_to<Menu_Window> (addr);
        win.start();
    }
    static void cb_quit(Graph_lib :: Address, Graph_lib :: Address addr)
    {
        Menu_Window& win = Graph_lib :: reference_to<Menu_Window> (addr);
        win.quit();
    }

    void quit() { quit_pushed = true; this->hide(); }
    void start() { start_pushed = true; this->hide(); }

};


#endif // GRAPHICS_WINDOWS_H
