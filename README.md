# BoggleSolver
A solver for the popular Boggle word game.

In this project I developed a function that takes as input a Boggle Board and a dictionary of suitable words and returns all words 
that can be created on the Boggle Board. The dictionary and Board files should be stored as text files. 

The program uses a Trie data structure to store the dictionary which allows for quick word Insertion and Search. 
The Boggle Board is stored as a graph with the vertices being each letter on the board. Each vertex has an edge with each of it's 
adjacent vertices. The graph for the Boggle Board is stored in an Adjacency matrix.
