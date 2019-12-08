#pragma once

#include <string>

using std::string;

///<summary>
	///<para>
	///Enumerators for word directions
	///</para>
	///</summary>
enum Directions { LeftToRight = 'l', UpToDown = 'd' };

///<summary>
	///<para>
	///Structure for passing a word that can be placed in a board
	///</para>
	///</summary>
class CWord
{
public:
	///<summary>
	///<para>
	///Direction where the word is placed.
	///</para>
	///</summary>
	Directions m_Direction;

	///<summary>
	///<para>
	///The word itself
	///</para>
	///</summary>
	string m_strWordValue = "";

	///<summary>
	///<para>
	///X coordinate where the word is placed (from 0 to 14)
	///</para>
	///</summary>
	int m_iXCoordinate = -1;

	///<summary>
	///<para>
	///Y coordinate where the word is placed (from 0 to 14)
	///</para>
	///</summary>
	int m_iYCoordinate = -1;

	///<summary>
	///<para>
	///Compares Direction Word value, Y and X coordinates and checks if all of them are equal
	///</para>
	///</summary>
	bool operator==(const CWord& krwrdcomparedWord) const {
		if (this->m_Direction == krwrdcomparedWord.m_Direction
			&& this->m_strWordValue == krwrdcomparedWord.m_strWordValue
			&& this->m_iXCoordinate == krwrdcomparedWord.m_iXCoordinate
			&& this->m_iYCoordinate == krwrdcomparedWord.m_iYCoordinate)
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	///<summary>
	///<para>
	///Adds word value to the added to object
	///</para>
	///</summary>
	CWord operator+=(CWord const &krwrdAddedWord) {
		CWord wrdResult = *this;
		wrdResult.m_strWordValue += krwrdAddedWord.m_strWordValue;
		return wrdResult;
	}

};
