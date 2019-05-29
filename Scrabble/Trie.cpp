#include "Trie.h"
using namespace std;
// #include <iostream>

TrieSet::TrieSet(){
  root = new TrieNode;
  root->inSet = false;    //FIX ME
  root->parent = nullptr;
  for(int i = 0; i < 26; i++)
    root->children[i] = nullptr;
}


TrieSet::~TrieSet(){
  to_free_memory(root);
}

void TrieSet::to_free_memory(TrieNode* node){
  for(int i = 0; i < 26; i++){
    if(node->children[i] != nullptr)
      to_free_memory(node->children[i]);
  }
  delete node;
}

int TrieSet::to_index(char ch) const{
  return (ch-'a');
}


/* Add this string to the set.
   Do nothing if the string is already in the set. */
void TrieSet::insert(string input){ 
  TrieNode* temp = root;
  transform(input.begin(), input.end(), input.begin(), ::tolower);
  for(size_t i = 0; i < input.size(); i++){
    TrieNode* parent = temp;
    temp = temp->children[to_index(input[i])];
    //if the current letter of the input is not created yet
    if(temp == nullptr){
      temp = new TrieNode;  //debugging
      temp->parent = parent;
      parent->children[to_index(input[i])] = temp;
      temp->inSet = false;
      for(int j = 0; j < 26; j++)
        temp->children[j] = nullptr;
    }
  }

  temp->inSet = true;

}

bool TrieSet::have_children(TrieNode* node){
  for(int i = 0; i < 26; i++){
    if(node->children[i] != nullptr)
      return true;
  }
  return false;
}

/* Removes this string from the set.
   Do nothing if the string is not already in the set. */
void TrieSet::remove(string input){ 
  transform(input.begin(), input.end(), input.begin(), ::tolower);
  TrieNode* temp = root;
  for(size_t i = 0; i < input.size(); i++){
    temp = temp->children[to_index(input[i])];
    //we dont need to do anything if the word is not in the set
    if(temp == nullptr)
      return;
  }

  //change to internal
  temp->inSet = false;
  int to_decrease = 0;
  int pos = to_index(input[input.size()-1]);
  while(!have_children(temp) && !temp->inSet && temp != root){
    // cout << "here" << endl;
    TrieNode* to_delete = temp;
    temp = temp->parent;
    temp->children[pos] = nullptr;
    to_decrease++;
    pos = to_index(input[input.size()-1-to_decrease]);
    delete to_delete;
  
  }
  
}

 /* returns the TrieNode matching the provided string, or returns NULL 
    if it is not the prefix of a word in the Trie. */
TrieNode* TrieSet::prefix(string px) const{
  transform(px.begin(), px.end(), px.begin(), ::tolower);
  TrieNode* temp = root;
  for(size_t i = 0; i < px.size(); i++){
    if(temp == nullptr)
      return nullptr;
    temp = temp->children[to_index(px[i])];
    //we dont need to do anything if the word is not in the set
    // if(temp == nullptr)
    //   return nullptr;
  }
  //temp will point to the last letter of px, if px is truly a prefix in the TrieSet
  return temp;
}

bool TrieSet::find(string word) const{

  transform(word.begin(), word.end(), word.begin(), ::tolower);
  TrieNode* last_letter = prefix(word);
  //if the last_letter is in the set, it means that the whole input is really a word in the set
  if(last_letter == nullptr)
    return false;
  if(last_letter->inSet)
    return true;
  return false;
}


// void TrieSet::printTrie() const  // DEBUG
// {
//   cout << "===PRINTING===" << endl;
//   printHelper(root, "");
// }

// void TrieSet::printHelper(TrieNode* node, std::string prefix) const  // DEBUG
// {
//   if(node->inSet){
//     cout << prefix << endl;
//   }

//   for(int i = 0; i < 26; ++i){
//     if(node->children[i] != nullptr){
//       string word = prefix + (char)('a'+i);
//       printHelper(node->children[i], word);
//     }
//   }
// }