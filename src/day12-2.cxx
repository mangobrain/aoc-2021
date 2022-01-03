#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>

int main(int argc, char const * const argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream fstr;
    fstr.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    fstr.open(argv[1]);

    struct Node {
        std::list<std::size_t> children;
        bool big;
        int visited;
        Node(bool b) : big(b), visited(0) {}
    };
    std::vector<Node> graph;
    std::map<std::string, std::size_t> index;
    std::size_t start = 0, end = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        // Split path into start & end nodes
        std::string a, b;
        std::size_t i = line.find('-');
        if (i == std::string::npos)
        {
            std::cerr << "Invalid input\n";
            return 1;
        }
        a = line.substr(0, i);
        b = line.substr(i + 1);

        // Find or create the nodes themselves
        auto ai = index.find(a);
        if (ai == index.end())
        {
            ai = index.emplace(a, graph.size()).first;
            graph.emplace_back(a[0] <= 'Z');
        }
        auto bi = index.find(b);
        if (bi == index.end())
        {
            bi = index.emplace(b, graph.size()).first;
            graph.emplace_back(b[0] <= 'Z');
        }

        // Keep track of start & end nodes
        if (a == "start")
            start = ai->second;
        else if (b == "start")
            start = bi->second;
        if (a == "end")
            end = ai->second;
        else if (b == "end")
            end = bi->second;

        // Add node B to node A's children, and vice versa.
        // The start can only ever be visited once on a path, it can never be
        // the small cave which is visited twice. To enforce this, only ever
        // add paths *away from* the start.
        if (b != "start")
            graph[ai->second].children.emplace_back(bi->second);
        if (a != "start")
            graph[bi->second].children.emplace_back(ai->second);
    }

    // Depth-first search of all paths from start to end.
    std::uint64_t paths = 0;
    bool doubled_up = false;
    struct State {
        bool backtrack;
        std::size_t index;
        State(bool b, std::size_t i) : backtrack(b), index(i) {}
    };
    std::stack<State> state;
    state.emplace(false, start);
    while (!state.empty())
    {
        auto s = state.top();
        state.pop();

        auto & node = graph[s.index];
        if (!s.backtrack)
        {
            // Increment the number of times we've visited this node. Big
            // nodes can be visited any number of times (never increment).
            // Small nodes can be visited
            // - twice, if we have not yet visited any small node twice
            // - otherwise, only once
            if (!node.big)
            {
                if (++node.visited == 2)
                    doubled_up = true;
            }
            // Not yet been here. Visit all unvisited children.
            for (auto c : node.children)
            {
                // We reached the end!
                if (c == end)
                {
                    ++paths;
                    continue;
                }
                if (graph[c].visited < (doubled_up ? 1 : 2))
                {
                    // For each child, insert a backtrack instruction, then
                    // the node itself. Stacks are LIFO, so we will hit these
                    // in reverse order: specifically, we will visit the node,
                    // visit (and backtrack back to) all of its children, then
                    // backtrack back from this node.
                    state.emplace(true, c);
                    state.emplace(false, c);
                }
            }
        } else {
            // Already been here - we must be backtracking.
            // When backtracking through the small cave that's been visited
            // twice, reset the flag that allows us to visit small caves twice
            // along this path.
            if ((!node.big) && (--node.visited == 1))
                doubled_up = false;
        }
    }

    std::cout << paths << '\n';
    return 0;
}
