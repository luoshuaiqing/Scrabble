/*
 * Dictionary.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The class Dictionary encodes the list of all legal words. */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

// #include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include "Trie.h"

class Dictionary 
{

public:

	/* The constructor gets passed the name of the file from which
	   to read the word list. */
	Dictionary (std::string dictionary_file_name);

	~Dictionary ();

	/* Checks whether the given word is in the dictionary, and returns true if so.
	   Case should not matter. Must run in O(logn) for the number of words in the dictionary. */
	bool isLegalWord (std::string const & word) const;

	
	//Add more public and private functions/variables here.
	bool is_prefix(std::string word);

private:
	void add_words_to_set(std::ifstream& _Dictionary);
	TrieSet my_dictionary;
};


#endif /* DICTIONARY_H_ */
