#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "contact.h"

Contact *createContact(char *name, char *surname, char *birthDate,
                       char *email, char *phone, char *address) {
    Contact *newContact = malloc(sizeof(Contact));

    newContact->name = malloc(strlen(name) + 1);
    strncpy(newContact->name, name, strlen(name) + 1);
    newContact->surname = malloc(strlen(surname) + 1);
    strncpy(newContact->surname, surname, strlen(surname) + 1);
    newContact->birthDate = malloc(strlen(birthDate) + 1);
    strncpy(newContact->birthDate, birthDate, strlen(birthDate) + 1);

    newContact->email = malloc(strlen(email) + 1);
    strncpy(newContact->email, email, strlen(email) + 1);
    newContact->phone = malloc(strlen(phone) + 1);
    strncpy(newContact->phone, phone, strlen(phone) + 1);
    newContact->address = malloc(strlen(address) + 1);
    strncpy(newContact->address, address, strlen(address) + 1);

    return newContact;
}

void printContact(Contact *contact) {
    if (contact != NULL) {
        printf("%s %s\n", contact->name, contact->surname);
        printf("%s\n", contact->birthDate);
        printf("%s\n", contact->email);
        printf("%s\n", contact->phone);
        printf("%s\n", contact->address);
    }
}

void deleteContact(Contact *contact) {
    if (contact != NULL) {
        free(contact->name);
        free(contact->surname);
        free(contact->birthDate);
        free(contact->email);
        free(contact->phone);
        free(contact->address);
        free(contact);
    }
}

char *getValueOfContact(Contact *contact, ContactKey key) {
    switch (key) {
        case NAME:
            return contact->name;
        case SURNAME:
            return contact->surname;
        case BIRTHDATE:
            return contact->birthDate;
        case EMAIL:
            return contact->email;
        case PHONE:
            return contact->phone;
        case ADDRESS:
            return contact->address;
    }
    return NULL;
}

int compareContacts(Contact *a, Contact *b, ContactKey key) {
    return strcmp(getValueOfContact(a, key), getValueOfContact(b, key));
}

int compareContactWithString(Contact *a, char *string, ContactKey key) {
    return strcmp(getValueOfContact(a, key), string);
}
