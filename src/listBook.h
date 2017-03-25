#ifndef LAB1V2_LISTBOOK_H
#define LAB1V2_LISTBOOK_H

#include "contact.h"

typedef struct LNode LNode;
typedef struct LBook LBook;

struct LNode {
    Contact *val;
    LNode *next;
    LNode *prev;
};

struct LBook {
    LNode *sentinel;
    ContactKey currentKey;
};

LBook *createBook_L(ContactKey key);

void printBook_L(LBook *contactBook);

int sizeOfBook_L(LBook *contactBook);

void deleteBook_L(LBook *contactBook);

LBook *sortBookByKey_L(LBook *contactBook, ContactKey key);

void insertIntoBook_L(LBook *contactBook, Contact *contact);

LNode *findNodeByKey_L(LBook *contactBook, char *string, ContactKey key);

Contact *removeNodeFromBook_L(LBook *contactBook, LNode *contactNode);

#endif //LAB1V2_LISTBOOK_H
