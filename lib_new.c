#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_FILE "users.txt"

// --- Structures ---
typedef struct Book {
    int id;
    char title[50];
    char author[50];
    int isIssued; // 0 = available, 1 = issued
    struct Book* next;
} Book;

typedef struct Section {
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
Section* deleteSection(Section* head, char name[]);
int login(char* role);

// --- Function Implementations ---
Section* addSection(Section* head, char name[]) {
    Section* newSec = (Section*)malloc(sizeof(Section));
    strcpy(newSec->name, name);
    newSec->books = NULL;
    newSec->next = head;
    return newSec;
}

Section* findSection(Section* head, char name[]) {
    Section* temp = head;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void displaySections(Section* head) {
    Section* temp = head;
    if (!temp) {
        printf("No sections found.\n");
        return;
    }
    printf("\nLibrary Sections:\n");
    while (temp) {
        printf("- %s\n", temp->name);
        temp = temp->next;
    }
}

void addBook(Section* sec, int id, char title[], char author[]) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isIssued = 0;
    newBook->next = sec->books;
    sec->books = newBook;
}

void displayBooks(Section* sec) {
    Book* temp = sec->books;
    if (!temp) {
        printf("No books in section %s.\n", sec->name);
        return;
    }
    printf("\nBooks in section %s:\n", sec->name);
    while (temp) {
        printf("ID:%d | %s by %s | %s\n", temp->id, temp->title, temp->author,
               temp->isIssued ? "Issued" : "Available");
        temp = temp->next;
    }
}

int issueBook(Section* sec, int id) {
    Book* temp = sec->books;
    while (temp) {
        if (temp->id == id && temp->isIssued == 0) {
            temp->isIssued = 1;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int returnBook(Section* sec, int id) {
    Book* temp = sec->books;
    while (temp) {
        if (temp->id == id && temp->isIssued == 1) {
            temp->isIssued = 0;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int deleteBook(Section* sec, int id) {
    Book* temp = sec->books;
    Book* prev = NULL;
    while (temp) {
        if (temp->id == id) {
            if (prev)
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

Section* deleteSection(Section* head, char name[]) {
    Section* temp = head;
    Section* prev = NULL;
    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            // Free all books in section
            Book* b = temp->books;
            while (b) {
                Book* next = b->next;
                free(b);
                b = next;
            }
            if (prev)
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

// --- User Login System ---
int login(char* role) {
    char username[50], password[50], fileUser[50], filePass[50], fileRole[20];
    FILE* fp = fopen(USER_FILE, "r");

    if (!fp) {
        printf("User file not found! Creating default users...\n");
        fp = fopen(USER_FILE, "w");
        fprintf(fp, "admin admin123 admin\n");
        fprintf(fp, "student student123 student\n");
        fclose(fp);
        fp = fopen(USER_FILE, "r");
    }

    printf("\nEnter Username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter Password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = 0;

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(role, fileRole);
            fclose(fp);
            printf("\nLogin successful! Role: %s\n", role);
            return 1;
        }
    }

    fclose(fp);
    printf("\nInvalid credentials!\n");
    return 0;
}

// --- MAIN FUNCTION ---
int main() {
    Section* library = NULL;
    int choice;
    char secName[50], title[50], author[50];
    int id;
    char role[20];

    if (!login(role)) {
        printf("Exiting program.\n");
        return 0;
    }

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Section (Admin)\n2. Delete Section (Admin)\n3. Display Sections\n");
        printf("4. Add Book (Admin)\n5. Delete Book (Admin)\n6. Display Books\n");
        printf("7. Issue Book (Student)\n8. Return Book (Student)\n9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        Section* sec;

        switch (choice) {
            case 1:
                if (strcmp(role, "admin") != 0) {
                    printf("Access denied! Admin only.\n");
                    break;
                }
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                library = addSection(library, secName);
                printf("Section added.\n");
                break;

            case 2:
                if (strcmp(role, "admin") != 0) {
                    printf("Access denied! Admin only.\n");
                    break;
                }
                printf("Enter Section Name to Delete: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                library = deleteSection(library, secName);
                printf("Section deleted (if existed).\n");
                break;

            case 3:
                displaySections(library);
                break;

            case 4:
                if (strcmp(role, "admin") != 0) {
                    printf("Access denied! Admin only.\n");
                    break;
                }
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                sec = findSection(library, secName);
                if (sec) {
                    printf("Enter Book ID: ");
                    scanf("%d", &id);
                    getchar();
                    printf("Enter Book Title: ");
                    fgets(title, 50, stdin);
                    title[strcspn(title, "\n")] = 0;
                    printf("Enter Author: ");
                    fgets(author, 50, stdin);
                    author[strcspn(author, "\n")] = 0;
                    addBook(sec, id, title, author);
                    printf("Book added.\n");
                } else {
                    printf("Section not found.\n");
                }
                break;

            case 5:
                if (strcmp(role, "admin") != 0) {
                    printf("Access denied! Admin only.\n");
                    break;
                }
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                sec = findSection(library, secName);
                if (sec) {
                    printf("Enter Book ID to Delete: ");
                    scanf("%d", &id);
                    getchar();
                    if (deleteBook(sec, id))
                        printf("Book deleted.\n");
                    else
                        printf("Book not found.\n");
                } else
                    printf("Section not found.\n");
                break;

            case 6:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                sec = findSection(library, secName);
                if (sec)
                    displayBooks(sec);
                else
                    printf("Section not found.\n");
                break;

            case 7:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                sec = findSection(library, secName);
                if (sec) {
                    printf("Enter Book ID to Issue: ");
                    scanf("%d", &id);
                    getchar();
                    if (issueBook(sec, id))
                        printf("Book issued successfully.\n");
                    else
                        printf("Book not found or already issued.\n");
                } else
                    printf("Section not found.\n");
                break;

            case 8:
                printf("Enter Section Name: ");
                fgets(secName, 50, stdin);
                secName[strcspn(secName, "\n")] = 0;
                sec = findSection(library, secName);
                if (sec) {
                    printf("Enter Book ID to Return: ");
                    scanf("%d", &id);
                    getchar();
                    if (returnBook(sec, id))
                        printf("Book returned successfully.\n");
                    else
                        printf("Book not found or not issued.\n");
                } else
                    printf("Section not found.\n");
                break;

            case 9:
                printf("Exiting system...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 9);

    // Free memory
    while (library)
        library = deleteSection(library, library->name);

    return 0;
}
