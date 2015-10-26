/*-----------------------------------------------------------
File Name: BoggleSolver.cpp
Author: Michael Bhatt
Created: 4/7/2015

This file contains the implementation for a solver to the popular Boggle word game.
--------------------------------------------------------------*/


#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <math.h>
#include <algorithm>
#include <cctype>
#include <set>

using namespace std;

///<summary>
///A node within the Trie Data Structure
///</summary>
class Trie_node
{	
	public:
	string letter; //The letter which denotes this node
	bool isWord; //True if a word can be created using this node
	Trie_node *children[26]; //The children nodes of this node


	///<summary>
	///Returns true if this node has at least 1 child node
	///</summary>
	bool hasChildren()
	{
		for (int i = 0; i<26; i++){
			if (children[i] != NULL)
			{return true;}
		}
		return false;
	}
};


///<summary>
///Trie data structure used to hold a dictionary of words
///Useful for efficient word insertion and retrieval for large word sets
///<summary>
class Trie{

	private:
	Trie_node *rootNode;
	
	///<summary>
	///Get the alphabetical index for the input character
	///Range [a,z] => [0- 25]
	///26 Reserved for Null
	///</summary>
	int GetAlphabeticalIndex(char c)
	{
		if (c == '0')
			return 26;
		else
			return int (c) - int ('a');
	}
	
	///<summary>
	///Adds a new node to the Trie Structure
	///</summary>
	Trie_node *AddNode(Trie_node *parent, char c, bool isWord)
	{
		Trie_node *newNode = CreateNode(c, isWord);

		parent->children[GetAlphabeticalIndex(c)] = newNode;		
		return newNode;
	}

	///<summary>
	///Creates a new Trie node
	///</summary>
	/// <param name="c">The letter associated with the Trie Node</param>
	/// <param name="isWord">True if this node is the last node in a dictionary word</param>
	Trie_node* CreateNode(char c, bool isWord)
	{
		Trie_node *newNode = new Trie_node;
		newNode->letter = c;
		newNode->isWord = isWord;
		
		for (int i = 0; i<26; i++)
		{newNode->children[i] = NULL;}
		
		return newNode;
	}
	
	///<summary> Attempts to find the leaf node of a word in the Trie
	///The leaf node is the node corresponding to the last letter of the word
	///<param name = "word> A string </param name>
	///<returns> The leaf node (Null if the word is not in the Trie) </returns>
	Trie_node * GetLeafNode(string word)
	{
		Trie_node *currentNode = rootNode;
		Trie_node *childNode;
		for (int i =0; i<word.length(); i++)
		{
			char letter =  tolower(word[i]);
			childNode = currentNode->children[GetAlphabeticalIndex(letter)];
			if (childNode == NULL)
			{
				return NULL;
			}
			currentNode = childNode;
		}
		return currentNode;
	}

	public:
	//Initialize the Trie Data Structure by Initialing the RootNode
	Trie()
	{	rootNode = CreateNode('\0', false);
	}
	
	///<summary> Open the dictionary file and add all the words from the dictionary into the Trie </summary>
	///<param name "path" = > The Location of the dictionary </param name>
	bool LoadDictionary(string path)
	{

		ifstream dictionary(path);
		string word;
		
		if (dictionary.is_open())
		{
			while (getline(dictionary,word))
			{Addword(word);}

			dictionary.close();
			return 1;
		}
		else
		{return 0;}

	}

	///<summary> Adds a word to the trie </summary>
	///<param name = "word> A word to be added to the Trie </param name>
	void Addword(string word)
	{
		int lastLetter = word.length() -1; //The last letter in the word
		Trie_node *currentNode = rootNode;
		Trie_node *childNode;
		for (int i =0; i < word.length(); i++)
		{
			char letter = tolower(word[i]);
			childNode = currentNode->children[GetAlphabeticalIndex(letter)];
			if (childNode == NULL)
			{
				if (i == lastLetter )
				{currentNode = AddNode(currentNode,letter, true);}
				else
				{currentNode = AddNode(currentNode,letter, false);}
			}
			else
			{currentNode = childNode;}
		}
	}

	///<summary>Checks if the current word is a substring of a larger word 
	///Traverses the trie to the last node of the input word
	///If that node has children then this word is a substring
	///<param name = "word"> The word to be checked </param name>
	bool isSubstring(string word)
	{
		Trie_node *LeafNode = GetLeafNode(word);
		
		if (LeafNode == NULL)
		{return false;}
		else
		{return LeafNode->hasChildren();}

	}

