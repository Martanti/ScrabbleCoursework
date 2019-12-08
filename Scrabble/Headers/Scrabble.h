#pragma once

#include "Board.h"
#include "UI.h"
#include "WordsMechanics.h"
#include "Player.h"
#include "LetterBag.h"
#include "AIPlayer.h"

#include <tuple>
#include <string>
#include <map>

#include <chrono>
#include <ratio>

using std::tuple;
using std::string;
using std::map;

///<summary>
	///<para>
	///Code meant to determine the status received input 
	///</para>
	///</summary>
	enum ResponseCode {Exit, Error, OK, Skip};

///<summary>
	///<para>
	///Class that handles all of the game mechanics
	///</para>
	///</summary>
class CScrabble
{
public:

	///<summary>
	///<para>
	///Start the main game loop
	///</para>
	///</summary>
	void Play();

private:

	///<summary>
	///<para>
	///The amount of letters that a player should hold
	///</para>
	///</summary>
	static const int m_kiLettersInHand = 7;

	CWordsMechanics m_WordMechanics;
	CUserInterface m_UserInterface;
	CBoard m_BoardMechanics;
	CPlayer m_Player;
	CAIPlayer m_AIPlayer;
	CLetterBag m_LetterBag;

	///<summary>
	///<para>
	///Make an AI take a turn
	///</para>
	///</summary>
	void AITurn(bool & bHasSkippedTurn);

	///<summary>
	///<para>
	///Reduce letters from the player held letters
	///</para>
	///</summary>
	void ReduceLettersFromPlayer(map<char, int> mLetters);

	///<summary>
	///<para>
	///Display everything needed for the game
	///</para>
	///</summary>
	void DisplayItems();

	///<summary>
	///<para>
	///Gets user inputs. Additionally returns a response code
	///</para>
	///</summary>
	tuple<ResponseCode, CWord> GetWord(bool bIsFirstTurn);

	///<summary>
	///<para>
	///Refil user hand with letters
	///</para>
	///</summary>
	void RefilUserHand();
	
	///<summary>
	///<para>
	///Does a final check on word legality. If word, its placement and needed letter amount are legal, return TRUE
	///</para>
	///</summary>
	bool FinalWordCheck(CWord wrdPlaced, map<char, int>mNeededLetters);

	///<summary>
	///<para>
	///Check for the word in the given proximity. Returns TRUE and the word if the word is found. Return False and new Word object if not found
	///</para>
	///</summary>
	CWord GetWordInDistance(CWord wrdChecked, int iHowFarAwayToCheck);

	///<summary>
	///<para>
	///Checks if all newly created words on the same axis are legal. If they are returns TRUE and list of those words. If not, returns FALSE and an ampty list.
	///</para>
	///</summary>
	tuple<bool, vector<CWord>> CheckLegalityOfNewlyCreatedWords(CWord wrdChecked);

	///<summary>
	///<para>
	///Checks if there are words behind and infront of the provided word and then combines with the given word
	///</para>
	///</summary>
	 CWord GetExtendedWord(CWord wrdChecked);

	///<summary>
	///<para>
	///Combines all the three words into one. Coordinates are based on first legal word
	///</para>
	///</summary>
	CWord CombineWordWithPreviousAndAfterWord(CWord wrdPrevious, CWord wrdCentral, CWord wrdAfter);

	///<summary>
	///<para>
	///Divides the board into 4 squares and records all the words that start there
	///</para>
	///</summary>
	vector<vector<CWord>>GetSquares();

	///<summary>
	///<para>
	///Get iterator of all Squares vector of a square that has the smallest amount of words in it, but still has words
	///</para>
	///</summary>
	vector<vector<CWord>>::iterator GetSmallestSquare(vector<vector<CWord>>& vecSqaure);

	///<summary>
	///<para>
	///Get dictionary of letter of all the placed words
	///</para>
	///</summary>
	map<char, int> GetLettersFromWords(vector<CWord> vecWords);

	///<summary>
	///<para>
	///Combines two dictionaries
	///</para>
	///</summary>
	map<char, int> CombineUsedLetters(map<char, int> mInHand, map<char, int> mFromWords);

	///<summary>
	///<para>
	///Get all the possible words that cross placed words
	///</para>
	///</summary>
	vector<CWord> GetWordsThatCanBePlaced(vector<CWord>vecPlacedWords, vector<string> vecPossibleWords, bool bFirstTurn);

	///<summary>
	///<para>
	///Filter legal words by checking if they can be placed, have enough letters and 
	///</para>
	///</summary>
	vector<tuple<CWord, vector<CWord>>> FilterLegalyPlacableWords(vector<CWord> vecIntersectingWords);

	///<summary>
	///<para>
	///Reduce used letters from AI hand
	///</para>
	///</summary>
	void ReduceLetterFromAI(map<char, int> mLetters);

	///<summary>
	///<para>
	///Refil AI player hand with letter if they exist
	///</para>
	///</summary>
	void RefilAIHand();

};

