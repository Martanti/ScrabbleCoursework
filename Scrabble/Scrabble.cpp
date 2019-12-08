#include "headers/Scrabble.h"

/*
 *	Public
 */

void CScrabble::Play() {

	std::cout << "Exit the game by inputing ~~ when asked for a word to input.\n";
	m_WordMechanics.Init();

	bool bIsFirstTurn = true;

	m_LetterBag.InitLeterBag();

	for (size_t i = 0; i < this->m_kiLettersInHand; i++)
	{
		if (this->m_LetterBag.CheckIfNotEmpty()) {
			this->m_Player.AddLetter(this->m_LetterBag.DrawALetter());
			this->m_AIPlayer.AddLetter(this->m_LetterBag.DrawALetter());
		}
	}

	m_AIPlayer.InitAllowedWords(this->m_WordMechanics.GetPossibleWords());

	bool bHasAISkipped = false;

	while (true)
	{
		this->DisplayItems();

		bool bShouldSkip = false;

		tuple<ResponseCode, CWord> tRecievedInput = this->GetWord(bIsFirstTurn);
		ResponseCode responseCode = std::get<0>(tRecievedInput);
		if (responseCode == Exit) {
			break;
		}
		else if (responseCode == Error) {
			continue;
		}
		else if (responseCode == Skip)
		{

			if ( bHasAISkipped )
			{
				this->m_UserInterface.DisplayMessage("Game over!");
				if (m_AIPlayer.m_iScore>m_Player.m_iScore){
					this->m_UserInterface.DisplayMessage("AI won");
				}

				else if (m_AIPlayer.m_iScore < m_Player.m_iScore) {
					this->m_UserInterface.DisplayMessage("Player won");
				}
				else {
					this->m_UserInterface.DisplayMessage("Friendship won");
				}

				break;
			}

			else {
				bShouldSkip = true;
			}

		}


		if (!bShouldSkip)
		{


			CWord wrdInput = std::get<CWord>(tRecievedInput);

			if (this->m_BoardMechanics.HasSameWordAlredyBeenPlaced(wrdInput))
			{
				this->m_UserInterface.DisplayMessage("This exact word has already been placed in this position!\n");
				continue;
			}

			map<char, int> mNeededLetters = m_BoardMechanics.GetNeededCharaters(wrdInput);

			wrdInput = this->GetExtendedWord(wrdInput);

			tuple<bool, vector<CWord>> tAdditionallyCreatedWorsCheck = this->CheckLegalityOfNewlyCreatedWords(wrdInput);
			if (!(std::get<bool>(tAdditionallyCreatedWorsCheck))) {
				this->m_UserInterface.DisplayMessage("While adding the word new words were created and at least one of those words is illegal\n");
				continue;
			}
			vector<CWord> vecwrdAdditionals = std::get<vector<CWord>>(tAdditionallyCreatedWorsCheck);

			if (!(this->m_WordMechanics.IsLegal(wrdInput.m_strWordValue))) {
				this->m_UserInterface.DisplayMessage("A new word \"" + wrdInput.m_strWordValue + "\" was created by placing your word, however this word is not a legal word\n");
				continue;
			}

			if (!(this->FinalWordCheck(wrdInput, mNeededLetters))) {
				continue;
			}

			if (bIsFirstTurn) {
				bIsFirstTurn = false;
			}

			m_BoardMechanics.PlaceWord(wrdInput);
			m_BoardMechanics.RegisterPlacedWord(wrdInput);
			ReduceLettersFromPlayer(mNeededLetters);

			vector<tuple<int, char, int>> crossedMultipliers = m_BoardMechanics.GetCrossedMultipliers(wrdInput);

			m_Player.m_iScore += m_WordMechanics.GetScore(wrdInput.m_strWordValue, crossedMultipliers);
			for (vector<CWord>::iterator it = vecwrdAdditionals.begin(); it != vecwrdAdditionals.end(); ++it)
			{
				vector<tuple<int, char, int>> vectCrossedMultipliersForAdditionals = m_BoardMechanics.GetCrossedMultipliers(*it);
				m_Player.m_iScore += m_WordMechanics.GetScore(it->m_strWordValue, vectCrossedMultipliersForAdditionals);
			}
			this->RefilUserHand();
		}

		this->m_UserInterface.DisplayMessage("AI taking a turn\n");
		AITurn(bHasAISkipped);
	}
}

/*
 *	Private
 */

