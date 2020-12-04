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
To compile the program:</br>
>    
     make map1

To run the program:</br>
> 
     ./map1 <csv_filename> <output_filename> < <keyfile_name> 

     <csv_filename> arg     - Dataset file
     <output_filename> arg  - Output file to record search results
     <keyfile_name> arg     - File of keys to be searched (contains one 
                              coordinates key separated by <space>
                              per line. Eg. x.xxx y.yyy) 
>
> ## <a name="map2"></a>Map2.c
To compile the program:</br>
>    
     make map2

To run the program:</br>
> 
     ./map2 <csv_filename> <output_filename> < <keyfile_name> 

     <csv_filename> arg     - Dataset file
     <output_filename> arg  - Output file to record search results
     <keyfile_name> arg     - File of keys to be searched (contains one 
                              coordinates-radius key separated by <space> 
                              per line. Example key x.xxx y.yyy r.rrr) 
>
# <a name="experimentation"></a>Experimentation
Refer to the [experimentation report](https://github.com/olivertan1999/Information-Retrieval-Using-KD-Tree/blob/main/Experimentation%20Report.pdf) to understand further the performance of algorithms used in this program. 
