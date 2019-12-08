#include "headers/FileManager.h"

/*
 *	Public
 */

vector<string>* CFileManager::GetLegalWords() {

	if (!(this->DoesExternalDictionaryExist())){\
		this->CreateExternalDictionary();
	}

	vector<string>* vecLegalWords = new vector<string>;

	std::ifstream ifExternalDictionary;
	ifExternalDictionary.open(this->m_kstrExternalDictionaryName);

	string strWord;

	while (ifExternalDictionary>>strWord)
	{
		vecLegalWords->push_back(strWord);
	}

	ifExternalDictionary.close();
	return vecLegalWords;
}

/*
 *	Private
 */

const string CFileManager::m_kstrExternalDictionaryName = "legalwords.txt";

const string CFileManager::m_kstrBackDictionaryName = "words.txt";

void CFileManager::CreateExternalDictionary() {

	std::cout << "External dictionary not found. \nCreating one....\n";
	auto startTime = std::chrono::high_resolution_clock::now(); //start time counting

	std::unique_ptr< vector<string>> vecLegalWords = this->ReadBackupDictionary();

	CQuickSort qstSort;

	//vecLegalWords = qstSort.Quicksort(*vecLegalWords);
	qstSort.Quicksort(*vecLegalWords);

	std::cout << vecLegalWords->size() << " words found\n";

	this->WriteExternalDictionary(std::move(vecLegalWords));
	auto finishTime = std::chrono::high_resolution_clock::now(); //end time counting
	auto durationTime = std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count();
	std::cout << "External dictionary created in: " << durationTime << " miliseconds\n";
}

bool CFileManager::IsWordLegal(string strWord) {

	for (size_t i = 0; i < strWord.size(); i++){
		int iLetterCode = (int)strWord[i];

		if (iLetterCode > 122 || iLetterCode < 97){
			return false;
		}
	}

	return true;
}

bool CFileManager::DoesExternalDictionaryExist() {
	std::ifstream ifFile(this->m_kstrExternalDictionaryName);
	return ifFile.good(); //return state of the file
}

void CFileManager::WriteExternalDictionary(std::unique_ptr<vector<string>> vecLegalwords) {
	std::ofstream ofExternalDictionary;

	ofExternalDictionary.open(this->m_kstrExternalDictionaryName);

	for (vector<string>::iterator it = vecLegalwords->begin(); it != vecLegalwords->end(); ++it)
	{
		ofExternalDictionary << *it << "\n";
	}

	ofExternalDictionary.close();
}

std::unique_ptr< vector<string>> CFileManager::ReadBackupDictionary() {
	std::ifstream ifBackupDictionary;
	ifBackupDictionary.open(this->m_kstrBackDictionaryName);

	string strWord;
	std::unique_ptr< vector<string>> vecLegalWords(new vector<string>);

	while (ifBackupDictionary >> strWord){
		if (this->IsWordLegal(strWord)){
			vecLegalWords->push_back(strWord);
		}
		else continue;
	}

	ifBackupDictionary.close();
	return vecLegalWords;
}