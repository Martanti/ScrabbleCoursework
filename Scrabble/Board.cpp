#include "headers/Board.h"

/*
 *	Public
 */

CBoard::CBoard() {
	//initialization of board
	for (size_t i = 0; i < m_kiBoardSize; i++)
	{
		this->m_rgcBoard[i] = '0';
	}
}

CBoard::~CBoard() {
	delete this->m_pcBoardProxy;
	delete this->m_pcMultipliersProxy;
}


char* CBoard::GetBoard() {
	for (size_t i = 0; i < m_kiBoardSize; i++) {
		this->m_pcBoardProxy[i] = this->m_rgcBoard[i];
	}

	return this->m_pcBoardProxy;
}

char* CBoard::GetMultipliers() {
	for (size_t i = 0; i < m_kiBoardSize; i++) {
		this->m_pcMultipliersProxy[i] = this->m_rgcMultipliers[i];
	}

	return this->m_pcMultipliersProxy;
}

bool CBoard::CanWordBePlaced(CWord wrdPlaced) {

	//size check
	if (wrdPlaced.m_Direction == LeftToRight) {
		int iPosOfLastChar = wrdPlaced.m_iXCoordinate + wrdPlaced.m_strWordValue.size() - 1; //as the coordinate should be included so -1
		if (iPosOfLastChar > 14) {
			return false;
		}
	}

	else {
		int iPosOfLastChar = wrdPlaced.m_iYCoordinate + wrdPlaced.m_strWordValue.size() - 1; //as the coordinate should be included so -1
		if (iPosOfLastChar > 14) {
			return false;
		}
	}

	int iIndexAtArray = (wrdPlaced.m_iYCoordinate * 15) + wrdPlaced.m_iXCoordinate;
	bool bHasConnectingLetter = false;

	for (size_t i = 0; i < wrdPlaced.m_strWordValue.size(); i++)
	{

		char cBoardLetter = m_rgcBoard[iIndexAtArray];
		char cWordLetter = wrdPlaced.m_strWordValue[i];

		//conflicting letters
		if (cBoardLetter != cWordLetter && cBoardLetter != '0') {
			return false;
		}

		//connecting letters
		if (cBoardLetter == cWordLetter) {
			bHasConnectingLetter = true;
		}

		iIndexAtArray += wrdPlaced.m_Direction == 'l' ? 1 : 15;
	}

	//it means it the first word
	if (wrdPlaced.m_iXCoordinate == 7 && wrdPlaced.m_iYCoordinate == 7) {
		return true;
	}

	//check for a additional words

	return bHasConnectingLetter;
}

void CBoard::PlaceWord(CWord wrdPlaced) {

	int iIndexAtArray = (wrdPlaced.m_iYCoordinate * 15) + wrdPlaced.m_iXCoordinate;

	for (size_t i = 0; i < wrdPlaced.m_strWordValue.size(); i++)
	{
		if (this->m_rgcBoard[iIndexAtArray] != wrdPlaced.m_strWordValue[i]) {
			this->m_rgcBoard[iIndexAtArray] = wrdPlaced.m_strWordValue[i];
		}

		iIndexAtArray += wrdPlaced.m_Direction == 'l' ? 1 : 15;
	}
}

vector<tuple<int/*index*/, char /*w for word and l for letter*/, int /*multiplication ammount*/>> CBoard::GetCrossedMultipliers(CWord wrdPlaced)
{
	vector<tuple<int, char, int>> vecResultMultipliers;

	int iIndexAtArray = (wrdPlaced.m_iYCoordinate * 15) + wrdPlaced.m_iXCoordinate;

	for (int i = 0; i < (int) wrdPlaced.m_strWordValue.size(); i++)
	{
		if (this->m_rgcMultipliers[iIndexAtArray] != 0)
		{
			tuple<char, int> tMultipliers = this->GetMultiplier(this->m_rgcMultipliers[iIndexAtArray]);
			vecResultMultipliers.push_back({ i, std::get<char>(tMultipliers), std::get<int>(tMultipliers) });
		}

		iIndexAtArray += wrdPlaced.m_Direction == 'l' ? 1 : 15;
	}

	return vecResultMultipliers;
}

