#include "Dictionary.h"
using namespace std;


void Dictionary::add_words_to_set(ifstream& _Dictionary)
{
	string myline;
	while(getline(_Dictionary,myline)){
		transform(myline.begin(), myline.end(), myline.begin(), ::tolower);		//add all the lower_case version of the words in the dictionary to my set
		my_dictionary.insert(myline);
	}
}


Dictionary::Dictionary (string dictionary_file_name)
{
	ifstream _Dictionary(dictionary_file_name);
	add_words_to_set(_Dictionary);
}

Dictionary::~Dictionary ()
{

}

bool Dictionary::is_prefix(string word){
	if(my_dictionary.prefix(word) == nullptr){
		return false;
	}
	return true;
}


bool Dictionary::isLegalWord (string const & word) const
{
	string word_lower_case = word;
	transform(word_lower_case.begin(), word_lower_case.end(), word_lower_case.begin(), ::tolower);		//compare the lower_case version of the word with the words in the dictoinary, which are also lower_case

	//if found the word
	if(my_dictionary.find(word_lower_case)){
		return true;
	}
	return false;
}

