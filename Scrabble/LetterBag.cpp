#include "Headers/LetterBag.h"


/*
 *	Public
 */
void CLetterBag::InitLeterBag() {
	

	map<char, int> mLetterBagDictionary = { {'a', 9}, {'b', 2}, {'c', 2}, {'d', 4}, {'e', 12}, {'f', 2},
						{'g', 3}, {'h', 2}, {'i', 9}, {'j', 1}, {'k', 1}, {'l', 4},
						{'m', 2}, {'n', 6}, {'o', 8}, {'p', 2}, {'q', 1}, {'r', 6},
						{'s', 4}, {'t', 6}, {'u', 4}, {'v', 2}, {'w', 2}, {'x', 1},
						{'y', 2}, {'z', 1}, {'?', 2} };	
					
	for (map<char, int>::iterator it = mLetterBagDictionary.begin(); it != mLetterBagDictionary.end(); ++it)
	{
		this->m_strLetterBag += this->GenerateMultipleLetterString(it->first, it->second);
	}

}

char CLetterBag::DrawALetter() {
	srand((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
	int iRandomIndex = rand() % this->m_strLetterBag.size();
	char cLetter = this->m_strLetterBag[iRandomIndex];
	this->m_strLetterBag.erase(this->m_strLetterBag.begin() + iRandomIndex);
	return cLetter;
}

bool CLetterBag::CheckIfNotEmpty() {
	return this->m_strLetterBag.size() > 0;
}

/*
 *	Private
 */

string CLetterBag::GenerateMultipleLetterString(char cSymbol, int iTimesToMultiply) {
	string strGeneratedLine = "";
	for (int i = 0; i < iTimesToMultiply; i++){
		strGeneratedLine += cSymbol;
	}

	return strGeneratedLine;
}