map<char, int> CBoard::GetNeededCharaters(CWord wrdPlaced) {
	int iIndexAtArray = (wrdPlaced.m_iYCoordinate * 15) + wrdPlaced.m_iXCoordinate;

	map<char, int> mNeededLetters;

	for (size_t i = 0; i < wrdPlaced.m_strWordValue.size(); i++)
	{
		if (this->m_rgcBoard[iIndexAtArray] != wrdPlaced.m_strWordValue[i])
		{
			if (mNeededLetters.count(wrdPlaced.m_strWordValue[i]) > 0)
			{
				mNeededLetters[wrdPlaced.m_strWordValue[i]]++;
			}
			else
			{
				mNeededLetters[wrdPlaced.m_strWordValue[i]] = 1;
			}
		}

		iIndexAtArray += wrdPlaced.m_Direction == 'l' ? 1 : 15;
	}

	return mNeededLetters;
}


vector<CWord> CBoard::GetAllTheAdditionalWords(CWord wrdPlaced) {
	int indexAtArray = (wrdPlaced.m_iYCoordinate * 15) + wrdPlaced.m_iXCoordinate;

	vector<CWord> vecwrdAdditionals;

	for (size_t i = 0; i < wrdPlaced.m_strWordValue.size(); i++) {
		
		char pivotChar = wrdPlaced.m_strWordValue[i];
		int iXAddition = wrdPlaced.m_Direction == LeftToRight ? i : 0;
		int iYAddition = wrdPlaced.m_Direction == UpToDown ? i : 0;
		Directions directionToCheck = wrdPlaced.m_Direction == LeftToRight ? UpToDown : LeftToRight;

		CWord additionalWord = this->RecreateAWord(pivotChar, wrdPlaced.m_iXCoordinate+iXAddition, wrdPlaced.m_iYCoordinate+iYAddition, directionToCheck );
		if (additionalWord.m_strWordValue.size() > 1)
		{
			vecwrdAdditionals.push_back(additionalWord);
		}
	}

	return vecwrdAdditionals;
}

vector<CWord> CBoard::GetIntersectedWords(CWord wrdPlaced) {
	vector<CWord> vecwrdIntersected;

	for (vector<CWord>::iterator it = this->m_vecPlacedWords.begin(); it!= this->m_vecPlacedWords.end(); ++it)
	{
		CWord wrdExisting = *it;

		if (wrdPlaced.m_Direction != wrdExisting.m_Direction)
		{
			/*Checks the ranges of Placed and Existing words. If they have match of their range and others perpendicularAxis point it means they intersect*/
			int iPlacedWordFirstLetterCoordinate =  wrdPlaced.m_Direction == LeftToRight? wrdPlaced.m_iXCoordinate: wrdPlaced.m_iYCoordinate;
			int iPlacedWordLastLetterCoordinate =  iPlacedWordFirstLetterCoordinate + wrdPlaced.m_strWordValue.size()-1;
			int iPlacedWordperpendicularAxis = wrdPlaced.m_Direction == LeftToRight ? wrdPlaced.m_iYCoordinate: wrdPlaced.m_iXCoordinate;


			int iExistingWordFirstLetterCoordinate = wrdExisting.m_Direction == LeftToRight? wrdExisting.m_iXCoordinate: wrdExisting.m_iYCoordinate;
			int iExistingWordLastLetterCoordinate = iExistingWordFirstLetterCoordinate + wrdExisting.m_strWordValue.size();
			int iExistingWordperpendicularAxis = wrdPlaced.m_Direction == LeftToRight ? wrdExisting.m_iYCoordinate : wrdExisting.m_iXCoordinate;

			if ((iPlacedWordperpendicularAxis>= iExistingWordFirstLetterCoordinate && iPlacedWordperpendicularAxis<= iExistingWordLastLetterCoordinate)
				&&
				(iExistingWordperpendicularAxis>= iPlacedWordFirstLetterCoordinate && iExistingWordperpendicularAxis<= iPlacedWordLastLetterCoordinate))
			{
				vecwrdIntersected.push_back(wrdExisting);
			}

		}
	}

	return vecwrdIntersected;
}

void CBoard::RegisterPlacedWord(CWord wrdPlaced) {
	this->m_vecPlacedWords.push_back(wrdPlaced);
}

