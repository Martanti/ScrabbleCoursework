#pragma once

#include <iostream>
#include <array>
#include <math.h>

#include <string>
#include <tuple>
#include <map>

using std::tuple;
using std::string;
using std::map;

///<summary>
	///<para>
	///Class for managing inputs and outputs
	///</para>
	///</summary>
class CUserInterface
{
public:

	///<summary>
	///<para>
	///Display board with words in it and multipliers
	///</para>
	///</summary>
	void DisplayBoard(char* pcBoard, char* pcMultipliers, int iSize);

	///<summary>
	///<para>
	///Get a word from the user
	///</para>
	///</summary>
	string GetWordFromUser();

	///<summary>
	///<para>
	///Get a coordinates (X position and Y position) of the where first word will start in 0 based indexes. 
	///</para>
	///</summary>
	tuple<int, int> GetPlacedCoordinates(bool &bWasSuccessful);

	///<summary>
	///<para>
	///Get a direction where to put a word
	///</para>
	///</summary>
	char GetDirectionLetter(bool &bWasSuccessful);

	void DisplayHeldLetters(map<char, int> mHeldLetters);

	void DisplayScore(int iScore);

	void DisplayAIScore(int iScore);

	void DisplayMessage(string strMessage);

private:
};

