#include "Node.h"

Node::Node() {
        book_code=-1;
            page=-1;
            paragraph=-1;
            sentence_no =-1;
            offset=-1;
            left=NULL;
            right=NULL;
}
    Node::~Node() {
    // Node* current = this;
    // while (current != nullptr) {
    //     Node* temp = current;
    //     current = current->right;
    //     delete temp;
    // }
    right = nullptr;
    left = nullptr;
}

Node::Node(int b_code, int pg, int para, int s_no, int off){
    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    offset = off;
}