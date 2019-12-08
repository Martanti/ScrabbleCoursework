#include "headers/WordsMechanics.h"

CWordsMechanics::CWordsMechanics() {}

CWordsMechanics::~CWordsMechanics() {
	delete this->m_pvecPossibleWords;
	delete this->m_pvecPossibleWordsProxy;
}

/*
 *	Public
 */

void CWordsMechanics::Init() {
	CFileManager fileManager;
	vector<string>* pvecLegalWords = fileManager.GetLegalWords();

	for (vector<string>::iterator it = pvecLegalWords->begin(); it != pvecLegalWords->end(); ++it)
	{
		int iWordScore = 0;

		string strWord = *it;

		for (size_t i = 0; i < strWord.size(); i++)
		{
			iWordScore += this->GetScore(strWord[i]);
		}

		this->m_pvecPossibleWords->push_back({strWord, iWordScore});

	}

	delete pvecLegalWords;
}

bool CWordsMechanics::IsLegal(string strWordToCheck) {
	int index = this->GetIndex(strWordToCheck);

	if (index > -1)
		return true;

	else
		return false;
}

int CWordsMechanics::GetScore(string strWord, vector<tuple<int/*index*/, char /*w for word and l for letter*/, int /*multiplication ammount*/>> vecMultipliers) {
	
	int iScore = 0;

	for (size_t i = 0; i < strWord.size(); i++)
	{
		int iLetterScore = this->GetScore(strWord[i]);

		for (vector<tuple<int, char, int>>::iterator it = vecMultipliers.begin(); it != vecMultipliers.end(); ++it)
		{
			if (std::get<0>(*it) == i && std::get<1>(*it) == 'l')
			{
				iLetterScore *= std::get<2>(*it);
			}
		}
		iScore += iLetterScore;

	}

	for (vector<tuple<int, char, int>>::iterator it = vecMultipliers.begin(); it != vecMultipliers.end(); ++it)
	{
		if ( std::get<1>(*it) == 'w')
		{
			iScore *= std::get<2>(*it);
		}
	}

	return iScore;
}

vector<tuple<string, int>>* CWordsMechanics::GetPossibleWords() {
	this->m_pvecPossibleWordsProxy->clear();
	for (vector<tuple<string, int>>::iterator it = this->m_pvecPossibleWords->begin(); it != this->m_pvecPossibleWords->end(); ++it)
	{
		this->m_pvecPossibleWordsProxy->push_back(*it);
	}
	return this->m_pvecPossibleWordsProxy;
}

/*
 *	Private
 */

int CWordsMechanics::GetScore(char cLetter) {

	if (cLetter == 'a' || cLetter == 'e' || cLetter == 'i'
		|| cLetter == 'o' || cLetter == 'u' || cLetter == 'l'
		|| cLetter == 'n' || cLetter == 's' || cLetter == 't' || cLetter == 'r') {
		return 1;
	}

	else if (cLetter == 'd' || cLetter == 'g') {
		return 2;
	}

	else if (cLetter == 'b' || cLetter == 'c' || cLetter == 'm' || cLetter == 'p') {
		return 3;
	}

	else if (cLetter == 'f' || cLetter == 'h' || cLetter == 'v' || cLetter == 'w' || cLetter == 'y') {
		return 4;
	}

	else if (cLetter == 'k') {
		return 5;
	}

	else if (cLetter == 'j' || cLetter == 'x') {
		return 8;
	}

	else if (cLetter == 'q' || cLetter == 'z') {
		return 10;
	}

	else {
		return 0;
	}

}

int CWordsMechanics::GetIndex(string strWord) {

	//binary search
	int iLeftIndex = 0;
	int iRightIndex = this->m_pvecPossibleWords->size()-1;

	while (iLeftIndex <= iRightIndex)
	{
		int iMiddleIndex = (iRightIndex + iLeftIndex) / 2;

		string strWordAtMiddle = std::get<string>(this->m_pvecPossibleWords->at(iMiddleIndex));

		if (strWordAtMiddle == strWord)
		{
			return iMiddleIndex;
		}

		if (strWordAtMiddle > strWord) //word is in the first half
		{
			iRightIndex = iMiddleIndex - 1;
		}

		else //word is second half
		{
			iLeftIndex = iMiddleIndex + 1;
		}
	}

	return -1;
}