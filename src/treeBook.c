#include <stdlib.h>
#include "treeBook.h"

TBook *createBook_T(ContactKey key) {
    TBook *book = malloc(sizeof(TBook));
    book->root = NULL;
    book->currentKey = key;
    return book;
}

void printTree(TNode *root) {
    if (root == NULL)
        return;
    printTree(root->left);
    printContact(root->val);
    printTree(root->right);
}

void printBook_T(TBook *contactBook) {
    printTree(contactBook->root);
}

int sizeOfTree(TNode *root) {
    if (root == NULL)
        return 0;
    return sizeOfTree(root->left) + 1 + sizeOfTree(root->right);
}

int sizeOfBook_T(TBook *contactBook) {
    return sizeOfTree(contactBook->root);
}

void deleteTree(TNode *root) {
    if (root == NULL) {
        return;
    }
    deleteTree(root->left);
    root->left = NULL;
    deleteContact(root->val);
    root->val = NULL;
    deleteTree(root->right);
    root->right = NULL;
    free(root);
}

void deleteBook_T(TBook *contactBook) {
    deleteTree(contactBook->root);
    contactBook->root = NULL;
    free(contactBook);
}

TNode *treeMinimum(TNode *root) {
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

TNode *treeMaximum(TNode *root) {
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

TNode *treeSuccessor(TNode *root) {
    if (root == NULL)
        return NULL;
    if (root->right != NULL)
        return treeMinimum(root->right);
    TNode *tmp = root->par;
    while (tmp != NULL && root == tmp->right) {
        root = tmp;
        tmp = tmp->par;
    }
    return tmp;
}

TNode *treePredecessor(TNode *root) {
    if (root == NULL)
        return NULL;
    if (root->left != NULL)
        return treeMaximum(root->left);
    TNode *tmp = root->par;
    while (tmp != NULL && root == tmp->left) {
        root = tmp;
        tmp = tmp->par;
    }
    return tmp;
}

void sortTreeByKey(TNode *oldRoot, TBook *newBook) {
    if (oldRoot == NULL)
        return;
    sortTreeByKey(oldRoot->left, newBook);
    sortTreeByKey(oldRoot->right, newBook);
    insertIntoBook_T(newBook, oldRoot->val);
    free(oldRoot);
}

TBook *sortBookByKey_T(TBook *contactBook, ContactKey key) {
    contactBook->currentKey = key;
    TBook *book = createBook_T(key);
    sortTreeByKey(contactBook->root, book);
    contactBook->root = book->root;
    free(book);
    return contactBook;
}

void insertIntoBook_T(TBook *contactBook, Contact *contact) {
    TNode *newNode = malloc(sizeof(TNode));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->par = NULL;
    newNode->val = contact;

    TNode *root = contactBook->root;
    TNode *par = NULL;
    while (root != NULL) {
        par = root;
        if (compareContacts(contact, root->val, contactBook->currentKey) < 0)
            root = root->left;
        else
            root = root->right;
    }
    newNode->par = par;
    if (par == NULL) //puste drzewo
        contactBook->root = newNode;
    else if (compareContacts(contact, par->val, contactBook->currentKey) < 0)
        par->left = newNode;
    else
        par->right = newNode;
}

TNode *treeSearchByKey(TNode *root, char *string, ContactKey key) {
    if (root == NULL || (compareContactWithString(root->val, string, key) == 0))
        return root;
    if (compareContactWithString(root->val, string, key) > 0)
        return treeSearchByKey(root->left, string, key);
    else
        return treeSearchByKey(root->right, string, key);
}

TNode *findNodeByKey_T(TBook *contactBook, char *string, ContactKey key) {
    return treeSearchByKey(contactBook->root, string, key);
}

void transplant(TBook *contactBook, TNode *replaced, TNode *replacement) {
    if (replaced->par == NULL)
        contactBook->root = replacement;
    else if (replaced == replaced->par->left)
        replaced->par->left = replacement;
    else
        replaced->par->right = replacement;
    if (replacement != NULL)
        replacement->par = replaced->par;
}

Contact *removeNodeFromBook_T(TBook *contactBook, TNode *contactNode) {
    Contact *returnContact = contactNode->val;
    if (contactNode->left == NULL)
        transplant(contactBook, contactNode, contactNode->right);
    else if (contactNode->right == NULL)
        transplant(contactBook, contactNode, contactNode->left);
    else {
        TNode *tmp = treeMinimum(contactNode->right);
        if (tmp->par != contactNode) {
            transplant(contactBook, tmp, tmp->right);
            tmp->right = contactNode->right;
            tmp->right->par = tmp;
        }
        transplant(contactBook, contactNode, tmp);
        tmp->left = contactNode->left;
        tmp->left->par = tmp;
    }
    contactNode->val = NULL;
    contactNode->par = NULL;
    contactNode->left = NULL;
    contactNode->right = NULL;
    free(contactNode);
    return returnContact;
}