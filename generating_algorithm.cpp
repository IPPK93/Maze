#ifndef GENERATING_ALGORITHM_CPP
#define GENERATING_ALGORITHM_CPP


#include "generating_algorithm.h"
#include <iostream>
#include <random>

void vector_add_point (std :: vector<std :: pair<int, int>>& v, int plus_x, int plus_y)
{
    v.push_back(std :: make_pair(v.back().first + plus_x, v.back().second + plus_y));
}

int Maze :: rand_int(int n_min, int n_max)
{
    int a = rand()%RAND_MAX;
    static std::default_random_engine ran(a);
    return std::uniform_int_distribution<>{n_min, n_max}(ran);
}

//gets random direction
int Maze :: get_rand_dir(int n_min, int n_max, int& except)
{
    int a{except};
    while(a == except) a = rand_int(n_min, n_max);

    //NOTE: our next direction won't be opposite our current direction
    switch(a)
    {
    case path_up:
        except = path_down;
        break;
    case path_down:
        except = path_up;
        break;
    case path_left:
        except = path_right;
        break;
    case path_right:
        except = path_left;
        break;
    default:
        throw std :: runtime_error("Unknown path");
    }

    return a;
}

Maze :: Maze(int m_w, int m_h)
    : maze_width{m_w}, maze_height{m_h},
      visit_chk(m_w*m_h), w_p_chk(9*(m_w*m_h), wall)
{ }                       //4 for path, 4(5) for walls, all start values=wall


// makes start point
void Maze :: set_start_point()
{
    //say start cell is already visited
    m_stack.push_back(std :: make_pair(0, 0));
    visit_chk.front() = visited_cell;
    ++visitedCellN;
    ordered_path.push_back(std :: make_pair(0, 0));
    maze_path.push_back(std :: make_pair(0, 0));
    cut_walls(0, 0);
}

// checks and makes next step
void Maze :: next_step(int dir)
{
    switch (dir)
    {
    case path_up:
        vector_add_point(m_stack, 0, -1);
        ordered_path.push_back(m_stack.back());
        visit_chk[m_stack.back().second*maze_width + m_stack.back().first] = visited_cell;
        ++visitedCellN;
        break;
    case path_down:
        vector_add_point(m_stack, 0, 1);
        ordered_path.push_back(m_stack.back());
        visit_chk[m_stack.back().second*maze_width + m_stack.back().first] = visited_cell;
        ++visitedCellN;
        break;
    case path_left:
        vector_add_point(m_stack, -1, 0);
        ordered_path.push_back(m_stack.back());
        visit_chk[m_stack.back().second*maze_width + m_stack.back().first] = visited_cell;
        ++visitedCellN;
        break;
    case path_right:
        vector_add_point(m_stack, 1, 0);
        ordered_path.push_back(m_stack.back());
        visit_chk[m_stack.back().second*maze_width + m_stack.back().first] = visited_cell;
        ++visitedCellN;
        break;
    default:
        throw std :: runtime_error("Wrong path");
    }

    //add need points to the path
    if(!(maze_path.back().first == maze_width - 1 && maze_path.back().second == maze_height - 1))
        maze_path.push_back(m_stack.back());
}

// checks whether or no we can make next step
bool Maze :: can_go(int dir) const
{
    if(dir == path_up && m_stack.back().second > 0)
    {
        if(visit_chk[(m_stack.back().second-1)*maze_width + m_stack.back().first] != visited_cell)
            return true;
    }
    else if(dir == path_down && m_stack.back().second < maze_height-1)
    {
        if(visit_chk[(m_stack.back().second+1)*maze_width + m_stack.back().first] != visited_cell)
            return true;
    }
    else if(dir == path_left && m_stack.back().first > 0)
    {
        if(visit_chk[m_stack.back().second*maze_width + (m_stack.back().first-1)] != visited_cell)
            return true;
    }
    else if(dir == path_right && m_stack.back().first < maze_width-1)
    {
        if(visit_chk[m_stack.back().second*maze_width + (m_stack.back().first+1)] != visited_cell)
            return true;
    }
    return false;
}

//key function
void Maze :: build_maze()
{
    if(visitedCellN == 0) set_start_point();

    //builds maze
    while(true)
    {
        if(visitedCellN < maze_width * maze_height)
        {
            // randomly choose direction
            int dir = get_rand_dir(path_down, path_up, except_path);

            if(can_go(dir))
            {
                next_step(dir);
                cut_walls(m_stack.back().first, m_stack.back().second);
            }
            else if(!(can_go(path_up) || can_go(path_down) || can_go(path_left) || can_go(path_right)))
            {
                m_stack.pop_back(); //goes back
                ordered_path.push_back(m_stack.back());
                cut_walls(ordered_path.back().first, ordered_path.back().second);
                if(!(maze_path.back().first == maze_width - 1 && maze_path.back().second == maze_height - 1))
                    maze_path.pop_back();
            }
        }
        else break;
    }

    //delete needed walls between two 2x2 blocks depending on dir vector(in mathematical meaning)
    for(int i = 0; i < (ordered_path.size()-1); ++i)
    {
        if(ordered_path[i+1].first == ordered_path[i].first)
        {
            //up to down
            if(ordered_path[i+1].second > ordered_path[i].second)
            {
                w_p_chk[3*(3*ordered_path[i+1].second-1)*maze_width + 3*ordered_path[i+1].first] = path;
                w_p_chk[3*(3*ordered_path[i+1].second-1)*maze_width + 3*ordered_path[i+1].first+1] = path;
            }
            //down to up
            else
            {
                w_p_chk[3*(3*ordered_path[i].second-1)*maze_width + 3*ordered_path[i].first] = path;
                w_p_chk[3*(3*ordered_path[i].second-1)*maze_width + 3*ordered_path[i].first+1] = path;
            }
        }

        else if(ordered_path[i+1].second == ordered_path[i].second)
        {
            //left to right
            if(ordered_path[i+1].first > ordered_path[i].first)
            {
                w_p_chk[3*(3*ordered_path[i+1].second)*maze_width + 3*ordered_path[i+1].first-1] = path;
                w_p_chk[3*(3*ordered_path[i+1].second+1)*maze_width + 3*ordered_path[i+1].first-1] = path;
            }
            //right to left
            else
            {
                w_p_chk[3*(3*ordered_path[i].second)*maze_width + 3*ordered_path[i].first-1] = path;
                w_p_chk[3*(3*ordered_path[i].second+1)*maze_width + 3*ordered_path[i].first-1] = path;
            }
        }
    }
}

//makes maze accessible to build new mazes
void Maze :: clean_maze()
{
    m_stack.clear();
    for (int i = 0; i < visit_chk.size(); ++i)
        visit_chk[i] = 0;
    for (int  i = 0 ; i < w_p_chk.size(); ++i)
        w_p_chk[i] = wall;
    ordered_path.clear();
    maze_path.clear();
    visitedCellN = 0;
    except_path = 0;
}

//cleans 2x2 blocks
void Maze :: cut_walls(int init_x, int init_y)
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            w_p_chk[3*(3*init_y+i)*maze_width + (3*init_x+j)] = path; //change all blocks (they were walls) into paths
}

//for other classes
const std :: vector<char>& Maze :: w_p_vector() const
{
    return w_p_chk;
}

//for other classes
const std :: vector<std :: pair<int, int>>& Maze :: path_vector() const
{
    return maze_path;
}


#endif // GENERATING_ALGORITHM_CPP
