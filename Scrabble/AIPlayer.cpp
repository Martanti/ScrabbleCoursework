#include "headers/AIPlayer.h"

CAIPlayer::CAIPlayer() {

}
CAIPlayer::~CAIPlayer() {
}

/*
 *	Public
 */

void CAIPlayer::InitAllowedWords(vector<tuple<string, int>>* vecLegalWords) {
	for (vector<tuple<string, int>>::iterator it = vecLegalWords->begin(); it!= vecLegalWords->end(); ++it){
		string strWord = std::get<string>(*it);
		int iWordLegth = strWord.size();
		if (iWordLegth == 3|| iWordLegth == 4 || iWordLegth == 7){
			string strTempWord = this->SortWord(strWord);
			this->m_mUsableWords[strTempWord].push_back(strWord);
		}
	}
}

vector<string> CAIPlayer::GetAvailableWords(map<char, int> availableCharacters) {
	string strAvailableLetters = this->DictionaryToString(availableCharacters);
	int iSizeOfLetters = strAvailableLetters.size();
	unique_ptr<vector<string>> pvecAllWords(new vector<string>);

	if (iSizeOfLetters>=3){
		unique_ptr<vector<string>> pvecThreeLetterWords = this->GetThreeCombinations(strAvailableLetters);
		pvecAllWords->insert(pvecAllWords->end(), pvecThreeLetterWords->begin(), pvecThreeLetterWords->end());
	}
	
	if (iSizeOfLetters >= 4){
		unique_ptr <vector<string>>vecFourLetterWords = this->GetFourCombinations(strAvailableLetters);
		pvecAllWords->insert(pvecAllWords->end(), vecFourLetterWords->begin(), vecFourLetterWords->end());
	}

	
	if (iSizeOfLetters>=7){
		unique_ptr <vector<string>> vecSevenLetterWords = this->GetSevenCombinations(strAvailableLetters);
		pvecAllWords->insert(pvecAllWords->end(), vecSevenLetterWords->begin(), vecSevenLetterWords->end());
	}
	
	pvecAllWords = this->AddForBlanks(std::move(pvecAllWords));

	vector<string> vecAvailableWords;
	for (vector<string>::iterator it = pvecAllWords->begin(); it != pvecAllWords->end(); ++it){

		if (this->m_mUsableWords.count(*it)>0){
			vector<string> vecWords = this->m_mUsableWords[*it];
			vecAvailableWords.insert(vecAvailableWords.end(), vecWords.begin(), vecWords.end());
		}
	}
	return vecAvailableWords;
}

/*
 *	Private
 */

string CAIPlayer::SortWord(string strSorted) {
	CQuickSort srtSorting;
	vector<char> vecSortedWord(strSorted.begin(), strSorted.end());
	//std::unique_ptr<vector<char>> vecSorted= srtSorting.Quicksort(vecSortedWord);
	srtSorting.Quicksort(vecSortedWord);
	string strSortedWord;
	for (vector<char>::iterator it = /*vecSorted->*/vecSortedWord.begin(); it!= /*vecSorted->*/vecSortedWord.end(); ++it)
	{
		strSortedWord += *it;
	}
	//string strSortedWord(vecSorted->begin(), vecSorted->end());
	return strSortedWord;
}

unique_ptr<vector<string>> CAIPlayer::GetThreeCombinations(string strAllowedLetters) {
	unique_ptr<vector<string>> pvecLetterCombinations( new vector<string>());
	unique_ptr<map<string, bool>> pmLetterCombinations(new map<string, bool>());//for cheking up for unique combinations
	int iSize = strAllowedLetters.size();
	for (int i = 0; i < iSize; i++){

		for (int j = 0; j < iSize; j++){
			if (j == i){
				continue;
			}

			for (int k = 0; k < iSize; k++){
				if (k == j || k == i){
					continue;
				}

				string strBuilt = "";
				strBuilt += strAllowedLetters[i];
				strBuilt += strAllowedLetters[j];
				strBuilt += strAllowedLetters[k];

				string strBuilSorted = this->SortWord(strBuilt);

				if (this->m_mUsableWords.count(strBuilSorted) >0)
				{
					if (pmLetterCombinations->count(strBuilSorted) > 0)
					{
						continue;
					}
					else
					{
						(*pmLetterCombinations)[strBuilSorted] = true;
						pvecLetterCombinations->push_back(strBuilSorted);
					}
				}
			}
		}
	}
	return pvecLetterCombinations;
}

unique_ptr<vector<string>> CAIPlayer::GetFourCombinations(string strAllowedCharacters) {
	unique_ptr<vector<string>> pvecLetterCombinations( new vector<string>());
	unique_ptr<map<string, bool>> pmLetterCombinations( new map<string, bool>());
	int iSize = strAllowedCharacters.size();
	for (int i = 0; i < iSize; i++) {

		for (int j = 0; j < iSize; j++){
			if (j == i) {
				continue;
			}

			for (int k = 0; k < iSize; k++){
				if (k == j || k == i) {
					continue;
				}

				for (int y = 0; y < iSize; y++){
					if (y == i || y == j || y== k){
						continue;
					}

					string strBuilt = "";
					strBuilt += strAllowedCharacters[i];
					strBuilt += strAllowedCharacters[k];
					strBuilt += strAllowedCharacters[j];
					strBuilt += strAllowedCharacters[y];
					string strBuiltSorted = this->SortWord(strBuilt);

					if (this->m_mUsableWords.count(strBuiltSorted) > 0)
					{
						if (pmLetterCombinations->count(strBuiltSorted) > 0)
						{
							continue;
						}
						else
						{
							(*pmLetterCombinations)[strBuiltSorted] = true;
							pvecLetterCombinations->push_back(strBuiltSorted);
						}
					}
					
				}
			}
		}
	}
	return pvecLetterCombinations;
}

