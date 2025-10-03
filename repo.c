#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Structures ---
typedef struct Book 
{
    int id;
    char title[50];
    char author[50];
    int isIssued; // 0 = available, 1 = issued
    struct Book* next;
} Book;

typedef struct Section 
{
    char name[50];
    Book* books;
    struct Section* next;
} Section;

// --- Function Prototypes ---
Section* addSection(Section* head, char name[]);
Section* findSection(Section* head, char name[]);
void displaySections(Section* head);
void addBook(Section* sec, int id, char title[], char author[]);
void displayBooks(Section* sec);
int issueBook(Section* sec, int id);
int returnBook(Section* sec, int id);
int deleteBook(Section* sec, int id);
Book* searchBookByID(Section* sec, int id);
Book* searchBookByTitle(Section* sec, char title[]);
Book* searchBookByAuthor(Section* sec, char author[]);

Section* deleteSection(Section* head, char name[]);

// --- Function Implementations ---

Section* addSection(Section* head, char name[]) 
{
    Section* newSec = (Section*)malloc(sizeof(Section));
    strcpy(newSec->name, name);
    newSec->books = NULL;
    newSec->next = head;
    return newSec;
}

Section* findSection(Section* head, char name[]) 
{
    Section* temp = head;
    while(temp) {
        if(strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void displaySections(Section* head) 
{
    Section* temp = head;
    if(!temp) {
        printf("No sections found.\n");
        return;
    }
    printf("Library Sections:\n");
    while(temp) {
        printf("- %s\n", temp->name);
        temp = temp->next;
    }
}

void addBook(Section* sec, int id, char title[], char author[]) 
{
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isIssued = 0;
    newBook->next = sec->books;
    sec->books = newBook;
}

void displayBooks(Section* sec) 
{
    Book* temp = sec->books;
    if(!temp) {
        printf("No books in section %s.\n", sec->name);
        return;
    }
    printf("Books in section %s:\n", sec->name);
    while(temp) {
        printf("ID:%d | %s by %s | %s\n", temp->id, temp->title, temp->author, temp->isIssued ? "Issued" : "Available");
        temp = temp->next;
    }
}

// Search by ID
Book* searchBookByID(Section* sec, int id) {
    Book* temp = sec->books;
    while(temp) {
        if(temp->id == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Search by Title (case-insensitive)
Book* searchBookByTitle(Section* sec, char title[]) {
    Book* temp = sec->books;
    while(temp) {
        if(strcasecmp(temp->title, title) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Search by Author (case-insensitive)
Book* searchBookByAuthor(Section* sec, char author[]) {
    Book* temp = sec->books;
    while(temp) {
        if(strcasecmp(temp->author, author) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


int issueBook(Section* sec, int id) 
{
    Book* temp = sec->books;
    while(temp) {
        if(temp->id == id && temp->isIssued == 0) {
            temp->isIssued = 1;
            return 1; // success
        }
        temp = temp->next;
    }
    return 0; // fail
}

int returnBook(Section* sec, int id) 
{
    Book* temp = sec->books;
    while(temp) {
        if(temp->id == id && temp->isIssued == 1) {
            temp->isIssued = 0;
            return 1; // success
        }
        temp = temp->next;
    }
    return 0; // fail
}

int deleteBook(Section* sec, int id) 
{
    Book* temp = sec->books;
    Book* prev = NULL;
    while(temp) {
        if(temp->id == id) {
            if(prev)
                prev->next = temp->next;
            else
                sec->books = temp->next;
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->next;
    }
    return 0;
}

Section* deleteSection(Section* head, char name[]) 
{
    Section* temp = head;
    Section* prev = NULL;
    while(temp) {
        if(strcmp(temp->name, name) == 0) {
            // Free all books in section
            Book* b = temp->books;
            while(b) {
                Book* next = b->next;
                free(b);
                b = next;
            }
            if(prev)
                prev->next = temp->next;
            else
                head = temp->next;
            free(temp);
            return head;
        }
        prev = temp;
        temp = temp->next;
    }
    return head;
}

int main() {
    Section* library = NULL;
    int choice;
    char secName[50], title[50], author[50];
    int id;

    do {
        printf("\n--- Library System Menu ---\n");
        printf("1. Add Section\n2. Delete Section\n3. Display Sections\n");
        printf("4. Add Book\n5. Delete Book\n6. Display Books in Section\n");
        printf("7. Issue Book\n8. Return Book\n9.Search Book 10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) 
        {
            case 1:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                library = addSection(library, secName);
                printf("Section added.\n");
                break;

            case 2:
                printf("Enter Section Name to Delete: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                library = deleteSection(library, secName);
                printf("Section deleted if it existed.\n");
                break;

            case 3:
                displaySections(library);
                break;

            case 4:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                Section* sec = findSection(library, secName);
                if(sec) {
                    printf("Enter Book ID: "); scanf("%d", &id); getchar();
                    printf("Enter Book Title: "); fgets(title, 50, stdin); title[strcspn(title,"\n")]=0;
                    printf("Enter Author: "); fgets(author, 50, stdin); author[strcspn(author,"\n")]=0;
                    addBook(sec, id, title, author);
                    printf("Book added.\n");
                } else {
                    printf("Section not found.\n");
                }
                break;

            case 5:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin); secName[strcspn(secName,"\n")]=0;
                sec = findSection(library, secName);
                if(sec) {
                    printf("Enter Book ID to Delete: "); scanf("%d", &id); getchar();
                    if(deleteBook(sec, id)) printf("Book deleted.\n");
                    else printf("Book not found.\n");
                } else printf("Section not found.\n");
                break;

            case 6:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin); secName[strcspn(secName,"\n")]=0;
                sec = findSection(library, secName);
                if(sec) displayBooks(sec);
                else printf("Section not found.\n");
                break;

            case 7:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin); secName[strcspn(secName,"\n")]=0;
                sec = findSection(library, secName);
                if(sec) {
                    printf("Enter Book ID to Issue: "); scanf("%d", &id); getchar();
                    if(issueBook(sec, id)) printf("Book issued successfully.\n");
                    else printf("Book not available or not found.\n");
                } else printf("Section not found.\n");
                break;

            case 8:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin); secName[strcspn(secName,"\n")]=0;
                sec = findSection(library, secName);
                if(sec) {
                    printf("Enter Book ID to Return: "); scanf("%d", &id); getchar();
                    if(returnBook(sec, id)) printf("Book returned successfully.\n");
                    else printf("Book not found or not issued.\n");
                } else printf("Section not found.\n");
                break;
            case 9:
    printf("Enter Section Name: ");
    fgets(secName, 50, stdin);
    secName[strcspn(secName, "\n")] = 0;
    sec = findSection(library, secName);

    if(sec) {
        int searchChoice;
        printf("Search by:\n1. ID\n2. Title\n3. Author\nEnter choice: ");
        scanf("%d", &searchChoice); getchar();

        if(searchChoice == 1) {
            printf("Enter Book ID: ");
            scanf("%d", &id); getchar();
            Book* found = searchBookByID(sec, id);
            if(found) printf("Found: ID:%d | %s by %s | %s\n",
                            found->id, found->title, found->author,
                            found->isIssued ? "Issued" : "Available");
            else printf("Book not found.\n");

        } else if(searchChoice == 2) {
            printf("Enter Book Title: ");
            fgets(title, 50, stdin); title[strcspn(title,"\n")] = 0;
            Book* found = searchBookByTitle(sec, title);
            if(found) printf("Found: ID:%d | %s by %s | %s\n",
                            found->id, found->title, found->author,
                            found->isIssued ? "Issued" : "Available");
            else printf("Book not found.\n");

        } else if(searchChoice == 3) {
            printf("Enter Author Name: ");
            fgets(author, 50, stdin); author[strcspn(author,"\n")] = 0;
            Book* found = searchBookByAuthor(sec, author);
            if(found) printf("Found: ID:%d | %s by %s | %s\n",
                            found->id, found->title, found->author,
                            found->isIssued ? "Issued" : "Available");
            else printf("Book not found.\n");

        } else {
            printf("Invalid search option!\n");
        }

    } else {
        printf("Section not found.\n");
    }
    break;

            case 10:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 9);

    // Free memory
    while(library) library = deleteSection(library, library->name);

    return 0;
}
