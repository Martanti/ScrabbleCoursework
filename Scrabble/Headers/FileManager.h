#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

#include "QuickSort.h"

using std::string;
using std::vector;

///<summary>
	///<para>
	///Class for managing (fidning, creating, reading) external dictionary
	///</para>
	///</summary>
class CFileManager
{
public:

	///<summary>
	///<para>
	///Get all of the legal words from dictionary
	///</para>
	///</summary>
	vector<string>* GetLegalWords();

private:

	///<summary>
	///<para>
	///Name of file containing sorted dictionary containing only legal words
	///</para>
	///</summary>
	static const string m_kstrExternalDictionaryName;

	///<summary>
	///<para>
	///Name of file containing back up dictionary that is unsorted and contains
	///</para>
	///</summary>
	static const string m_kstrBackDictionaryName;

	///<summary>
	///<para>
	///Checks if given word contains illegal characters. Returns TRUE if word is legal for game
	///</para>
	///</summary>
	bool IsWordLegal(string strWord);

	///<summary>
	///<para>
	///Creates external dictionary from reading, filtering and sorting backup dictionary
	///</para>
	///</summary>
	void CreateExternalDictionary();

	///<summary>
	///<para>
	///Checks if external dicktionary exists, returns TRUE if it does
	///</para>
	///</summary>
	bool DoesExternalDictionaryExist();

	///<summary>
	///<para>
	///Write legal words into external dictionary file
	///</para>
	///</summary>
	void WriteExternalDictionary(std::unique_ptr< vector<string>>legalwords);

	///<summary>
	///<para>
	///Read the backup dictionary
	///</para>
	///</summary>
	std::unique_ptr<vector<string>> ReadBackupDictionary();
};

