#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include <vector>
// #include "hash.cpp"
// #include "hash1.cpp"
using namespace std;

QNA_tool::QNA_tool()
{
    // Implement your function here
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
}
bool ispresentt(char c, string s = " .,-:!\"\'()?—[]“”‘’˙;@")
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
void QNA_tool::find(string s, HashMap &a)
{
    int index = d.hash(s);
    for (int i = 0; i < d.tabl[index].size(); i++)
    {
        if (d.tabl[index][i].st == s)
        {
            int para_no = d.tabl[index][i].para_no;
            int page_no = d.tabl[index][i].pg_no;
            int book_no = d.tabl[index][i].book_no;
            a.insert(para_no, page_no, book_no, 0);
        }
    }
}
string tolowercase(string s)
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
void extract_csv(Trie &t)
{
    std::ifstream file("unigram_freq.csv");
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    std::string header;
    if (getline(file, header))
    {
    }

    // std::cout << "Let's go" << std::endl;
    std::string line;
    long long l=1;
    while (getline(file, line))
    {
        std::istringstream iss(line);
        std::string word, counts;
        // cout<<"yess";
        if (getline(iss, word, ',') && getline(iss, counts))
        {
            try
            {
                long long count = std::stoll(counts);
                t.insert(word, count,l);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error parsing count: " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Count is out of range: " << e.what() << std::endl;
            }
            l++;
        }
    }
}

long long extract_count(string s, Trie m)
{
    return m.get_value(s);
}
float get_const(string s, Trie m) {
    long long line = m.get_line(s);
    float ct = 0;  // Declare ct outside the if-else ladder

    if (line < 26) {
        ct = 26 * 0.01;
    } else if (line >= 26 && line < 63) {
        ct = 2.6;
    } else if (line >= 63 && line < 10500) {
        ct = 26;
    } else if (line >= 10500 && line < 33200) {
        ct = 130;
    } else if (line >= 33200 && line < 100000) {
        ct = 1300;
    } else if (line >= 100000) {
        ct = 3250;
    }

    return ct;  
}
void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    d.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    Binaryheap b;
    Trie wordcounts;
    // cout<<"yes";
    extract_csv(wordcounts);
    // cout<<"here";
    HashMap h;
    for (int i = 0; i < question.size(); i++)
    {
        if (ispresentt(question[i]))
        {
            i++;
            if (i >= question.size())
            {
                continue;
            }
        }
        int j = i;
        while ((!ispresentt(question[j])) && j < question.size())
        {
            j++;
        }
        string y = question.substr(i, (j - i));
        i = j;
        y = tolowercase(y);
        // cout<<y<<endl;
        int n = 0;
        float cfreq = d.get_word_count(y);
        // cout<<cfreq<<endl;
        float gfreq = extract_count(y, wordcounts);
        // cout<<gfreq<<endl;
        // float factor=get_const(y,wordcounts);
        float wrd_score = (1 + cfreq) / (1 + gfreq);
        // cout<<wrd_score<<endl;
        find(y, h);
        for (int u = 0; u < h.table.size(); u++)
        {
            if (!h.table[u].empty())
            {
                for (int v = 0; v < h.table[u].size(); v++)
                {
                    // cout<<" start ";
                    int occur = h.table[u][v].occ;
                    float para_score = wrd_score * occur;
                    h.table[u][v].score += para_score;
                    // cout<<para_score<<"  "<<h.table[u][v].para_no<<"  "<<h.table[u][v].book_no<<"  "<<h.table[u][v].pg_no<<endl<<endl;
                    h.table[u][v].occ = 0;
                    // h.table[i][j].occ=0;
                    // if(b.heap.size()<k)
                    //     b.insertt(h.table[i][j].para_no,h.table[i][j].pg_no,h.table[i][j].book_no,h.table[i][j].score);
                    // else if(para_score>b.get_min().score)
                    // {
                    //     b.insertt(h.table[i][j].para_no,h.table[i][j].pg_no,h.table[i][j].book_no,h.table[i][j].score);
                    // }
                }
            }
        }
    }
    for (int i = 0; i < h.table.size(); i++)
    {
        if (!h.table.empty())
        {
            for (int j = 0; j < h.table[i].size(); j++)
            {
                float para_score = h.table[i][j].score;
                if (b.heap.size() < k)
                    b.insertt(h.table[i][j].para_no, h.table[i][j].pg_no, h.table[i][j].book_no, h.table[i][j].score);
                else if (para_score > b.get_min().score)
                {
                    b.deleteMin();
                    b.insertt(h.table[i][j].para_no, h.table[i][j].pg_no, h.table[i][j].book_no, h.table[i][j].score);
                }
            }
        }
    }
    Node *topKList = nullptr;
    Node *tail = nullptr;
    while (!b.heap.empty())
    {
        int para_no = b.get_min().para_no;
        int bk_no = b.get_min().bk_no;
        int pg_no = b.get_min().pg_no;
        float score = b.get_min().score;
        // cout << score << endl;
        b.deleteMin();

        // Insert at the beginning for decreasing order
        Node *newNode = new Node(bk_no, pg_no, para_no, 0, 0);
        newNode->right = topKList;
        topKList = newNode;
    }

    return topKList;

    // Implement your function here
}