	///<summary> Attempts to find the word in the Trie Structure </summary>
	///<param name = "word> The word to find </param name>
	///<returns> True if the word is in the Trie, False otherwise </returns>
	bool FindWord(string word)
	{
		Trie_node *LeafNode = GetLeafNode(word);
		if (LeafNode != NULL)
		{
			if (LeafNode->isWord)
			{return true;}
			else
			{return false;}
		}
		else
		{return false;}
	}
};

//A position on the Boggle Board (x,y)
//x indicates the row on the board
//y indicates the column on the board
class BoardPosition
{
	public:
	BoardPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x;
	int y;
};

class BoggleBoard
{
private:
	string **board;
	int x;
	int y;

public:
	///<summary> Returns the x dimension of the board </summary>
	int getX() 
	{return x;}
	
	///<summary> Returns the y dimension of the board </summary>
	int getY()
	{return y;}

	///<summary> Initializes a board of the input size </summary>
	BoggleBoard(int rows, int columns)
	{
		this->x = columns;
		this->y = rows;
		board = new string*[y];
		for(int i = 0; i < y; ++i) 
			{board[i] = new string[x];}
	}
	
	///<summary> Change the size of the Boggle Board </summary>
	///NOTE: If the dimensions are adjusted the board must be loaded again
	void AdjustDimensions(int rows, int columns)
	{
		this->x = columns;
		this->y = rows;
		//Clear the old board and create a new board
		delete board;
		board = new string*[y];
		for(int i = 0; i < y; ++i) 
			{board[i] = new string[x];}
	}

	///<summary> Loads a Boggle board from a text file </summary>
	///<param name "Board_path"> The path to the text file containing the board </param name>
	///<returns> True if the board was loaded successfully </returns>
	bool LoadBoard(string Board_path)
	{
		ifstream file2(Board_path);
		if (file2.is_open())
		{
			string line;
			
			for (int i = 0; i < y; i++)
			{
				getline(file2,line);
				std::remove_if (line.begin(), line.end(), isspace); //Strip Whitespace from each line
				for (int j =0; j< x; j++)
				{	
					if(isalpha(line[j])) //All characters on a Board must be alphabetical
						board[i][j] = line[j];					
					else
						return 0;
				}
			}
		}
		else //File did not open properly
		{return 0; }

		return 1;
	}
	///<summary> Returns the letter at the given location on the Boggle Board </summary>
	string GetLetter(BoardPosition location)
	{
		return board[location.x][location.y];
	}
	///<summary> Converts a vertex ID into a board position </summary>
	BoardPosition Vertex_To_BoardPosition(int vertex)
	{
		BoardPosition output(vertex/x , vertex % x);
		return output;
	}

};

///<summary> Creates an adjacency matrix to represent the Boggle Board
///The matrix is size (v^2) where v is the number of letters on the boggle board (vertices)
///If two vertices in the graph are adjacent they have an edgeweight of 1 
///</summary>
class AdjacencyMatrix
{
private:
	int **adjacencyMatrix;
	int vertices;

	///<summary> Checks if two locations on the boggle board are adjacent </summary>
	int CheckAdjacency(BoardPosition position1, BoardPosition position2)
	{
		if (abs(position1.x - position2.x) > 1 || abs(position1.y - position2.y) > 1)
			return 0;
		else
			return 1;
	}


public:
	///<returns> The vertices in the Adjacency Matrix </summary>
	int Get_Vertices()
	{return vertices;}

	///<summary> Uses an Adjacency Matrix to store the Boggle board as a Graph
	/// Adjacency Matrix is used in order to efficiently retrieve connected edges
	///</summary>
	///<param name = "vertices"> Each vertex in the graph represents a letter on the Boggle Board </param name>
	///<param name = "board"> The boggle board object </param name>
	AdjacencyMatrix(BoggleBoard board) //, BoggleBoard board)
	{	
		vertices = board.getX() * board.getY();
		adjacencyMatrix = new int*[vertices];
		for(int i = 0; i < vertices; ++i) 
		{adjacencyMatrix[i] = new int[vertices];}


		for (int i =0; i< vertices; i++)
		{for (int j =0; j<vertices; j++)
		{
			if (i == j)
			{
				adjacencyMatrix[i][j] = 0; //Location cannot be adjacent to itself
			}
			else
			{
				BoardPosition Location1 = board.Vertex_To_BoardPosition(i);
				BoardPosition Location2 = board.Vertex_To_BoardPosition(j);
				adjacencyMatrix[i][j] = CheckAdjacency(Location1, Location2);
			}
		}
		}
	}
	///<summary> Check if two vertices are are adjacent </summary>
	bool CheckAdjacent(int vert1, int vert2)
	{
		if (adjacencyMatrix[vert1][vert2] == 1)
			return 1;
		else
			return 0;
	}

};


