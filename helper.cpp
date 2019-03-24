#include "helper.hpp"
#include "prettyprint.hpp"

#include <fstream>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>


// Templated to print an element on a single line
template <typename T> void print(const T d) {
    std::cout << d;
}

void print_grid(cgrid_t &grid) {
    for (crow_t *row : grid) {
        std::for_each(row->begin(), row->end(), &print<rowElem_t>);
        std::cout << std::endl;
    }
}

void print_grid(ngrid_t &grid) {
    for (std::vector<int> *row : grid) {
        for (int elem : *row) {
            std::cout << std::setw(3) << std::setfill(' ')
                      << elem << " ";
        }
        std::cout << std::endl;
    }
}

/*
 * */
cgrid_t file_to_grid(std::string &filepath) {
    std::ifstream file;
    file.open(filepath);

    std::string line;
    cgrid_t result;
    size_t lastSize = 0;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            assert(not((lastSize != 0) and (lastSize != line.size())) && "Each row of the input array should of the same size.");
            lastSize = line.size();

            crow_t *newRow = new crow_t(line.begin(), line.end());
            result.push_back(newRow);
        }
    } else {
        std::cout << "ERROR: " << "Unable to open the specified file" << std::endl;
    }
    return result;
}

ngrid_t number_nodes(cgrid_t &grid) {
    ngrid_t numbered_grid(grid.size());
    for (auto &row : numbered_grid) {
        row = new std::vector<int>(grid[0]->size(), -1);
    }

    /* Number the nodes */
    int iter = 0;
    for (size_t r = 0; r < grid.size(); r++) {
        for (size_t c = 0; c < grid[r]->size(); c++) {
            if (grid[r]->operator[]((unsigned long)c) == '.') {
                numbered_grid[r]->operator[]((unsigned long)c) = iter++;
            }
        }
    }

    return numbered_grid;
}

dict grid_to_graph(ngrid_t &grid) {
    dict result;
    unsigned long height = grid.size();
    unsigned long width = grid[0]->size();

    for (size_t r = 0; r < height; r++) {
        for (size_t c = 0; c < width; c++) {
            int elem = grid[r]->operator[](c);

            if (elem != -1) { /* A node */
                /* Add the southern neighbour */
                if (r != height-1) {
                    auto south = grid[r+1]->operator[](c );
                    if (south != -1) result[elem].push_back(south);
                }

                /* Add the northern neighbour */
                if (r != 0) {
                    auto north = grid[r-1]->operator[](c );
                    if (north != -1) result[elem].push_back(north);
                }

                /* Add the eastern neighbour */
                if (c != width-1) {
                    auto east = grid[r]->operator[](c + 1);
                    if (east != -1)
                        result[elem].push_back(east);
                }

                /* Add the western neighbour */
                if (c != 0) {
                    auto west = grid[r]->operator[](c - 1);
                    if (west != -1)
                        result[elem].push_back(west);
                }
            }
        }
    }

    return result;
}

void write_sgrid(sgrid_t grid, std::string outputFilePath) {
    std::ofstream outputFile;
    outputFile.open(outputFilePath);
    for (srow_t *row : grid) {
        for (size_t i = 0; i < row->size(); i++) {
            outputFile << row->operator[](i);
            if (i != row->size()-1)
                outputFile << ",";
            else
                outputFile << std::endl;
        }
    }
}

