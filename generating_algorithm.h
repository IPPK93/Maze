#ifndef GENERATING_ALGORITHM_H
#define GENERATING_ALGORITHM_H


#include <vector>
#include "time.h"

//Engine class
class Maze
{
public:
    Maze(int m_w, int m_h);

    void build_maze();
    void clean_maze();

    //to get access in other classes
    const std :: vector<char>& w_p_vector() const;
    const std :: vector<std :: pair<int, int>>& path_vector() const;

private:
    //note that array will have points from (0,0) to (maze_width-1,maze_height-1)
    int maze_height;
    int maze_width;
    int visitedCellN{0};        //number of already visited cells

    enum
    {
        //give paths its identifiers
        path_up = 4,
        path_down = 1,
        path_left = 2,
        path_right = 3,
        visited_cell = 100,

        //for w_p_chk. We need to minimize magic symbols
        wall = 'w',
        path = 'p'
    };

    int except_path{0};
    int rand_int(int n_min, int n_max);
    int get_rand_dir(int n_min, int n_max, int& except);

    std :: vector<std :: pair<int,int>> m_stack;            // randomly visits all cells
    std :: vector<int> visit_chk;                           // says whether it's a path or not, changes walls into paths
    std :: vector<std :: pair<int, int>> ordered_path;      // has directed values to build blocks and cuts walls where needed
    std :: vector<char> w_p_chk;                            // we cut walls here (values go to the graphics via w_p_vector() function)
    std :: vector<std :: pair<int, int>> maze_path;         // right path of our maze (values go to the graphics via path_vector() function)

    void set_start_point();

    bool can_go(int dir) const;
    void next_step(int dir);

    void cut_walls(int init_x, int init_y);
};


#endif // GENERATING_ALGORITHM_H
