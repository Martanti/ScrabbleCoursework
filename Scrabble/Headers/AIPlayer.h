#pragma once

#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <math.h>

#include "Word.h"
#include "QuickSort.h"
#include "Player.h"

using std::map;
using std::vector;
using std::string;
using std::tuple;
using std::unique_ptr;

///<summary>
	///<para>
	///Logic for AI player
	///</para>
	///</summary>
class CAIPlayer : public CPlayer
{
public:
	CAIPlayer();
	~CAIPlayer();
	
	///<summary>
	///<para>
	///Filters and saves all of the legal words that the AI will use
	///</para>
	///</summary>
	void InitAllowedWords(vector<tuple<string, int>> *vecLegalWords);
	
	///<summary>
	///<para>
	///Get All of the available words that AI can create out of the given letter pool
	///</para>
	///</summary>
	vector<string> GetAvailableWords(map<char, int> availableCharacters);

private:

	///<summary>
	///<para>
	///Stored legal words that can be used by the AI
	///</para>
	///</summary>
	map<string, vector<string>> m_mUsableWords;

	///<summary>
	///<para>
	///Takes a string and sorts its characters alphabetically
	///</para>
	///</summary>
	string SortWord(string strSorted);

	///<summary>
	///<para>
	///Retrieves all of possible 3 letter combinations
	///</para>
	///</summary>
	unique_ptr<vector<string>>  GetThreeCombinations(string strAllowedLetters);

	///<summary>
	///<para>
	///Retrieves all of possible 4 letter combinations
	///</para>
	///</summary>
	unique_ptr<vector<string>> GetFourCombinations(string strAllowedLetters);

	///<summary>
	///<para>
	///Retrieves all of possible 7 letter combinations
	///</para>
	///</summary>
	unique_ptr<vector<string>> GetSevenCombinations(string strAllowedLetters);

	///<summary>
	///<para>
	///Converts letter pool to a one string of available letters
	///</para>
	///</summary>
	string DictionaryToString(map<char, int> mLetters);

	///<summary>
	///<para>
	///Checks all the given words and makes multiple versions of those that have blanks
	///</para>
	///</summary>
	unique_ptr<vector<string>> AddForBlanks(unique_ptr<vector<string>> pvecAllTheWords);

	///<summary>
	///<para>
	///Deletes all the words with blanks from a refered list
	///</para>
	///</summary>
	void DeleteBlanks(vector<string> &vecAllTheWords);

};

