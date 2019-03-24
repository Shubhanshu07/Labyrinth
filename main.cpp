#include "main.hpp"
#include "helper.hpp"
#include "prettyprint.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

/* Appends the second data list to the first one */
void append(std::vector<dataList> &a, std::vector<dataList> &b) {
    a.reserve(a.size() + b.size());
    a.insert(a.begin()+a.size(), b.begin(), b.end());
}

/* Depth first search */
std::vector<dataList> DFS(dict *G, data v, dataList seen, dataList *path=nullptr) {
    if (path->empty())
        path->push_back((data)v);

    /* Color the current node */
    seen.push_back((data)v);

    /* List of all the paths found */
    std::vector<dataList> paths;

    /* Iterate over all the neighbouring nodes of the node v */
    for (data t : G->operator[](v)) {
        /* If this node was never seen before add it to the path else move on to the next node */
        if (std::find(seen.begin(), seen.end(), t) == seen.end()) {
            dataList t_path(*path);     /* Create a new temporary path using the existing path */
            t_path.push_back(t);        /* Add the current element to the temporary path */

            paths.push_back(t_path);    /* Add temporary path to list of paths */

            /* Execute DFS on the node t */
            std::vector<dataList> result = DFS(G, t, seen, &t_path);

            /* Add the results obtained from DFS to all the paths found */
            append(paths, result);
        }
    }

    return paths;
}

sgrid_t generate_path(std::string inputFilePath) {
    dict G;
    std::string fpath(inputFilePath);

    auto grid = file_to_grid(fpath); /* Get the grid from the input file */
    auto ngrid = number_nodes(grid); /* Number the nodes in the input grid */

    G = grid_to_graph(ngrid);        /* Get the graph representation of the nodes in the grid*/

    std::vector<dataList> all_paths;
    for (std::pair<data, dataList> elem : G) { /* Iterate over all the elements */
        dataList seen;
        dataList *path = new dataList();

        for (dataList path : DFS(&G, elem.first, seen, path)) {
            all_paths.push_back(path);
        }

        delete path;
    }

    /* Calculate size (length) of each path (std::vector) in the vector all_paths*/
    std::vector<size_t> sizes(all_paths.size());
    std::transform(all_paths.begin(), all_paths.end(), sizes.begin(),
                   mem_fun_ref(&std::vector<data>::size));

    /* Find the path with maximum length */
    size_t max_len = *std::max_element(sizes.begin(), sizes.end());

    /* Find the first path that is the longest */
    dataList longestPath;
    for (dataList elem : all_paths) {
        if (elem.size() == max_len) {
            std::cout << "Longest Path: " << elem << std::endl;
            longestPath = elem;
            break;
        }
    }

    /*** Re-number the grid ***/
    /* Create a mapping from new scheme to the previous one */
    std::map<data, data> gridNumberMap;
    unsigned long iterNum = 0;
    for (data pos : longestPath) {
        gridNumberMap[pos] = iterNum++;
    }

    sgrid_t resultantGrid(ngrid.size());
    /* Find all the nodes in the grid and use mapping to re-number them */
    for (size_t r = 0; r < ngrid.size(); r++) {
        resultantGrid[r] = new srow_t(ngrid[r]->size());
        for (size_t c  = 0; c < ngrid[r]->size(); c++) {
            /* Reconstruct the original grid */
            int elem = ngrid[r]->operator[](c);
            auto isPresent = gridNumberMap.find(elem);
            if (elem == -1) {
                resultantGrid[r]->operator[](c) = "#";
            } else if ( isPresent == gridNumberMap.end()) {
                resultantGrid[r]->operator[](c) = ".";
            } else {
                resultantGrid[r]->operator[](c) = std::to_string(gridNumberMap.find(elem)->second);
            }
        }
    }

    std::cout << "Max path length: " << max_len << std::endl;

    std::cout << "Output grid:" << std::endl;
    for (srow_t *row : resultantGrid) {
        std::cout << *row << std::endl;
    }

    /*** Delete all dynamically allocated elements ***/
    for (std::vector<int> *row : ngrid) {
        delete row;
    }

    for (crow_t *row : grid) {
        delete row;
    }

    return resultantGrid;
}

int main(int argc, char *argv[]) {
    std::string inputFilePath, outputFilePath;
    bool inputPathFound = false, outputPathFound = false;

    /* Parse the arguments supplied to the program */
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]).compare("--help") == 0 or std::string(argv[i]).compare("-h") == 0) {
            std::cout << "Help: " << std::endl
                      << "-i <filepath> : Input file path" << std::endl
                      << "-o <filepath> : Output file path" << std::endl
                      << "--help or -h  : Displays this message" << std::endl;
            return 0;
        } else if (std::string(argv[i]).compare("-i") == 0) {
            assert(i+1 < argc && "Please supply an input file name with argument '-i'");
            inputFilePath = std::string(argv[i+1]);
            inputPathFound = true;
        } else if (std::string(argv[i]).compare("-o") == 0) {
            assert(i+1 < argc && "Please supply an output file name with argument '-o'");
            outputFilePath = std::string(argv[i+1]);
            outputPathFound = true;
        }
    }

    /* Check if all the parameters are correctly supplied */
    bool error = false;
    if (!inputPathFound) {
        std::cout << "ERROR: Input path not specified" << std::endl;
        error = true;
    }
    if (!outputPathFound) {
        std::cout << "ERROR: Output path not specified" << std::endl;
        error = true;
    }

    if (error) {
        std::cout << "Exiting..." << std::endl;
        return 1;
    } else {
        /* Process the input from the input file and write it to the output file specified */
        auto resultantGrid = generate_path(inputFilePath);
        write_sgrid(resultantGrid, outputFilePath);

        /* Delete dynamically allocated objects */
        for (srow_t *row : resultantGrid) {
            delete row;
        }
    }
    return 0;
}

