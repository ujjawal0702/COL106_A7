// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;
class SearchEngine {
private:
    // You can add attributes/helper functions here
    string tolowercase(string s);
    static const int d=256;
    static const int q=101;
    struct SentenceInfo{
        int BookCode;
        int PageNumber;
        int Paragraph;
        int SentenceNumber;
        std::string Sentence;
        SentenceInfo(int bookCode,int page,int paragraph,int sentence_no, const std::string &s):
        BookCode(bookCode),PageNumber(page),Paragraph(paragraph),SentenceNumber(sentence_no),Sentence(s){}
    };
    // Node* head;
    std::vector<SentenceInfo> sentences;
public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};