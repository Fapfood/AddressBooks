#ifndef LAB1V2_CONTACT_H
#define LAB1V2_CONTACT_H


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

Contact *createContact(char *name, char *surname, char *birthDate,
                       char *email, char *phone, char *address);

void printContact(Contact *contact);

void deleteContact(Contact *contact);

char *getValueOfContact(Contact *contact, ContactKey key);

int compareContacts(Contact *a, Contact *b, ContactKey key);

int compareContactWithString(Contact *a, char *string, ContactKey key);

#endif //LAB1V2_CONTACT_H