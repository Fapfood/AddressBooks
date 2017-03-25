#ifndef LAB1V2_TREEBOOK_H
#define LAB1V2_TREEBOOK_H

#include "contact.h"

typedef struct TNode TNode;
typedef struct TBook TBook;

struct TNode {
    Contact *val;
    TNode *par;
    TNode *left;
    TNode *right;
};

struct TBook {
    TNode *root;
    ContactKey currentKey;
};

TBook *createBook_T(ContactKey key);

void printBook_T(TBook *contactBook);

int sizeOfBook_T(TBook *contactBook);

void deleteBook_T(TBook *contactBook);

TBook *sortBookByKey_T(TBook *contactBook, ContactKey key);

void insertIntoBook_T(TBook *contactBook, Contact *contact);

TNode *findNodeByKey_T(TBook *contactBook, char *string, ContactKey key);

Contact *removeNodeFromBook_T(TBook *contactBook, TNode *contactNode);

#endif //LAB1V2_TREEBOOK_H
