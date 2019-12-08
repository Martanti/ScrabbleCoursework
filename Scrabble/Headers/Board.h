#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <map>

#include "Word.h"

using std::string;
using std::tuple;
using std::vector;
using std::map;


///<summary>
	///<para>
	///Class for handling interactions with board and multipliers
	///</para>
	///</summary>
class CBoard
{
public:

	///<summary>
	///<para>
	///Size of the playing board
	///</para>
	///</summary>
	static const int m_kiBoardSize = 225;
	//15x15 board

	CBoard();
	~CBoard();

	///<summary>
	///<para>
	///Get a pointer to a copy of board array
	///</para>
	///</summary>
	char* GetBoard();

	///<summary>
	///<para>
	///Get a pointer to a copy of multipliers array
	///</para>
	///</summary>
	char* GetMultipliers();

	///<summary>
	///<para>
	///Check if word can be placed. Directions: 'D' for from up to down and 'L' for from right to left. Positions start at 0.
	///<para/>
	///Use before PlaceWord in order to avoid problems
	///</para>
	///</summary>
	bool CanWordBePlaced(CWord wrdPlaced);

	///<summary>
	///<para>
	///Get a list of words that are being intersected
	///</para>
	///</summary>
	vector<CWord> GetIntersectedWords(CWord wrdPlaced);

	///<summary>
	///<para>
	///Get all the words that are created from placing this word
	///<para/>
	///Use before PlaceWord in order to avoid problems
	///</para>
	///</summary>
	vector<CWord> GetAllTheAdditionalWords(CWord wrdPlaced);

	///<summary>
	///<para>
	///Get a dictionary of needed letters for the word to be placed
	///</para>
	///</summary>
	map<char, int> GetNeededCharaters(CWord wrdPlaced);

	///<summary>
	///<para>
	///Place a word in a board. Directions: 'D' for from up to down and 'L' for from right to left. Vector of any multipliers if there are any. Positions start at 0.
	///</para>
	///</summary>
	void PlaceWord(CWord wrdPlaced);

	///<summary>
	///<para>
	///Returns list of tuples containing index of the letter, leter for multiplication type ('w' for word and 'l' for letter) and number of how big multiplication is respectively
	///</para>
	///</summary>
	vector<tuple<int, char, int>> GetCrossedMultipliers(CWord wrdPlaced);

	///<summary>
	///<para>
	///Register a word to the list of placed words
	///</para>
	///</summary>
	void RegisterPlacedWord(CWord wrdPlaced);

	///<summary>
	///<para>
	///Check if there already has been word like this. Allows to avoid people typing the same word over alredy existing one
	///</para>
	///</summary>
	bool HasSameWordAlredyBeenPlaced(CWord wrdPlaced);

	///<summary>
	///<para>
	///Reacreate a word from direction and coordinates of one of the letters of the word
	///<para/>
	///Automatically gets the letter from the coordinates
	///</para>
	///</summary>
	CWord RecreateAWord(int iXPivot, int iYPivot, Directions checkedDirection);

	///<summary>
	///<para>
	///Reacreate a word from direction and coordinates of one of the letters of the word
	///<para/>
	///Uses given letter as a pivot
	///</para>
	///</summary>
	CWord RecreateAWord(char cPivotLetter, int iXPivot, int iYPivot, Directions checkedDirection);

	///<summary>
	///<para>
	///Get a copy of list of placed words
	///</para>
	///</summary>
	vector<CWord> GetPlacedWords();

private:

	char *m_pcBoardProxy = new char[m_kiBoardSize];
	char *m_pcMultipliersProxy = new char[m_kiBoardSize];

	///<summary>
	///<para>
	///List of words that alredy have been placed
	///</para>
	///</summary>
	vector<CWord> m_vecPlacedWords;

	///<summary>
	///<para>
	///Board for having the words in
	///</para>
	///</summary>
	char m_rgcBoard[m_kiBoardSize] = {};

	///<summary>
	///<para>
	///Keeping a original state of board with all of the multipliers. 
	///<para/> '+' - center star/double word
	///<para/> '@' - double letter 
	///<para/> '#' - triplle letter 
	///<para/> '%' - double word 
	///<para/> '&amp;' - triple word
	///</para>
	///</summary>
	char m_rgcMultipliers[m_kiBoardSize] = {
		'&', ' ', ' ', '@', ' ', ' ', ' ', '&',' ', ' ', ' ', '@', ' ', ' ', '&',
		' ', '%', ' ', ' ', ' ', '#', ' ', ' ',' ', '#', ' ', ' ', ' ', '%', ' ',
		' ', ' ', '%', ' ', ' ', ' ', '@', ' ','@', ' ', ' ', ' ', '%', ' ', ' ',
		'@', ' ', ' ', '%', ' ', ' ', ' ', '@',' ', ' ', ' ', '%', ' ', ' ', '@',
		' ', ' ', ' ', ' ', '%', ' ', ' ', ' ',' ', ' ', '%', ' ', ' ', ' ', ' ',
		' ', '#', ' ', ' ', ' ', '#', ' ', ' ',' ', '#', ' ', ' ', ' ', '#', ' ',
		' ', ' ', '@', ' ', ' ', ' ', '@', ' ','@', ' ', ' ', ' ', '@', ' ', ' ',
		'&', ' ', ' ', '@', ' ', ' ', ' ', '+',' ', ' ', ' ', '@', ' ', ' ', '&',
		' ', ' ', '@', ' ', ' ', ' ', '@', ' ','@', ' ', ' ', ' ', '@', ' ', ' ',
		' ', '#', ' ', ' ', ' ', '#', ' ', ' ',' ', '#', ' ', ' ', ' ', '#', ' ',
		' ', ' ', ' ', ' ', '%', ' ', ' ', ' ',' ', ' ', '%', ' ', ' ', ' ', ' ',
		'@', ' ', ' ', '%', ' ', ' ', ' ', '@',' ', ' ', ' ', '%', ' ', ' ', '@',
		' ', ' ', '%', ' ', ' ', ' ', '@', ' ','@', ' ', ' ', ' ', '%', ' ', ' ',
		' ', '%', ' ', ' ', ' ', '#', ' ', ' ',' ', '#', ' ', ' ', ' ', '%', ' ',
		'&', ' ', ' ', '@', ' ', ' ', ' ', '&',' ', ' ', ' ', '@', ' ', ' ', '&',
	};


	///<summary>
	///<para>
	///Convert multiplier symbol from multiplier symbol into a tuple of word/letter character and multiplier
	///</para>
	///</summary>
	tuple<char, int> GetMultiplier(char cChecked);

	///<summary>
	///<para>
	///Get a part of word before the pivot point
	///</para>
	///</summary>
	string GetWordPartBeforePivot(int iDirectionIncrement, int iPivotPositionInArray, int iFurtherestToCheck);

	///<summary>
	///<para>
	///Get a part of word after the pivot point. Returns the word part and the last index.
	///</para>
	///</summary>
	tuple<string, int> GetWordPartAfterPivot(int iDirectionIncrement, int iPivotPositionInArray, int iFurtherestToCheck);
};

