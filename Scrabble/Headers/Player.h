#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

///<summary>
	///<para>
	///Class for handling player of scrabble game
	///</para>
	///</summary>
class CPlayer
{
public:

	///<summary>
	///<para>
	///Checks
	///</para>
	///</summary>
	bool HasLettersForWord(map<char, int> mNeededLetters);

	///<summary>
	///<para>
	///Take away the letter from currently held ones
	///</para>
	///</summary>
	void ReduceLetter(char cLetter);

	///<summary>
	///<para>
	///Add new letter to currently held ones
	///</para>
	///</summary>
	void AddLetter(char cLetter);

	///<summary>
	///<para>
	///Returns the current number of letters held by the player
	///</para>
	///</summary>
	int CountLetters();

	///<summary>
	///<para>
	///Currently held score
	///</para>
	///</summary>
	int m_iScore = 0;

	map<char, int> GetHeldLetters();

protected:

	///<summary>
	///<para>
	///Dictionary of currently held letters
	///</para>
	///</summary>
	map<char, int> m_mHeldCharacters;

	///<summary>
	///<para>
	///Turn a word into dictionary of words
	///</para>
	///</summary>
	map<char, int> WordToDictionary(string strWord);

	///<summary>
	///<para>
	///Count missmatches between word letter dictionary and held letter dictionary
	///</para>
	///</summary>
	int CountMissmatches(map<char, int> mWordDictionary);

};