bool CBoard::HasSameWordAlredyBeenPlaced(CWord wrdPlaced) {
	for (vector<CWord>::iterator it = this->m_vecPlacedWords.begin(); it != this->m_vecPlacedWords.end(); ++it)
	{
		if (*it == wrdPlaced)
		{
			return true;
		}
	}

	return false;
}

vector<CWord> CBoard::GetPlacedWords() {
	return this->m_vecPlacedWords;
}

/*
 *	Private
 */

tuple<char, int> CBoard::GetMultiplier(char cChecked) {

	tuple<char, int> result = { ' ', 0 };

	if (cChecked == '+' || cChecked == '%'){
		//double word
		result = { 'w', 2 };
	}

	else if (cChecked == '&'){
		//triple word
		result = { 'w', 3 };
	}

	else if (cChecked == '@'){
		//double letter
		result = { 'l', 2 };
	}

	else if (cChecked == '#'){
		//triple letter
		result = { 'l', 3 };
	}

	return result;
}

CWord CBoard::RecreateAWord(int pivotXCoordinate, int pivotYCoordinate, Directions direction) {
	int indexAtArray = (pivotYCoordinate * 15) + pivotXCoordinate;
	char letter = this->m_rgcBoard[indexAtArray];
	return RecreateAWord(letter, pivotXCoordinate, pivotYCoordinate, direction);
}

CWord CBoard::RecreateAWord(char cPivotLetter, int iXPivot, int iYPivot, Directions direction) {
	
	if (cPivotLetter == '0'){ //pivot is empty character
		CWord emptyWord;
		return emptyWord;
	}

	int iIndexAtArray = (iYPivot * 15) + iXPivot;
	int iFurtherestToCheckBefore = direction == UpToDown ? iXPivot : iYPivot * 15;
	int iFurtherestToCheckAfter = direction == UpToDown ? iXPivot + 210 /* =14*15 */ : (iYPivot + 1) * 15 - 1;

	int iChangeIncrement = direction == UpToDown ? 15 : 1;

	string strFormedWord = this->GetWordPartBeforePivot(iChangeIncrement, iIndexAtArray, iFurtherestToCheckBefore);
	strFormedWord += cPivotLetter;
	tuple<string, int> tWordAfterPivot = this->GetWordPartAfterPivot(iChangeIncrement, iIndexAtArray, iFurtherestToCheckAfter);
	strFormedWord += std::get<string>(tWordAfterPivot);

	CWord wrdAdditional;
	wrdAdditional.m_strWordValue = strFormedWord;
	wrdAdditional.m_Direction = direction;
	int firstLetterPos = std::get<int>(tWordAfterPivot) - strFormedWord.size() * iChangeIncrement;
	wrdAdditional.m_iXCoordinate = firstLetterPos % 15;
	wrdAdditional.m_iYCoordinate = firstLetterPos / 15;

	return wrdAdditional;
}

string CBoard::GetWordPartBeforePivot(int iDirectionIncrement, int iPivotPositionInArray, int iFurtherestToCheck) {

	string strWordBeforePivot = "";
	int iCheckedIndex = iPivotPositionInArray - iDirectionIncrement;

	while (this->m_rgcBoard[iCheckedIndex] != '0' && iCheckedIndex >= iFurtherestToCheck)
	{
		strWordBeforePivot += this->m_rgcBoard[iCheckedIndex];
		iCheckedIndex -= iDirectionIncrement;
	}

	std::reverse(strWordBeforePivot.begin(), strWordBeforePivot.end());
	return strWordBeforePivot;
}

tuple<string, int> CBoard::GetWordPartAfterPivot(int iDirectionIncrement, int iPivotPositionInArray, int iFurtherestToCheck) {
	string strWordAfterPivot = "";
	int iCheckedIndex = iPivotPositionInArray + iDirectionIncrement;
	while (this->m_rgcBoard[iCheckedIndex] != '0' && iCheckedIndex <= iFurtherestToCheck)
	{
		strWordAfterPivot += this->m_rgcBoard[iCheckedIndex];
		iCheckedIndex += iDirectionIncrement;
	}
	return { strWordAfterPivot, iCheckedIndex };
}