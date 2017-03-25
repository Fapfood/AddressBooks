#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/resource.h>
#include <time.h>


struct rusage startRusage;
struct rusage endRusage;
struct timespec startReal;
struct timespec endReal;

typedef struct Contact Contact;
typedef enum ContactKey ContactKey;

enum ContactKey {
    NAME,
    SURNAME,
    BIRTHDATE,
    EMAIL,
    PHONE,
    ADDRESS
};

struct Contact {
    char *name;
    char *surname;
    char *birthDate;
    char *email;
    char *phone;
    char *address;
};

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


static const char *DELIMITERS = ";";

LBook *(*createBook_L)(ContactKey);
void (*printBook_L)(LBook *);
int (*sizeOfBook_L)(LBook *);
void (*deleteBook_L)(LBook *);
LBook *(*sortBookByKey_L)(LBook *, ContactKey);
void (*insertIntoBook_L)(LBook *, Contact *);
LNode *(*findNodeByKey_L)(LBook *, char *, ContactKey);
Contact *(*removeNodeFromBook_L)(LBook *, LNode *);
TBook *(*createBook_T)(ContactKey);
void (*printBook_T)(TBook *);
int (*sizeOfBook_T)(TBook *);
void (*deleteBook_T)(TBook *);
TBook *(*sortBookByKey_T)(TBook *, ContactKey);
void (*insertIntoBook_T)(TBook *, Contact *);
TNode *(*findNodeByKey_T)(TBook *, char *, ContactKey);
Contact *(*removeNodeFromBook_T)(TBook *, TNode *);
Contact *(*createContact)(char *, char *, char *, char *, char *, char *);
void (*printContact)(Contact *);
void (*deleteContact)(Contact *);
char *(*getValueOfContact)(Contact *, ContactKey);
int (*compareContacts)(Contact *, Contact *, ContactKey);
int (*compareContactWithString)(Contact *, char *, ContactKey);

void loadLibraryFunctions() {
    void *libaddressbook = dlopen("lib/libaddressbook.so", RTLD_LAZY);

    createBook_L = dlsym(libaddressbook, "createBook_L");
    printBook_L = dlsym(libaddressbook, "printBook_L");
    sizeOfBook_L = dlsym(libaddressbook, "sizeOfBook_L");
    deleteBook_L = dlsym(libaddressbook, "deleteBook_L");
    sortBookByKey_L = dlsym(libaddressbook, "sortBookByKey_L");
    insertIntoBook_L = dlsym(libaddressbook, "insertIntoBook_L");
    findNodeByKey_L = dlsym(libaddressbook, "findNodeByKey_L");
    removeNodeFromBook_L = dlsym(libaddressbook, "removeNodeFromBook_L");
    createBook_T = dlsym(libaddressbook, "createBook_T");
    printBook_T = dlsym(libaddressbook, "printBook_T");
    sizeOfBook_T = dlsym(libaddressbook, "sizeOfBook_T");
    deleteBook_T = dlsym(libaddressbook, "deleteBook_T");
    sortBookByKey_T = dlsym(libaddressbook, "sortBookByKey_T");
    insertIntoBook_T = dlsym(libaddressbook, "insertIntoBook_T");
    findNodeByKey_T = dlsym(libaddressbook, "findNodeByKey_T");
    removeNodeFromBook_T = dlsym(libaddressbook, "removeNodeFromBook_T");
    createContact = dlsym(libaddressbook, "createContact");
    printContact = dlsym(libaddressbook, "printContac");
    deleteContact = dlsym(libaddressbook, "deleteContact");
    getValueOfContact = dlsym(libaddressbook, "getValueOfContact");
    compareContacts = dlsym(libaddressbook, "compareContacts");
    compareContactWithString = dlsym(libaddressbook, "compareContactWithString");
}

void resetTimer() {
    getrusage(RUSAGE_SELF, &startRusage);
    clock_gettime(CLOCK_MONOTONIC_RAW, &startReal);
}

