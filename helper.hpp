#ifndef LABYRINTH_HELPER_HPP
#define LABYRINTH_HELPER_HPP

#include "global.hpp"

/*** Types for representing the input grid, uses char for an element ***/
typedef char rowElem_t;
typedef std::vector<rowElem_t> crow_t;
typedef std::vector<crow_t*> cgrid_t;

/*** Types for representing the output grid, uses std::string for an element ***/
typedef std::vector<std::string> srow_t;
typedef std::vector<srow_t*> sgrid_t;

/*** Type for representing grid where nodes are non negative integers while wall is represented by -1 ***/
typedef std::vector<std::vector<int>*> ngrid_t;

/* Returns a cgrid_t object representation of the grid in text file supplied */
cgrid_t file_to_grid(std::string &filepath);

/* Numbers all the nodes in the grid starting from 0*/
ngrid_t number_nodes(cgrid_t &grid);

/* Returns an adjacency matrix like representation of the graph created by connecting N4 neighbouring nodes */
dict grid_to_graph(ngrid_t &grid);

/* Takes a sgrid_t object and writes the grid to the file of the filepath supplied */
void write_sgrid(sgrid_t grid, std::string outputFilePath);

#endif //LABYRINTH_HELPER_HPP