void CScrabble::ReduceLettersFromPlayer(map<char, int> mLetters) {

	for (map<char, int>::iterator it = mLetters.begin(); it != mLetters.end(); ++it)
	{
		int iLetterCount = it->second;

		for (int i = 0; i < iLetterCount; i++)
		{
			this->m_Player.ReduceLetter(it->first);
		}
	}
}

void CScrabble::DisplayItems() {
	this->m_UserInterface.DisplayMessage("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	this->m_UserInterface.DisplayMessage("Write _skip to skip a turn\n");
	this->m_UserInterface.DisplayBoard(this->m_BoardMechanics.GetBoard(), this->m_BoardMechanics.GetMultipliers(), this->m_BoardMechanics.m_kiBoardSize);
	this->m_UserInterface.DisplayScore(this->m_Player.m_iScore);
	this->m_UserInterface.DisplayAIScore(this->m_AIPlayer.m_iScore);
	this->m_UserInterface.DisplayHeldLetters(this->m_Player.GetHeldLetters());
}

tuple<ResponseCode, CWord> CScrabble::GetWord(bool bIsFirstTurn) {
	//word
	CWord wrdInput;
	string strWord = this->m_UserInterface.GetWordFromUser();
	//exit
	if (strWord == "~~") {
		return { Exit, wrdInput };
	}

	if (strWord == "_skip") {
		return { Skip, wrdInput };
	}

	//wrong word
	if (!m_WordMechanics.IsLegal(strWord)) {
		this->m_UserInterface.DisplayMessage("Such word does not exist in the library.\nStarting Over!\n");
		return { Error, wrdInput };
	}

	bool bWasSuccessful = true;
	//coordinates
	tuple<int, int> tCoordinates = bIsFirstTurn ? std::make_tuple(7, 7) : m_UserInterface.GetPlacedCoordinates(bWasSuccessful);

	//wrong coordinates
	if (!bWasSuccessful) {
		this->m_UserInterface.DisplayMessage("Problem with inputinig coordinates.\nStarting Over!\n");
		return { Error, wrdInput };
	}

	int iXCoordinate = std::get<0>(tCoordinates);
	int iYCoordinate = std::get<1>(tCoordinates);

	//direction
	char cDirectionLetter = m_UserInterface.GetDirectionLetter(bWasSuccessful);
	//wrong direction
	if (!bWasSuccessful) {
		this->m_UserInterface.DisplayMessage("Problem with inputinig direction.\nStarting Over!\n");
		return { Error, wrdInput };
	}

	wrdInput.m_Direction = cDirectionLetter == 'l' ? LeftToRight : UpToDown;
	wrdInput.m_strWordValue = strWord;
	wrdInput.m_iXCoordinate = iXCoordinate;
	wrdInput.m_iYCoordinate = iYCoordinate;

	return { OK, wrdInput };
}

void CScrabble::RefilUserHand() {
	int iDifference = (m_kiLettersInHand - m_Player.CountLetters());

	for (int i = 0; i < iDifference; i++)
	{
		if (m_LetterBag.CheckIfNotEmpty())
		{
			m_Player.AddLetter(m_LetterBag.DrawALetter());
		}
	}
}

bool CScrabble::FinalWordCheck(CWord wrdChecked, map<char, int> mNeededLetters) {
	if (!(this->m_BoardMechanics.CanWordBePlaced(wrdChecked))) {
		this->m_UserInterface.DisplayMessage("Word could not be placed in provided coordinates.\nStarting Over!\n");
		return false;
	}

	if (!(this->m_Player.HasLettersForWord(mNeededLetters))) {
		this->m_UserInterface.DisplayMessage("You do not have enough letters for this word.\nStarting Over!\n");
		return false;
	}

	return true;
}

tuple<bool, vector<CWord>> CScrabble::CheckLegalityOfNewlyCreatedWords(CWord wrdChecked) {
	vector<CWord> createdWords;
	//check if placing couple words next to each other creates a new word

	vector<CWord> crossedWords = this->m_BoardMechanics.GetIntersectedWords(wrdChecked);
	vector<CWord> additionalWords = this->m_BoardMechanics.GetAllTheAdditionalWords(wrdChecked);
	vector<vector<CWord>::iterator> matchedCrossedWords;

	for (vector<CWord>::iterator crossedWordsItterator = crossedWords.begin(); crossedWordsItterator != crossedWords.end(); ++crossedWordsItterator)
	{
		for (vector<CWord>::iterator additionalWordIterator = additionalWords.begin(); additionalWordIterator != additionalWords.end(); ++additionalWordIterator)
		{
			if ((*crossedWordsItterator) == (*additionalWordIterator) && additionalWordIterator != additionalWords.end() && crossedWordsItterator != crossedWords.end())
			{
				matchedCrossedWords.push_back(additionalWordIterator);
			}
		}
	}

	int iMatchedSize = matchedCrossedWords.size();
	for (int i = 0; i < iMatchedSize; i++)
	{
		additionalWords.erase(matchedCrossedWords[i]);
	}

	for (vector<CWord>::iterator it = additionalWords.begin(); it != additionalWords.end(); ++it)
	{
		if (this->m_WordMechanics.IsLegal(it->m_strWordValue)) {
			createdWords.push_back(*it);
		}

		else {
			return { false, {} };
		}
	}

	return { true, createdWords };
}

CWord CScrabble::GetExtendedWord(CWord wrdChecked) {

	CWord checkPreviousWord = this->GetWordInDistance(wrdChecked, -1);
	CWord checkAftersWord = this->GetWordInDistance(wrdChecked, wrdChecked.m_strWordValue.size());

	CWord proxy = this->CombineWordWithPreviousAndAfterWord(checkPreviousWord, wrdChecked, checkAftersWord);
	return proxy;

}

CWord CScrabble::GetWordInDistance(CWord wrdChecked, int iHowFarAwayToCheck) {
	int yPosInDistance = wrdChecked.m_Direction == 'l' ? wrdChecked.m_iYCoordinate : wrdChecked.m_iYCoordinate + iHowFarAwayToCheck;
	int xPosInDistance = wrdChecked.m_Direction == 'l' ? wrdChecked.m_iXCoordinate + iHowFarAwayToCheck : wrdChecked.m_iXCoordinate;

	CWord fetchedWord = this->m_BoardMechanics.RecreateAWord(xPosInDistance, yPosInDistance, wrdChecked.m_Direction);

	return fetchedWord;
}

CWord CScrabble::CombineWordWithPreviousAndAfterWord(CWord wrdPrevious, CWord wrdCenter, CWord wrdAfter) {
	CWord proxy;

	if (wrdPrevious.m_strWordValue.empty() || wrdPrevious.m_strWordValue == "0") {
		proxy = wrdCenter;
	}
	else {
		proxy = wrdPrevious;
		proxy += wrdCenter;
	}

	if (wrdAfter.m_strWordValue.empty() || wrdPrevious.m_strWordValue == "0") {
		return proxy;
	}

	else {
		proxy += wrdAfter;
		return proxy;
	}
}

void CScrabble::AITurn(bool & bHasSkippedTurn) {
	vector<vector<CWord>> vecSquare = this->GetSquares();

	bool bFirstTurn = false;
	int iEmptySquares = 0;
	for (vector<CWord> vecSmallSquare: vecSquare){
		if (vecSmallSquare.empty()){
			iEmptySquares++;
		}
	}
	if (iEmptySquares == 4){
		bFirstTurn = true;
	}
	bHasSkippedTurn = false;
	while (true){
		vector<vector<CWord>>::iterator smallestSqure = this->GetSmallestSquare(vecSquare);

		map<char, int> mUsedLetters;
		if (!bFirstTurn){
			if (smallestSqure == vecSquare.end()) {
				bHasSkippedTurn = true;
				this->m_UserInterface.DisplayMessage("AI skipped a turn\n");
				return;
			}
			else {
				vector<CWord> vecChoosenSquare = *smallestSqure;
				vecSquare.erase(smallestSqure);
				mUsedLetters = this->GetLettersFromWords(vecChoosenSquare);
			}
		}
		map<char, int>mHeldLetters = this->m_AIPlayer.GetHeldLetters();
		map<char, int> mUsableLetters = this->CombineUsedLetters(mHeldLetters, mUsedLetters);
		vector<string> vecPossibleWords = this->m_AIPlayer.GetAvailableWords(mUsableLetters);

		vector<CWord> vecIntersectingWords = this->GetWordsThatCanBePlaced(this->m_BoardMechanics.GetPlacedWords(), vecPossibleWords, bFirstTurn);
		vector<tuple<CWord, vector<CWord>>> vecLegalPlacableWords = this->FilterLegalyPlacableWords(vecIntersectingWords);
		if (vecLegalPlacableWords.empty()){
			continue;
		}
		srand((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
		int iRandomIndex = rand() % vecLegalPlacableWords.size();

		tuple<CWord, vector<CWord>> tSelectedWord = vecLegalPlacableWords[iRandomIndex];
		CWord wrdSelected = std::get<CWord>(tSelectedWord);
		vector<CWord> vecAdjacent = std::get<vector<CWord>>(tSelectedWord);

		this->m_BoardMechanics.PlaceWord(wrdSelected);
		this->m_BoardMechanics.RegisterPlacedWord(wrdSelected);

		map<char, int>mLetters = this->m_BoardMechanics.GetNeededCharaters(wrdSelected);
		this->ReduceLetterFromAI(mLetters);
		this->RefilAIHand();
		
		vector<tuple<int, char, int>> vecMiltipliers = this->m_BoardMechanics.GetCrossedMultipliers(wrdSelected);
		this->m_AIPlayer.m_iScore += this->m_WordMechanics.GetScore(wrdSelected.m_strWordValue, vecMiltipliers);
		for (vector<CWord>::iterator it= vecAdjacent.begin(); it!= vecAdjacent.end(); ++it)
		{
			vector<tuple<int, char, int>> vecAdjacentMiltipliers = this->m_BoardMechanics.GetCrossedMultipliers(*it);
			this->m_AIPlayer.m_iScore += this->m_WordMechanics.GetScore(wrdSelected.m_strWordValue, vecAdjacentMiltipliers);
		}

		this->m_UserInterface.DisplayMessage("AI placed a word: " + wrdSelected.m_strWordValue + "\n" );
		return;
	}
}

vector<vector<CWord>> CScrabble::GetSquares() {
	vector<CWord> vecFirstSquare;
	vector<CWord> vecSecondSquare;
	vector<CWord> vecThirdSquare;
	vector<CWord> vecFourthSquare;

	vector<CWord> vecPlacedWords = this->m_BoardMechanics.GetPlacedWords();
	for (vector<CWord>::iterator it = vecPlacedWords.begin(); it != vecPlacedWords.end(); ++it)
	{
		int iXCoordinate = it->m_iXCoordinate;
		int iYCoordinate = it->m_iYCoordinate;
		//first square
		if (iXCoordinate < 7 && iYCoordinate < 7)
		{
			vecFirstSquare.push_back(*it);
		}

		//second square
		else if (iXCoordinate >= 7 && iYCoordinate < 7)
		{
			vecSecondSquare.push_back(*it);
		}

		//third square
		else if (iXCoordinate < 7 && iYCoordinate >= 7)
		{
			vecThirdSquare.push_back(*it);
		}

		//fourth square
		else
		{
			vecFourthSquare.push_back(*it);
		}
	}

	vector<vector<CWord>> vecSquare = { vecFirstSquare, vecSecondSquare, vecThirdSquare, vecFourthSquare };
	return vecSquare;
}

vector<vector<CWord>>::iterator CScrabble::GetSmallestSquare(vector<vector<CWord>>& vecSquare) {
	vector<vector<CWord>>::iterator smallestSqure = vecSquare.end();
	int iSmallestNumber = INT32_MAX;
	for (vector<vector<CWord>>::iterator it = vecSquare.begin(); it != vecSquare.end(); ++it)
	{
		int iSize = it->size();
		if (iSize > 0 && iSize < iSmallestNumber)
		{
			smallestSqure = it;
			iSmallestNumber = iSize;
		}
	}
	return smallestSqure;
}

map<char, int> CScrabble::GetLettersFromWords(vector<CWord> vecWords) {
	map<char, int> mUsedLetters;

	for (vector<CWord>::iterator it = vecWords.begin(); it != vecWords.end(); ++it)
	{
		string strWord = it->m_strWordValue;
		for (size_t i = 0; i < strWord.size(); i++)
		{
			if (mUsedLetters.count(strWord[i]) > 0)
			{
				mUsedLetters[strWord[i]]++;
			}
			else
			{
				mUsedLetters[strWord[i]] = 1;
			}
		}
	}
	return mUsedLetters;
}

map<char, int> CScrabble::CombineUsedLetters(map<char, int> mInHand, map<char, int> mFromWords) {
	for (map<char, int>::iterator it = mFromWords.begin(); it != mFromWords.end(); ++it)
	{
		char cLetter = it->first;
		int iLetterCount = it->second;
		if (mInHand.count(it->first) > 0)
		{
			mInHand[cLetter] += it->second;
		}
		else
		{
			mInHand[cLetter] = iLetterCount;
		}
	}

	return mInHand;
}

vector<CWord> CScrabble::GetWordsThatCanBePlaced(vector<CWord>vecPlacedWords, vector<string> vecPossibleWords, bool bFirstTurn) {
	vector<CWord> vecWordsThatCanBePlaced;
	if (bFirstTurn){
		for (string strPossibleWords : vecPossibleWords){
			CWord wrdConsideredToPlace;
			wrdConsideredToPlace.m_iXCoordinate = 7;
			wrdConsideredToPlace.m_iYCoordinate = 7;
			wrdConsideredToPlace.m_strWordValue = strPossibleWords;
			wrdConsideredToPlace.m_Direction = LeftToRight;
			vecWordsThatCanBePlaced.push_back(wrdConsideredToPlace);
			//so both directions exist
			wrdConsideredToPlace.m_Direction = UpToDown;
			vecWordsThatCanBePlaced.push_back(wrdConsideredToPlace);
		}

	}
	else{
		for (CWord wrdAlreadyPlacedWord : vecPlacedWords){
			for (string strPossibleWord : vecPossibleWords){
				for (size_t i = 0; i < wrdAlreadyPlacedWord.m_strWordValue.size(); i++){
					for (size_t j = 0; j < strPossibleWord.size(); j++){
						if (wrdAlreadyPlacedWord.m_strWordValue[i] == strPossibleWord[j])
						{
							CWord wrdConsideredToPlace;
							wrdConsideredToPlace.m_strWordValue = strPossibleWord;

							wrdConsideredToPlace.m_Direction = wrdAlreadyPlacedWord.m_Direction == LeftToRight ? UpToDown : LeftToRight;
							wrdConsideredToPlace.m_iXCoordinate = wrdAlreadyPlacedWord.m_iXCoordinate;
							wrdConsideredToPlace.m_iYCoordinate = wrdAlreadyPlacedWord.m_iYCoordinate;

							if (wrdConsideredToPlace.m_Direction == LeftToRight) {
								wrdConsideredToPlace.m_iYCoordinate += i;
								wrdConsideredToPlace.m_iXCoordinate -= j;
							}

							else {
								wrdConsideredToPlace.m_iYCoordinate -= j;
								wrdConsideredToPlace.m_iXCoordinate += i;
							}

							if (wrdConsideredToPlace.m_iYCoordinate < 0 || wrdConsideredToPlace.m_iYCoordinate> 14 || wrdConsideredToPlace.m_iXCoordinate < 0 || wrdConsideredToPlace.m_iXCoordinate> 14) {
								continue;
							}

							wrdConsideredToPlace = this->GetExtendedWord(wrdConsideredToPlace);
							if (this->m_BoardMechanics.HasSameWordAlredyBeenPlaced(wrdConsideredToPlace) || !(this->m_WordMechanics.IsLegal(wrdConsideredToPlace.m_strWordValue))) {
								continue;
							}

							vecWordsThatCanBePlaced.push_back(wrdConsideredToPlace);
						}
					}
				}
			}
		}
	}

	return vecWordsThatCanBePlaced;
}

vector<tuple<CWord, vector<CWord>>> CScrabble::FilterLegalyPlacableWords(vector<CWord> vecIntersectingWords) {
	vector<tuple<CWord, vector<CWord>>> vecLegalPlacableWords;

	for (vector<CWord>::iterator it = vecIntersectingWords.begin(); it!= vecIntersectingWords.end(); ++it)
	{
		map<char, int> mNeededLetters = this->m_BoardMechanics.GetNeededCharaters(*it);
		bool bHasEnoughLeters = this->m_AIPlayer.HasLettersForWord(mNeededLetters);
		bool bCanWordBePlaced = this->m_BoardMechanics.CanWordBePlaced(*it);

		if (bHasEnoughLeters && bCanWordBePlaced)
		{
			tuple<bool, vector<CWord>> tNearWords = this->CheckLegalityOfNewlyCreatedWords(*it);
			bool bLegalNearWords = std::get<bool>(tNearWords);
			if (bLegalNearWords)
			{
				vecLegalPlacableWords.push_back({ *it, std::get<vector<CWord>>(tNearWords) });
			}
		}
	}
	return vecLegalPlacableWords;
}

void CScrabble::ReduceLetterFromAI(map<char, int> mLetters) {
	for (map<char, int>::iterator it = mLetters.begin(); it != mLetters.end(); ++it)
	{
		int iLetterCount = it->second;

		for (int i = 0; i < iLetterCount; i++)
		{
			this->m_AIPlayer.ReduceLetter(it->first);
		}
	}
}

void CScrabble::RefilAIHand() {
	int iDifference = ( this->m_kiLettersInHand - this->m_AIPlayer.CountLetters());

	for (int i = 0; i < iDifference; i++)
	{
		if (this->m_LetterBag.CheckIfNotEmpty())
		{
			this->m_AIPlayer.AddLetter(this->m_LetterBag.DrawALetter());
		}
	}
}