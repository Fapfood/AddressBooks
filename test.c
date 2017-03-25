#include <stdio.h>
#include <stdlib.h>
#include "src/treeBook.h"
#include "src/listBook.h"
#include <string.h>
#include <sys/resource.h>
#include <time.h>


struct rusage startRusage;
struct rusage endRusage;
struct timespec startReal;
struct timespec endReal;

static const char *DELIMITERS = ";";

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

void bookOperations_T() {
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

void bookOperations_L() {
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
    printf("list\n");
    bookOperations_L();
    printf("tree\n");
    bookOperations_T();

    return 0;
}
