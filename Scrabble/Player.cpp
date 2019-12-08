#include "headers/Player.h"

/*
 *	Public
 */

bool CPlayer::HasLettersForWord(map<char, int> mNeededLetters) {

	int iMissmatches = this->CountMissmatches(mNeededLetters);

	if (iMissmatches>0){

		if (this->m_mHeldCharacters.count('?') > 0){

			if (this->m_mHeldCharacters['?'] - iMissmatches >=0){
				return true;
			}
			
		}
		return false;
	}

	else{
		return true;
	}
}


void CPlayer::AddLetter(char cLetter) {

	if (this->m_mHeldCharacters.count(cLetter) > 0)
	{
		this->m_mHeldCharacters[cLetter]++;
	}
	else
	{
		this->m_mHeldCharacters[cLetter] = 1;
	}
}

void CPlayer::ReduceLetter(char cLetter) {

	if (this->m_mHeldCharacters.count(cLetter)>0){
		this->m_mHeldCharacters[cLetter]--;

		if (this->m_mHeldCharacters[cLetter] < 1)
		{
			this->m_mHeldCharacters.erase(cLetter);
		}
	}

	else{
		this->m_mHeldCharacters['?']--;

		if (this->m_mHeldCharacters['?'] < 1)
		{
			this->m_mHeldCharacters.erase('?');
		}
	}
}

int CPlayer::CountLetters() {
	int iLetters = 0;

	for (map<char, int>::iterator it = this->m_mHeldCharacters.begin(); it != this->m_mHeldCharacters.end(); ++it)
	{
		iLetters += it->second;
	}

	return iLetters;
}

map<char, int> CPlayer::GetHeldLetters() {
	return this->m_mHeldCharacters;
}

/*
 *	Private
 */

map<char, int> CPlayer::WordToDictionary(string sWord) {

	map<char, int> mLettersInWord;

	//dictionary of charaters in the word
	for (size_t i = 0; i < sWord.size(); i++) {
		char cLetter = sWord[i];

		if (mLettersInWord.count(cLetter) > 0) {
			mLettersInWord[cLetter]++;
		}

		else {
			mLettersInWord[cLetter] = 1;
		}
	}

	return mLettersInWord;
}

int CPlayer::CountMissmatches(map<char, int> wordDictionary) {
	int iMissmatches = 0;

	for (map<char, int>::iterator it = wordDictionary.begin(); it != wordDictionary.end(); ++it)
	{
		char cLetter = it->first;
		int iCount = it->second;

		if (this->m_mHeldCharacters.count(cLetter) > 0) {

			int iDifferenceInLetters = this->m_mHeldCharacters[cLetter] - iCount;

			if (iDifferenceInLetters < 0)
			{
				iMissmatches += iDifferenceInLetters;
			}
		}

		else {
			iMissmatches += iCount;
		}
	}

	return iMissmatches;
}