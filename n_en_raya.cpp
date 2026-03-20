#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <deque>
#include <set>
#include <limits>
#include <queue>

// 21 x 21

#define INIT_POS {0, 0}

using coord = std::pair<int, int>;
using coord_data = std::pair<coord, double>;

/*
class node
{
public:
    coord position;
    std::vector<node*> neighbors;


    node(const coord& in_coord) :
        position(in_coord), neighbors() {}

    bool operator < (const node& other)
    {
        if (position.first != other.position.first)
            return position.first < other.position.first;
        
        return position.second < other.position.second;
    }

    void add(node* in_node)
    {
        neighbors.push_back(in_node);
    }
};
*/

double distance(const coord& a, const coord& b)
{
    double first = a.first - b.first;
    double second = a.second - b.second;

    return std::sqrt((first * first) + (second * second));
}


class graph_draw
{
public:
    coord STARTING_POS, ENDING_POS;

    graph_draw(const size_t& in_COLS, const size_t& in_ROWS, const size_t& in_step) :
        COLS(in_COLS), ROWS(in_ROWS), step(in_step), STARTING_POS(INIT_POS), ENDING_POS(INIT_POS)
    {
        // Creating nodes
        for (auto j = 0; j <= in_ROWS; j++)
            for (auto i = 0; i <= in_COLS; i++)
                graph[{i * step, j * step}] = std::set<coord>();


        for (auto j = 0; j < in_ROWS; j++)
            for (auto i = 0; i < in_COLS; i++)
                graph[{(step/2) + (i * step), (step / 2) + (j * step)}] = std::set<coord>();

        // Creating edges
        for (auto j = 0; j <= in_ROWS; j++)
            for (auto i = 0; i <= in_COLS; i++)
            {
                coord current = { i * step, j * step };

                coord right = {current.first + step, current.second};
                coord down = { current.first, current.second + step};

                if (is_valid(right))
                {
                    graph[current].insert(right);
                    graph[right].insert(current);
                }

                if (is_valid(down))
                {
                    graph[current].insert(down);
                    graph[down].insert(current);
                }
            }

        for (auto j = 0; j < in_ROWS; j++)
            for (auto i = 0; i < in_COLS; i++)
            {
                size_t half_s = step / 2;

                coord current = { half_s + (i * step), half_s + (j * step) };
                coord u_r = { current.first + half_s, current.second - half_s };
                coord u_l = { current.first - half_s, current.second - half_s };
                coord b_r = { current.first + half_s, current.second + half_s};
                coord b_l = { current.first - half_s, current.second + half_s};

                if (is_valid(u_r))
                {
                    graph[current].insert(u_r);
                    graph[u_r].insert(current);
                }

                if (is_valid(u_l))
                {
                    graph[current].insert(u_l);
                    graph[u_l].insert(current);
                }

                if (is_valid(b_r))
                {
                    graph[current].insert(b_r);
                    graph[b_r].insert(current);
                }

                if (is_valid(b_l))
                {
                    graph[current].insert(b_l);
                    graph[b_l].insert(current);
                }
            }
    }

    void print()
    {
        for (auto& c : graph)
        {
            std::cout << c.first.first << " - " << c.first.second << " :";

            for (auto& n : c.second)
                std::cout << "[ " << n.first << ", " << n.second << " ] ";
            std::cout << "\n";
        }
    }

    void set_POS(const coord& in_STARTING, const coord& in_ENDING)
    {
        STARTING_POS = in_STARTING;
        ENDING_POS = in_ENDING;
    }

    void erase_node(const coord& to_erase)
    {
        if (is_valid(to_erase))
        {
            for (auto &key : graph[to_erase])
                graph[key].erase(to_erase);

            graph.erase(to_erase);
        }
    }

