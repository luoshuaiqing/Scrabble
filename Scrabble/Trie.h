#include <string>
#include <algorithm>

struct TrieNode {
  bool inSet;   // Is this node the end of a string in your dictionary?
  TrieNode *parent;
  TrieNode *children[26]; // one child for each letter of the alphabet.
  // int child_index;
};

class TrieSet {
public:

  TrieSet (); // a constructor for an empty trie
  ~TrieSet (); // destructor

  void insert (std::string input);
   /* Add this string to the set.
      Do nothing if the string is already in the set. */

  void remove (std::string input);
   /* Removes this string from the set.
      Do nothing if the string is not already in the set. */

  TrieNode* prefix(std::string px) const; 
   /* returns the TrieNode matching the provided string, or returns NULL 
      if it is not the prefix of a word in the Trie. */

  //find a string in the TrieSet
  bool find(std::string word) const;

  int to_index(char ch) const;
private:

  TrieNode* root;
  bool have_children(TrieNode* node);
  void to_free_memory(TrieNode* node);
  // void printTrie() const;
  // void printHelper(TrieNode* node, std::string prefix) const;


};