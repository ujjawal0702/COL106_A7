#include "dict.h"
Dict::Dict()
{
    tabl.resize(100007);
}
Dict::~Dict()
{
    // Implement your function here
}
string Dict::tolowercase(string s)
{
    string result = s;
    for (char &c : result)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c = c + 32;
        }
    }
    return result;
}
bool ispresent(char c, string s = " .,-:!\"\'()?—[]“”‘’˙;@")
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == c)
        {
            return true;
        }
    }
    return false;
}
int Dict::get_word_count(string word)
{
    int index = hash(word);
    int count = 0;
    for (int i = 0; i < tabl[index].size(); i++)
    {
        if (tabl[index][i].st == word)
        {
            count = count + 1;
        }
    }
    return count;
}
void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    for (int i = 0; i < sentence.size(); i++)
    {
        if (ispresent(sentence[i]))
        {
            i++;
            if (i >= sentence.size())
            {
                return;
            }
        }
        int j = i;
        while ((!ispresent(sentence[j])) && j < sentence.size())
        {
            j++;
        }
        string y = sentence.substr(i, (j - i));
        i = j;
        y = tolowercase(y);
        if (!y.empty())
        {
            int index = hash(y);
            KeyValue entry = {paragraph, page, book_code, y}; // Create an instance
            tabl[index].push_back(entry);
        }
    }
}

// int main() {
//     // Create a BTree
//     cout<<"entered main";
//     BTree bTree;
//     cout<<"xyz";
//     // Insert nodes into the tree
//     bTree.insert("apple", 1, 101, 1);
//     bTree.insert("banana", 2, 102, 2);
//     bTree.insert("orange", 3, 103, 3);
//     bTree.insert("grape", 4, 104, 4);
//     bTree.insert("grape", 9, 104, 4);

//     // Retrieve the root of the tree
//     BNode* root = bTree.getRoot();

//     // Display the information in the nodes (in-order traversal)
//     Dict dict;  // Assuming Dict has been properly initialized
//     dict.displayInOrder(root);
//     return 0;
// }

// Helper function for in-order traversal