unique_ptr<vector<string>> CAIPlayer::GetSevenCombinations(string strAllowedLetters) {
	int iSize = strAllowedLetters.size();
	unique_ptr <vector<string>> pvecCombinations ( new vector<string>());
	unique_ptr <map<string, bool>> pmCombinations ( new map < string, bool>());
	for (int i1 = 0; i1 < iSize; i1++)
	{
		for (int i2 = 0; i2 < iSize; i2++)
		{
			if (i2 == i1){
				continue;
			}
			for (int i3 = 0; i3 < iSize; i3++)
			{
				if (i3 == i2 || i3 == i1){
					continue;
				}
				for (int i4 = 0; i4 < iSize; i4++)
				{
					if (i4 == i3 || i4 == i2 || i4 == i1){
						continue;
					}
					for (int i5 = 0; i5 < iSize; i5++)
					{
						if (i5 == i4 || i5 == i3 || i5 == i2 || i5 == i1){
							continue;
						}
						for (int i6 = 0; i6 < iSize; i6++)
						{
							if (i6 == i5 || i6 == i4 || i6 == i3 || i6 == i2 || i6 == i1){
								continue;
							}
							for (int i7 = 0; i7  < iSize; i7 ++)
							{
								if (i7 == i6 || i7 == i5 || i7 ==i4 || i7 == i3 || i7 == i2 || i7 == i1){
									continue;
								}
								string strBuilt = "";
								strBuilt += strAllowedLetters[i1];
								strBuilt += strAllowedLetters[i2];
								strBuilt += strAllowedLetters[i3];
								strBuilt += strAllowedLetters[i4];
								strBuilt += strAllowedLetters[i5];
								strBuilt += strAllowedLetters[i6];
								strBuilt += strAllowedLetters[i7];

								string strBuiltSorted = this->SortWord(strBuilt);

								if (this->m_mUsableWords.count(strBuiltSorted))
								{
									if (pmCombinations->count(strBuiltSorted) > 0) {
										continue;
									}
									else {
										(*pmCombinations)[strBuiltSorted] = true;
										pvecCombinations->push_back(strBuiltSorted);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return pvecCombinations;
}

unique_ptr <vector<string>> CAIPlayer::AddForBlanks(unique_ptr<vector<string>> pvecAllTheWords) {
	bool bAllCleared = false;	
	while (!bAllCleared)
	{
		bAllCleared = true;
		map<string, bool> mLetterCombinations;
		vector<string> vecTemp;
		for (vector<string>::iterator it = pvecAllTheWords->begin(); it != pvecAllTheWords->end(); ++it){
			string strWord = *it;
			int iWordSize = strWord.size();
			for (int i = 0; i < iWordSize; i++){
				if (strWord[i] == '?'){
					bAllCleared = false;

					for (int j = 97; j < 123; j++)
					{
						string strTempWord = strWord;
						strTempWord[i] = (char)j;
						strTempWord = this->SortWord(strTempWord);
						if (mLetterCombinations.count(strTempWord) >0){
							continue;
						}
						else{
							mLetterCombinations[strTempWord] = true;
							vecTemp.push_back(strTempWord);
						}
					}
					break;
				}
			}	
		}
		this->DeleteBlanks(*pvecAllTheWords);		
		pvecAllTheWords->insert(pvecAllTheWords->end(), vecTemp.begin(), vecTemp.end());
	}
	return pvecAllTheWords;
}

void CAIPlayer::DeleteBlanks(vector<string>& vecAllTheWords) {
	bool bNeedsDeletion = true;

	while (bNeedsDeletion)
	{
		bNeedsDeletion = false;
		vector<string>::iterator itToDelete;
		for (vector<string>::iterator it = vecAllTheWords.begin(); it != vecAllTheWords.end(); ++it) {
			bool bShouldBreakOut = false;
			int iWordSize = it->size();
			for (int i = 0; i < iWordSize; i++)
			{
				if (it->at(i) == '?')
				{
					bShouldBreakOut = true;
					bNeedsDeletion = true;
					itToDelete = it;
				}
			}
			if (bShouldBreakOut)
			{
				break;
			}
		}
		if (bNeedsDeletion)
		{
			vecAllTheWords.erase(itToDelete);
		}
	}
}

string CAIPlayer::DictionaryToString(map<char, int> mLetters) {
	string strLetters = "";
	for (map<char, int>::iterator it = mLetters.begin(); it != mLetters.end(); ++it)
	{
		int iSize = it->second;
		char cLetter = it->first;
		for (int i = 0; i < iSize; i++)
		{
			strLetters += cLetter;
		}
	}
	return strLetters;
}