void QNA_tool::query(string question, string filename)
{
    Binaryheap b;
    Trie wordcounts;
    // cout<<"yes";
    extract_csv(wordcounts);
    // cout<<"here";
    HashMap h;
    float max_ws = 0;
    for (int i = 0; i < question.size(); i++)
    {
        if (ispresentt(question[i]))
        {
            i++;
            if (i >= question.size())
            {
                continue;
            }
        }
        int j = i;
        while ((!ispresentt(question[j])) && j < question.size())
        {
            j++;
        }
        string y = question.substr(i, (j - i));
        i = j;
        y = tolowercase(y);
        // cout<<y<<endl;
        int n = 0;
        float cfreq = d.get_word_count(y);
        // cout<<cfreq<<endl;
        float gfreq = extract_count(y, wordcounts);
        // cout<<gfreq<<endl;
        float factor = get_const(y, wordcounts);
        float wrd_score = ((1 + cfreq) / (1 + gfreq)) * factor;
        if (max_ws < wrd_score)
        {
            max_ws = wrd_score;
        }
        // cout<<wrd_score<<endl;
        find(y, h);
        for (int u = 0; u < h.table.size(); u++)
        {
            if (!h.table[u].empty())
            {
                for (int v = 0; v < h.table[u].size(); v++)
                {
                    // cout<<" start ";
                    int occur = h.table[u][v].occ;
                    float para_score = ((wrd_score * occur) / max_ws);
                    h.table[u][v].score += para_score;
                    // cout<<para_score<<"  "<<h.table[u][v].para_no<<"  "<<h.table[u][v].book_no<<"  "<<h.table[u][v].pg_no<<endl<<endl;
                    h.table[u][v].occ = 0;
                    // h.table[i][j].occ=0;
                    // if(b.heap.size()<k)
                    //     b.insertt(h.table[i][j].para_no,h.table[i][j].pg_no,h.table[i][j].book_no,h.table[i][j].score);
                    // else if(para_score>b.get_min().score)
                    // {
                    //     b.insertt(h.table[i][j].para_no,h.table[i][j].pg_no,h.table[i][j].book_no,h.table[i][j].score);
                    // }
                }
            }
        }
    }
    for (int i = 0; i < h.table.size(); i++)
    {
        if (!h.table.empty())
        {
            for (int j = 0; j < h.table[i].size(); j++)
            {
                float para_score = h.table[i][j].score;
                if (b.heap.size() < 7) // taking optimal k=7;
                    b.insertt(h.table[i][j].para_no, h.table[i][j].pg_no, h.table[i][j].book_no, h.table[i][j].score);
                else if (para_score > b.get_min().score)
                {
                    b.deleteMin();
                    b.insertt(h.table[i][j].para_no, h.table[i][j].pg_no, h.table[i][j].book_no, h.table[i][j].score);
                }
            }
        }
    }
    Node *topKList = nullptr;
    Node *tail = nullptr;
    while (!b.heap.empty())
    {
        int para_no = b.get_min().para_no;
        int bk_no = b.get_min().bk_no;
        int pg_no = b.get_min().pg_no;
        float score = b.get_min().score;
        // cout << score << endl;
        b.deleteMin();

        // Insert at the beginning for decreasing order
        Node *newNode = new Node(bk_no, pg_no, para_no, 0, 0);
        newNode->right = topKList;
        topKList = newNode;
    }
    string s = " ";
    string API_KEY = "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";
    string prop = "Generate three different responses and provide the most coherent one and Set the temperature to 0.5 to provide more focused response for the question -  ";
    string q_final = prop + question;
    query_llm(s, topKList, 7, API_KEY, q_final);
    // Implement your function here
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: "
              << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // headorary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }
    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}
