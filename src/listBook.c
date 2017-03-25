#include <stdlib.h>
#include "listBook.h"

LBook *createBook_L(ContactKey key) {
    LBook *book = malloc(sizeof(LBook));
    LNode *sentinel = malloc(sizeof(LNode));
    sentinel->val = NULL;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    book->sentinel = sentinel;
    book->currentKey = key;
    return book;
}

void printBook_L(LBook *contactBook) {
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        printContact(tmp->val);
        tmp = tmp->next;
    }
}

int sizeOfBook_L(LBook *contactBook) {
    int size = 0;
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        size++;
        tmp = tmp->next;
    }
    return size;
}

void deleteBook_L(LBook *contactBook) {
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        deleteContact(tmp->val);
        tmp->val = NULL;
        tmp = tmp->next;
        free(tmp->prev);
        tmp->prev = NULL;
    }
    free(contactBook->sentinel);
    free(contactBook);
}

LBook *mergeBooks(LBook *a, LBook *b) {
    if (a->sentinel->next == a->sentinel) {
        deleteBook_L(a);
        return b;
    }
    if (b->sentinel->next == b->sentinel) {
        deleteBook_L(b);
        return a;
    }
    a->sentinel->prev->next = b->sentinel->next;
    b->sentinel->next->prev = a->sentinel->prev;
    a->sentinel->prev = b->sentinel->prev;
    a->sentinel->prev->next = a->sentinel;
    b->sentinel->next = b->sentinel;
    b->sentinel->prev = b->sentinel;
    deleteBook_L(b);
    return a;
}

void addAtTheEndOfBook(LBook *contactBook, Contact *contact) {
    LNode *newNode = malloc(sizeof(LNode));
    newNode->val = contact;
    newNode->next = contactBook->sentinel;
    newNode->prev = contactBook->sentinel->prev;
    contactBook->sentinel->prev = newNode;
    contactBook->sentinel->prev->prev->next = newNode;
}

LBook *sortBookByKey_L(LBook *contactBook, ContactKey key) {
    contactBook->currentKey = key;
    if (contactBook->sentinel->next == contactBook->sentinel->prev) { //dwa przypadki, pusta i jednoelementowa
        return contactBook;
    }
    LBook *lessBook = createBook_L(key);
    LBook *equalBook = createBook_L(key);
    LBook *moreBook = createBook_L(key);
    Contact *pivot = contactBook->sentinel->next->val;

    Contact *contactTmp;
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        contactTmp = removeNodeFromBook_L(contactBook, tmp);
        if (compareContacts(contactTmp, pivot, key) == 0) {
            addAtTheEndOfBook(equalBook, contactTmp);
        } else if (compareContacts(contactTmp, pivot, key) > 0) {
            addAtTheEndOfBook(moreBook, contactTmp);
        } else {
            addAtTheEndOfBook(lessBook, contactTmp);
        }
        tmp = contactBook->sentinel->next;
    }

    LBook *bookTmp = mergeBooks(sortBookByKey_L(lessBook, key), mergeBooks(equalBook, sortBookByKey_L(moreBook, key)));
    tmp = contactBook->sentinel;
    contactBook->sentinel = bookTmp->sentinel;
    bookTmp->sentinel = tmp;
    deleteBook_L(bookTmp);
    return contactBook;
}

void insertIntoBook_L(LBook *contactBook, Contact *contact) {
    LNode *newNode = malloc(sizeof(LNode));
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->val = contact;
    LNode *tmp = contactBook->sentinel->next;
    if (tmp == contactBook->sentinel) {
        newNode->next = contactBook->sentinel;
        newNode->prev = contactBook->sentinel;
    } else {
        while (tmp != contactBook->sentinel && compareContacts(tmp->val, contact, contactBook->currentKey) <= 0)
            tmp = tmp->next;
        newNode->next = tmp;
        newNode->prev = tmp->prev;
    }
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
}

LNode *findNodeByKey_L(LBook *contactBook, char *string, ContactKey key) {
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        if (compareContactWithString(tmp->val, string, key) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

Contact *removeNodeFromBook_L(LBook *contactBook, LNode *contactNode) {
    LNode *tmp = contactBook->sentinel->next;
    while (tmp != contactBook->sentinel) {
        if (tmp == contactNode) {
            Contact *returnContact = contactNode->val;
            contactNode->val = NULL;
            contactNode->prev->next = contactNode->next;
            contactNode->next->prev = contactNode->prev;
            contactNode->next = NULL;
            contactNode->prev = NULL;
            free(contactNode);
            return returnContact;
        }
        tmp = tmp->next;
    }
    return NULL;
}