///<summary> Finds all possible solutions on a Boggle Board </summary>
class BoggleSolver
{
private:
	set<string> SolutionWords;
	///<Summary> Finds all available words given the current word and the last letter used on the board</summary>
	///<param name = "board"> The Boggle Board </param name>
	///<param name = "currentWord"> The current string of letters on the boggle board</param name>
	///<param name = "lastVertex"> The vertex of the last letter used on the Board</param name>
	///<param name = "adjMat"> The adjaceny Matrix representing the Boggle Board</param name>
	///<param name = "dictionary"> A trie containing a dictionary of words </param name>
	void FindWords(BoggleBoard board, string currentWord, int lastVertex, AdjacencyMatrix adjMat, Trie dictionary)
{		
		int vertices = adjMat.Get_Vertices();

		//Iterate over each letter on the board
		for (int nextVertex =0; nextVertex< vertices; nextVertex++)
		{
			BoardPosition currentLocation = board.Vertex_To_BoardPosition(nextVertex);
			string letter = board.GetLetter(currentLocation);
			
			//If this letter is adjacent to the last letter used and it is not already in the word
			if (adjMat.CheckAdjacent(lastVertex,nextVertex) && currentWord.find(letter) == -1)
			{
				if(dictionary.FindWord(currentWord+letter))
				{SolutionWords.insert(currentWord+letter);}
				
				//If this word has children continue recursion
				if (dictionary.isSubstring(currentWord+letter))
				{FindWords(board, currentWord + letter, nextVertex, adjMat,dictionary);}
			}
		}
}

public:
	///<summary> Finds all the possible words that can be made on the input Boggle Board </summary>
	///<param name = "rows"> The rows on the Boggle Board </param name>
	///<param name = "columns"> The columns on the Boggle Board </param name>
	///<param name = "dictionaryPath"> The path for the Dictionary file</param name>
	///<param name = "rows"> The path for the Boggle Board file</param name>
	///<returns> A set containing all the words that can be created on the Boggle Board </returns>
	set<string> GetSolutions(int rows, int columns, string dictionaryPath, string boardPath)
	{
		//Build the Dictionary of words from the User Specified path
		Trie dictionary;
		if (!dictionary.LoadDictionary(dictionaryPath))
		{
			cout << "Invalid dictionary" << endl;
			return SolutionWords;
		}
		//Load the Boggle Board from the User Specified path
		BoggleBoard board(rows,columns);
		if (!board.LoadBoard(boardPath))
		{
			cout << "Invalid board" << endl;
			return SolutionWords;
		}
	
		//Store the Boggle Board as a graph
		//Each letter represents a vertex
		//Vertices that are adjacent create an edge
		AdjacencyMatrix adjMat(board);

		//Iterate over each letter on the board
		for (int currentVertex =0; currentVertex< adjMat.Get_Vertices(); currentVertex ++)
		{
			BoardPosition currentLocation = board.Vertex_To_BoardPosition(currentVertex);
			string currentWord = board.GetLetter(currentLocation);
		
			if(dictionary.FindWord(currentWord)) //Check for 1 letter words
			{SolutionWords.insert(currentWord);}
		
			FindWords(board,currentWord,currentVertex, adjMat,dictionary);
		}

		return SolutionWords;
	}

};
int main()
{
	BoggleSolver mysolver;
	int rows, columns;
	string dictionaryPath, boardPath;

	//Prompt the user for Board size, Dictionary File and Board file
	cout << "Enter the Dimensions for the Boggle Board \n" << endl;
	cout << "How many rows on the Boggle Board?" << endl;
	cin >> rows;
	
	cout << "How many columns on the Boggle Board?" << endl;
	cin >> columns;
	
	cout << "Enter the path for the Dictionary file you wish to use." << endl;
	cin >> dictionaryPath;
	
	cout << "Enter the path for the Boggle Board you wish to solve" << endl;
	cin >> boardPath;

	clock_t timer;
	timer = clock();
	set <string> words = mysolver.GetSolutions(rows,columns,dictionaryPath,boardPath);
	
	std::cout << "Solution took " << (std::clock() - timer) / (double)(CLOCKS_PER_SEC / 1000) << " ms to execute" << std::endl;
	system("pause");
}