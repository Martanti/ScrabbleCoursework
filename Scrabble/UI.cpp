#include "Headers/UI.h"

/*
 *	Public
 */

void CUserInterface::DisplayBoard(char *pcBoard, char *pcMultipliers, int iSize) {

	int iRowLength = (int) sqrt(iSize);

	//display x coordinates
	std::cout << "    ";
	for (int i = 97; i < iRowLength+97; i++) //using ASCII
	{
		std::cout<< ((char) i)<< "  ";
	}
	int iRowIndex = -1;

	for (int i = 0; i < iSize; i++)
	{
		//display coordinates
		int iCurrentRow = i / iRowLength;

		if (iRowIndex<iCurrentRow){
			iRowIndex = i / iRowLength;
			if (iRowIndex +1< 10){
				std::cout << "\n  " << iRowIndex+1;
			}
			else{
				std::cout << "\n " << iRowIndex+1;
			}
			
		}

		if (pcBoard[i] != '0'){
			std::cout << "[" << pcBoard[i] << "]";
		}
		else{
			std::cout << "[" << pcMultipliers[i] << "]";
		}
	}

	std::cout << "\n";
}

string CUserInterface::GetWordFromUser() {

	std::cout << "Enter your word:\n";

	string strWord;
	std::cin.clear();
	std::cin>>strWord;
	return strWord;
}

tuple<int, int> CUserInterface::GetPlacedCoordinates(bool& bWasSuccessful) {
	std::cout << "Please, write coordinate for X axis (a-o)\n";
	char cInputtedXPos;
	std::cin.clear();
	std::cin >> cInputtedXPos;
	
	cInputtedXPos = std::tolower(cInputtedXPos);

	if (cInputtedXPos < 'a' || cInputtedXPos>'o') {
		bWasSuccessful = false;
		return { 0,0 }; 
	}

	int iXPos = ((int)cInputtedXPos) - 97;

	std::cout << "Please, write coordinate for Y axis (1-15)\n";

	int iYPos;
	std::cin.clear();
	std::cin >> iYPos;
	std::cin.clear();
	
	iYPos--; //to make it zero based

	if (iYPos < 0 || iYPos>14) { 
		bWasSuccessful = false;
		return {0,0};
	}

	bWasSuccessful = true;
	return {iXPos, iYPos};

}

char CUserInterface::GetDirectionLetter(bool& bWasSuccessful) {
	std::cout << "Enter a direction. 'l' for from left to right. 'd' for from up to down\n";

	char cInputtedDirection;
	std::cin.clear();
	std::cin >> cInputtedDirection;
	
	cInputtedDirection = std::tolower(cInputtedDirection);

	if (cInputtedDirection != 'l' && cInputtedDirection != 'd') {
		bWasSuccessful = false;
		return 0;
	}

	bWasSuccessful = true;
	return cInputtedDirection;
}

void CUserInterface::DisplayHeldLetters(map<char, int> mHeldLetters) {
	std::cout << "Letters currently held: \n";
	for (map<char, int>::iterator it = mHeldLetters.begin(); it != mHeldLetters.end(); ++it)
	{
		std::cout << (char) toupper(it->first) << " - " << it->second << " | ";
	}
	std::cout << "\n";
}

void CUserInterface::DisplayScore(int iScore) {
	std::cout << "Your score is: " << iScore<<"\n";
}

void CUserInterface::DisplayAIScore(int iScore) {
	std::cout << "Enemy score is: " << iScore << "\n";
}

void CUserInterface::DisplayMessage(string strMessage) {
	std::cout << strMessage;
}
/*
 *	Private
 */