    void dfs(std::set<coord>& visited, std::vector<coord>& path)
    {
        bool found = false;
        std::deque<coord> to_explore;

        //       Coord, parent
        std::map<coord, coord> parents;

        to_explore.push_back(STARTING_POS);
        visited.insert(STARTING_POS);

        while (!to_explore.empty())
        {
            auto current = to_explore.front();
            to_explore.pop_front();


            if (current == ENDING_POS)
            {
                found = true;
                break;
            }

            for (auto& neighbor : graph[current])
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    parents[neighbor] = current;
                    to_explore.push_front(neighbor);
                }
            }
        }

        // Save path
        if (found)
        {
            auto backwards = ENDING_POS;
            while (backwards != STARTING_POS)
            {
                path.push_back(backwards);
                backwards = parents[backwards];
            }
        }
    }

    void bfs(std::set<coord>& visited, std::vector<coord>& path)
    {
        bool found = false;
        std::deque<coord> to_explore;

        //       Coord, parent
        std::map<coord, coord> parents;

        to_explore.push_back(STARTING_POS);
        visited.insert(STARTING_POS);


        while (!to_explore.empty())
        {
            auto current = to_explore.front();
            to_explore.pop_front();


            if (current == ENDING_POS)
            {
                found = true;
                break;
            }

            for (auto& neighbor : graph[current])
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    parents[neighbor] = current;
                    to_explore.push_back(neighbor);
                }
            }
        }

        // Save path
        if (found)
        {
            auto backwards = ENDING_POS;
            while (backwards != STARTING_POS)
            {
                path.push_back(backwards);
                backwards = parents[backwards];
            }
        }
    }

    void hill_climbing(std::set<coord>& visited, std::vector<coord>& path)
    {
        bool found = false;

        std::priority_queue<hill, std::vector<hill>, hill> to_explore;

        //       Coord, parent
        std::map<coord, coord> parents;

        to_explore.push(hill(STARTING_POS, distance(STARTING_POS, ENDING_POS)));
        visited.insert(STARTING_POS);

        while (!to_explore.empty())
        {
            auto current_dt = to_explore.top();
            auto current = current_dt.pos;
            to_explore.pop();


            if (current == ENDING_POS)
            {
                found = true;
                break;
            }

            for (auto& neighbor : graph[current])
            {
                if (visited.find(neighbor) == visited.end()) // New neighbor
                {
                    parents[neighbor] = current;
                    visited.insert(neighbor);
                    double new_distance = distance(neighbor, ENDING_POS);

                    to_explore.push(hill(neighbor, new_distance));
                }
            }
        }

        // Save path
        if (found)
        {
            auto backwards = ENDING_POS;
            while (backwards != STARTING_POS)
            {
                path.push_back(backwards);
                backwards = parents[backwards];
            }
        }
    }

    void a_star(std::set<coord>& visited, std::vector<coord>& path)
    {
        bool found = false;

        std::priority_queue<a_st, std::vector<a_st>, a_st> to_explore;

        //       Coord, parent
        std::map<coord, coord> parents;

        to_explore.push(a_st(STARTING_POS, 0.0, distance(STARTING_POS, ENDING_POS)));
        visited.insert(STARTING_POS);

        while (!to_explore.empty())
        {
            auto current_dt = to_explore.top();
            auto current = current_dt.pos;
            to_explore.pop();

            if (current == ENDING_POS)
            {
                found = true;
                break;
            }

            for (auto& neighbor : graph[current])
            {
                if (visited.find(neighbor) == visited.end()) // New neighbor
                {
                    parents[neighbor] = current;
                    visited.insert(neighbor);

                    double new_g = current_dt.g + distance(current, neighbor);
                    double new_h = distance(neighbor, ENDING_POS);

                    to_explore.push(a_st(neighbor, new_g, new_h));
                }
            }
        }

        // Save path
        if (found)
        {
            auto backwards = ENDING_POS;
            while (backwards != STARTING_POS)
            {
                path.push_back(backwards);
                backwards = parents[backwards];
            }
        }
    }

private:

    class hill
    {
    public:
        coord pos;
        double dist;

        hill(const coord& in_pos, const double& in_dist) :
            pos(in_pos), dist(in_dist)
        {}

        bool operator () (const hill& a, const hill& b)
        {
            return a.dist > b.dist;
        }
    };

    class a_st
    {
    public:
        coord pos;
        double h, g;

        a_st(const coord& in_pos, const double& in_g, const double& in_h) :
            pos(in_pos), g(in_g), h(in_h)
        { }

        bool operator () (const a_st& a, const a_st& b)
        {
            return (a.h + a.g) > (b.h + b.g);
        }
    };

    std::map<coord, std::set<coord>> graph;
    size_t COLS, ROWS, step;

    bool is_valid(const coord& in_cord)
    {
        if (in_cord.first < 0 || in_cord.first > COLS * step)
            return false;
        else if (in_cord.second < 0 || in_cord.second > ROWS * step)
            return false;

        return true;
    }
};


int main()
{
    graph_draw graphin(2, 2, 10);
    graphin.print();
}