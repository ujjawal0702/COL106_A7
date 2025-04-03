// Do NOT add any other includes
#include "search.h"
string SearchEngine::tolowercase(string s)
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
SearchEngine::SearchEngine()
{
    // Implement your function here
}

SearchEngine::~SearchEngine()
{
    // Implement your function here
    // Node* current = head;
    // while (current != nullptr) {
    //     Node* temp = current;
    //     current = current->right;
    //     delete temp;
    // }
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    sentences.push_back(SentenceInfo(book_code, page, paragraph, sentence_no, tolowercase(sentence)));
    return;
}

Node *SearchEngine::search(string pattern, int &n_matches)
{
    // Implement your function here
    Node *tail = nullptr;
    Node *head = nullptr;
    n_matches = 0;
    std::string patternLower = tolowercase(pattern);
    int m = patternLower.length();
    int p = 0;
    int t = 0;
    int h = 1;
    for (int i = 0; i < m - 1; i++)
        h = (h * d) % q;
    for (int i = 0; i < m; i++)
    {
        p = (d * p + patternLower[i]) % q;
    }
    for (const auto &info : sentences)
    {
        if (info.Sentence.length() < m)
        {
            continue;
        }
        t = 0;
        for (int i = 0; i < m; i++)
            t = (d * t + info.Sentence[i]) % q;
        for (int i = 0; i <= info.Sentence.length() - m; i++)
        {
            if (p == t)
            {
                bool flag = true;
                for (int j = 0; j < m; j++)
                    if (info.Sentence[i + j] != patternLower[j])
                    {
                        flag = false;
                        break;
                    }
                if (flag)
                {
                    Node *newNode = new Node(info.BookCode, info.PageNumber, info.Paragraph, info.SentenceNumber, i);
                    if (!head)
                    {
                        head = newNode;
                        tail = head;
                    }
                    else
                    {
                        newNode->left = tail;
                        tail->right = newNode;
                        tail = newNode;
                    }
                    n_matches++;
                }
            }
            if (i < info.Sentence.length() - m)
            {
                t = (d * (t - info.Sentence[i] * h) + info.Sentence[i + m]) % q;
                if (t < 0)
                    t = (t + q);
            }
        }
    }
    // Node* temp;
    // temp=head;
    // while(temp!=NULL)
    //     {   cout<<"yes";
    //     // cout<<head->page;
    //         int para=temp->page;
    //         // cout<<"enter1";
    //         int pg=temp->paragraph;
    //         // cout<<"enter2";
    //         int bk=temp->book_code;
    //         // cout<<"enter3";
    //         // cout<<"enter4";
    //         temp=temp->right;
    //     }
    cout << "EXITTT";
    cout << head->paragraph << head->book_code;
    return head;
}