void measureTime(char* description) {
    getrusage(RUSAGE_SELF, &endRusage);
    clock_gettime(CLOCK_MONOTONIC_RAW, &endReal);
    printf("\tUSER:\t %10ldus\t",     (long) ((endRusage.ru_utime.tv_sec - startRusage.ru_utime.tv_sec) * 1000000 + (endRusage.ru_utime.tv_usec - startRusage.ru_utime.tv_usec)));
    printf("\tSYSTEM:\t %10ldus\t",   (long) ((endRusage.ru_stime.tv_sec - startRusage.ru_stime.tv_sec) * 1000000 + (endRusage.ru_stime.tv_usec - startRusage.ru_stime.tv_usec)));
    printf("REAL:\t %10ldus  ",       (long) ((endReal.tv_sec - startReal.tv_sec) * 1000000000 + (endReal.tv_nsec - startReal.tv_nsec)) / 1000);
    printf("|--- %s\n", description);
}

static void loadContacts(Contact *contacts[1000]) {
    FILE *stream = fopen("data.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int i = 0;
    nread = getline(&line, &len, stream);
//line zawiera linię, len długośc bufora gdy line = null i len = 0 samemu alokuje bufor,
// gdy się zada jakiś i będize za mały, getline sama zrobi realloc i update tych wartości
    while (nread != -1) {
        //-1 gdy plik się skończy, bo nread zawiera liczbę przeczytanych
        char *name = strtok(line, DELIMITERS);
        char *surname = strtok(NULL, DELIMITERS);
        char *birthDate = strtok(NULL, DELIMITERS);
        char *email = strtok(NULL, DELIMITERS);
        char *phone = strtok(NULL, DELIMITERS);
        char *address = strtok(NULL, DELIMITERS);
        contacts[i] = createContact(name, surname, birthDate, email, phone, address);
        i++;
        nread = getline(&line, &len, stream);
    }
    free(line);
    fclose(stream);
}

void *bookOperations_T() {
    Contact *contacts[1000];
    loadContacts(contacts);

    resetTimer();
    TBook *contactBook_T = createBook_T(SURNAME);
    measureTime("create book");

    resetTimer();
    int i;
    for (i = 0; i < 1000; ++i) {
        insertIntoBook_T(contactBook_T, contacts[i]);
    }
    measureTime("insert 1000");

    resetTimer();
    //Webb jest pierwszy w drzewie
    findNodeByKey_T(contactBook_T, "Webb", SURNAME);
    measureTime("find opt");

    resetTimer();
    removeNodeFromBook_T(contactBook_T, findNodeByKey_T(contactBook_T, "Webb", SURNAME));
    measureTime("remove opt");

    resetTimer();
    sortBookByKey_T(contactBook_T, PHONE);
    measureTime("sort");

    sortBookByKey_T(contactBook_T, SURNAME);

    resetTimer();
    //Zimmerman jest ostatni w drzewie
    findNodeByKey_T(contactBook_T, "Zimmerman", SURNAME);
    measureTime("find pes");

    resetTimer();
    removeNodeFromBook_T(contactBook_T, findNodeByKey_T(contactBook_T, "Zimmerman", SURNAME));
    measureTime("remove pes");

    deleteBook_T(contactBook_T);
}

void *bookOperations_L() {
    Contact *contacts[1000];
    loadContacts(contacts);

    resetTimer();
    LBook *contactBook_L = createBook_L(SURNAME);
    measureTime("create book");

    resetTimer();
    int i;
    for (i = 0; i < 1000; ++i) {
        insertIntoBook_L(contactBook_L, contacts[i]);
    }
    measureTime("insert 1000");

    resetTimer();
    //Webb jest pierwszy w liście
    findNodeByKey_L(contactBook_L, "Webb", SURNAME);
    measureTime("find opt");

    resetTimer();
    removeNodeFromBook_L(contactBook_L, findNodeByKey_L(contactBook_L, "Webb", SURNAME));
    measureTime("remove opt");

    resetTimer();
    sortBookByKey_L(contactBook_L, PHONE);
    measureTime("sort");

    sortBookByKey_L(contactBook_L, SURNAME);

    resetTimer();
    //Zimmerman jest ostatni w liście
    findNodeByKey_L(contactBook_L, "Zimmerman", SURNAME);
    measureTime("find pes");

    resetTimer();
    removeNodeFromBook_L(contactBook_L, findNodeByKey_L(contactBook_L, "Zimmerman", SURNAME));
    measureTime("remove pes");

    deleteBook_L(contactBook_L);
}

int main() {
    loadLibraryFunctions();
    printf("list\n");
    bookOperations_L();
    printf("tree\n");
    bookOperations_T();

    return 0;
}
