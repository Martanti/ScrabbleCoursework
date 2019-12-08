#pragma once

#include <iostream>
#include <fstream>

#include <tuple>
#include <string>
#include <vector>
#include <map>

#include "FileManager.h"

using std::string;
using std::vector;
using std::tuple;
using std::map;

///<summary>
	///<para>
	///Class for handling legal words
	///</para>
	///</summary>
class CWordsMechanics
{
public:

	CWordsMechanics();
	~CWordsMechanics();

	///<summary>
	///<para>
	///Initialize the legal word dictionary and calculate basic score
	///</para>
	///</summary>
	void Init();

	///<summary>
	///<para>
	///Checks a given word for validaty. Returns TRUE if legal
	///</para>
	///</summary>
	bool IsLegal(string strWordToCheck);

	///<summary>
	///<para>
	///Calculates score of the word keeping in mind all the multipliers
	///</para>
	///</summary>
	int GetScore(string strWord, vector<tuple<int/*index*/, char /*w for word and l for letter*/, int /*multiplication ammount*/>> vecMultipliers);

	///<summary>
	///<para>
	///Get a copy of legal words
	///</para>
	///</summary>
	vector<tuple<string, int>>* GetPossibleWords();

private:

	///<summary>
	///<para>
	///List that contain all legal words and their normal score
	///</para>
	///</summary>
	vector<tuple<string, int>>* m_pvecPossibleWords = new vector<tuple<string, int>>();
	//stored in heap because how big it will get

	///<summary>
	///<para>
	///Copy of legal words list
	///</para>
	///</summary>
	vector<tuple<string, int>>* m_pvecPossibleWordsProxy = new vector<	tuple<string, int>>();


	///<summary>
	///<para>
	///Calculates basic score of a word and returns it
	///</para>
	///</summary>
	int GetScore(char cLetter);


	///<summary>
	///<para>
	///Returns index of given word in legal words array
	///</para>
	///</summary>
	int GetIndex(string strWord);


};