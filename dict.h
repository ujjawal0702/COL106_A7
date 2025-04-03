#include <string>
#include <vector>
#include <iostream>
#include <fstream>
// #include "hash.cpp"
// #include "hash2.cpp" // Assuming you still need this

using namespace std;

// Define a structure to represent key-value pairs
struct KeyValue
{
    int para_no = -1;
    int pg_no = -1;
    int book_no = -1;
    string st = "";
     KeyValue(int para = -1, int pg = -1, int book = -1, const string& s = "")
        : para_no(para), pg_no(pg), book_no(book), st(s) {}
};

    // public:

class Dict
{
public:
    // AVLTree tree;
    // HashMp pm;

     static const int tablSize = 100007;
    int hash(string key)
    {
        int hash = 0;
        for (int i = 0; i < key.size(); i++)
        {
            hash = (17 * hash + key[i]) % tablSize;
        }
        return hash;
    }
    
    string tolowercase(string s);

    vector<vector<KeyValue>> tabl;
    
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);
};