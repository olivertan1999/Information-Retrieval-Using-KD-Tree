# Implementing Information Retrieval And Map Functions Using KD Tree

# Contents
* [Introduction](#introduction)
* [Instruction](#instruction)
  * [map1](#map1)
  * [map2](#map2)
* [Experimentation](#experimentation)

# <a name="introduction"></a>Introduction
The aim of this project is to create a K-D tree to support interactive map functionality for the City of Melbourne Census of Land Use and Employment (CLUE) dataset. The program map1.c accepts locations queries from stdin and search the tree for the location nearest to the query point. If there are multiple businesses at this location, all of them must be included in the output. **If there is no record found in the query point, the program will output the nearest record from that query point.** The record(s) of the business(s) found will be printed to the output file specified by the user.</br></br>
The program map2.c accepts x, y, radius queries from stdin and search the tree for all locations within the requested radius of the (x, y) point. The records found will be printed to the output file. When there are multiple businesses at the same location, all of these records will be included in the output. **If no business establishments are located with the query radius, the program will output the word NOTFOUND.**

# <a name="instruction"></a>Instruction
> ## <a name="map1"></a>Map1.c
>

# <a name="experimentation"></a>Experimentation8
