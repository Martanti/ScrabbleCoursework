#pragma once

#include <map>
#include <chrono>
#include <ratio>

#include <string>

#include <iostream>

using std::map;
using std::string;

///<summary>
	///<para>
	///Class for handling letter bag
	///</para>
	///</summary>
class CLetterBag
{
public:

	///<summary>
	///<para>
	///Initialize the bag with the values
	///</para>
	///</summary>
	void InitLeterBag();

	///<summary>
	///<para>
	///Draw a single letter from the bag
	///</para>
	///</summary>
	char DrawALetter();

	///<summary>
	///<para>
	///Check if there are any letters to draw.
	///<para/>
	///Use before DrawLetter in order to avoid bugs.
	///</para>
	///</summary>
	bool CheckIfNotEmpty();

private:

	///<summary>
	///<para>
	///A string containing all the letters in one place so that chance take a letter is realistic
	///</para>
	///</summary>
	string m_strLetterBag = "";
	//map<char, int> letterBag;

	///<summary>
	///<para>
	///Generate a string by adding a character to itself multiple times
	///</para>
	///</summary>
	string GenerateMultipleLetterString(char cSymbol, int iTimesToMultiply);

};

