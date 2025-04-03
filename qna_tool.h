#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"
const int ALPHABET_SIZE = 26;
using namespace std;
class TrieNode {
public:
    TrieNode* children[ALPHABET_SIZE];
    long long value;
    long long line;

    TrieNode() : value(0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {}

    void insert(const std::string& key, long long value,long long line) {
        TrieNode* current = root;
        for (char ch : key) {
            int index = ch - 'a'; // Assuming only lowercase English letters
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->value = value;
        current->line=line;
    }

    long long get_value(const std::string& key) const {
        TrieNode* current = root;
        for (char ch : key) {
            int index = ch - 'a';
            if (!current->children[index]) {
                return 0; // Key not found
            }
            current = current->children[index];
        }
        return current->value;
    }
    long long get_line(const std::string& key) const{
      TrieNode* current = root;
        for (char ch : key) {
            int index = ch - 'a';
            if (!current->children[index]) {
                return 0; // Key not found
            }
            current = current->children[index];
        }
        return current->line;
    }
};
struct HeapNode{
  int para_no;
  int pg_no;
  int bk_no;
  float score;
  HeapNode(int para_no,int pg_no,int bk_no,float score)
  :para_no(para_no),pg_no(pg_no),bk_no(bk_no),score(score){}
};
class Binaryheap
{
    // private:
    public:
    vector<HeapNode> heap;
    void swap(HeapNode &a,HeapNode &b)
    {
        HeapNode temp=a;
        // temp=a;
        a=b;
        b=temp;
    }
    int getParentIndex(int i) {
    return (i - 1) / 2;
  }
  
  int getLeftChildIndex(int i) {
    return 2 * i + 1;
  }
  
  int getRightChildIndex(int i) {
    return 2 * i + 2;
  }
  void insertt(int para_no,int pg_no,int bk_no,float score)
  {
    heap.push_back(HeapNode(para_no,pg_no,bk_no,score));
    heapifyUp(heap.size()-1);
  }
  void heapifyUp(int index) {
    if (index == 0) return; // base condition for termination of a recursive invocation of the fnc
    
    int parentIndex = getParentIndex(index);
    
    if (heap[parentIndex].score > heap[index].score) {
      swap(heap[parentIndex], heap[index]);
      heapifyUp(parentIndex);
    }
  }
  void heapifyDown(int index) {
    int leftChild = getLeftChildIndex(index);
    int rightChild = getRightChildIndex(index);
    
    if (leftChild >= heap.size()) return; // No children
    
    int minIndex = index;
    
    if (heap[minIndex].score > heap[leftChild].score) {
      minIndex = leftChild;
    }
    
    if (rightChild < heap.size() && heap[minIndex].score > heap[rightChild].score) {
      minIndex = rightChild;
    }
    
    if (minIndex != index) {
      swap(heap[minIndex], heap[index]);
      heapifyDown(minIndex);
    }
  }
  void heapifyDown(int n, int index) {
    int leftChild = getLeftChildIndex(index);
    int rightChild = getRightChildIndex(index);
    
    int minIndex = index;
    
    if (leftChild < n && heap[minIndex].score > heap[leftChild].score) {
      minIndex = leftChild;
    }
    
    if (rightChild < n && heap[minIndex].score > heap[rightChild].score) {
      minIndex = rightChild;
    }
    
    if (minIndex != index) {
      swap(heap[minIndex], heap[index]);
      heapifyDown(n, minIndex);
    }
  }
  void deleteMin() {
    if (heap.empty()) {
      std::cout << "Heap is empty!" << std::endl;
      return;
    }
    
    heap[0] = heap.back();
    heap.pop_back();
    
    heapifyDown(0);
  }
  HeapNode get_min()
  {
    return heap[0];
  }
};
struct KeyValuePair {
    int para_no=-1;
    int pg_no=-1;
    int book_no=-1;
    float score=-1;
    int occ=-1;
    KeyValuePair(int para, int pg, int book, float scr, int occurrence)
        : para_no(para), pg_no(pg), book_no(book), score(scr), occ(occurrence) {}
};

// Define the hash map
class HashMap {
private:
    static const int tableSize = 100;  
    int hash(int key) {
        return key%100;
    }
public:
    HashMap() {
        table.resize(tableSize);
    }
        std::vector<std::vector<KeyValuePair>> table;
    void insert(int para_no,int pg_no,int book_no,float score) {
        int index = hash(para_no);
        for(auto& pair: table[index]){
            if(pair.pg_no==pg_no && pair.book_no==book_no && pair.para_no==para_no){
                pair.score=pair.score+score;
                pair.occ+=1;
                return;
            }
        }
        table[index].push_back(KeyValuePair(para_no,pg_no,book_no,score,1));
    }
    int get_score(int para_no,int pg_no,int book_no) {
        int index = hash(para_no);
        for (const auto& pair :table[index]) {
            if (pair.pg_no == pg_no && pair.book_no==book_no && pair.para_no==para_no) {
                return pair.score;
            }
        }
        return -1;  
    }

};
class QNA_tool {

    
    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user
    
    // SearchEngine s;
    // You can add attributes/helper functions here
public:
    Dict d;
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor
    void find(string s, HashMap &a);
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};