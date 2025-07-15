// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
int daysBetweenDates(const char *d1, const char *d2);

// Books Structure
typedef struct
{
    int id;
    char title[100];
    char author[50];
    char category[50];
    int copies;
} Book;

// Credentials Structure
typedef struct
{
    char username[50];
    char password[50];
    int isAdmin;
} User;

// Issue Structure
typedef struct
{
    int bookID;
    char username[50];
    char issueDate[20];
    char returnDate[20];
    char status[20];
} IssueRecord;

struct books
{
    int id;
    char title[100];
    char author[100];
    char genre[50];
    int quantity;
};

int role = 0;
// Funtion for Authentication
int authenticateUser(char username[], char password[])
{
    FILE *fp = fopen("data/users.csv", "r");
    if (!fp)
    {
        printf("\nError: Unable to open user database.\n");
        return 0;
    }

    char header[100];
    fgets(header, sizeof(header), fp);

    User user;
    while (fscanf(fp, "%49[^,],%49[^,],%d\n", user.username, user.password, &user.isAdmin) == 3)
    {
        if (strcmp(username, user.username) == 0)
        {
            if (strcmp(password, user.password) == 0)
            {
                role = user.isAdmin;
                fclose(fp);
                printf("\n========================================\n");
                printf("|  Login Status: SUCCESS           |\n");
                printf("|  Welcome, %-23s|\n", username);
                printf("========================================\n");
                return 1;
            }
            else
            {
                fclose(fp);
                printf("\n========================================\n");
                printf("|  Login Status: AUTH FAILED      |\n");
                printf("|  Reason: Username Not Found     |\n");
                printf("========================================\n");
                return 0;
            }
        }
    }

    fclose(fp);
    printf("\n========================================\n");
    printf("|  Login Status: AUTH FAILED      |\n");
    printf("|  Reason: Username Not Found     |\n");
    printf("========================================\n");
    return 0;
}

// Function for Adding Book
void addBook()
{
    Book newBook;
    FILE *fp = fopen("data/books.csv", "a");
    if (!fp)
        return;

    printf("Enter Book ID: ");
    scanf("%d", &newBook.id);
    getchar();
    printf("Enter Title: ");
    fgets(newBook.title, 100, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(newBook.author, 50, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;
    printf("Enter Category: ");
    fgets(newBook.category, 50, stdin);
    newBook.category[strcspn(newBook.category, "\n")] = 0;
    printf("Enter Copies: ");
    scanf("%d", &newBook.copies);

    fprintf(fp, "%d,%s,%s,%s,%d\n", newBook.id, newBook.title, newBook.author, newBook.category, newBook.copies);
    fclose(fp);
    printf("========================================\n");
    printf("|        Book Added Successfully!      |\n");
    printf("========================================\n");
}

// Function for Displaying Books
void displayBooks()
{
    FILE *fp = fopen("data/books.csv", "r");
    Book book;
    if (!fp)
        return;

    char header[200];
    fgets(header, sizeof(header), fp);

    printf("\n===================================================================================================================\n");
    printf("| %-5s | %-30s | %-20s | %-15s | %-6s |\n", "ID", "Title", "Author", "Category", "Copies");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n", &book.id, book.title, book.author, book.category, &book.copies) == 5)
    {
        printf("| %-5d | %-30s | %-20s | %-15s | %-6d |\n",
               book.id, book.title, book.author, book.category, book.copies);
    }

    printf("===================================================================================================================\n");

    fclose(fp);
}

// Function for Inventory Monitoring
void inventoryReport()
{
    FILE *fp = fopen("data/books.csv", "r");
    Book book;
    int total = 0;

    if (!fp)
    {
        printf(" Could not open books.csv\n");
        return;
    }

    char header[200];
    fgets(header, sizeof(header), fp);

    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n", &book.id, book.title, book.author, book.category, &book.copies) == 5)
    {
        total += book.copies;
    }

    printf("\n================================================\n");
    printf("|               Inventory Report              |\n");
    printf("================================================\n");
    printf("| Total number of books available: %-10d |\n", total);
    printf("================================================\n");

    fclose(fp);
}

// Function for searching book by author
void searchByAuthor()
{
    FILE *fp = fopen("data/books.csv", "r");
    Book b;
    char authorname[100];
    int found = 0;
    char line[256];

    printf("\n============================================\n");
    printf("|            SEARCH BY AUTHOR              |\n");
    printf("============================================\n");
    printf("Enter Author Name to Search: ");
    getchar();
    fgets(authorname, sizeof(authorname), stdin);
    authorname[strcspn(authorname, "\n")] = 0;

    if (!fp)
    {
        printf("Error opening file!\n");
        printf("============================================\n");
        return;
    }

    fgets(line, sizeof(line), fp);
    printf("\n===================================================================================================================\n");
    printf("| %-5s | %-30s | %-20s | %-15s | %-6s |\n", "ID", "Title", "Author", "Category", "Copies");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n",
                  &b.id, b.title, b.author, b.category, &b.copies) == 5)
    {
        if (strcmp(b.author, authorname) == 0)
        {
            printf("| %-5d | %-30s | %-20s | %-15s | %-6d |\n",
                   b.id, b.title, b.author, b.category, b.copies);
            found = 1;
        }
    }

    if (!found)
    {
        printf("| %-98s |\n", "No books found by the specified author.");
    }

    printf("===================================================================================================================\n");
    fclose(fp);
}

// Function for searching book by category
void searchByCategory()
{
    FILE *fp = fopen("data/books.csv", "r");
    Book b;
    char category[100];
    int found = 0;
    char line[256];

    printf("\n============================================\n");
    printf("|           SEARCH BY CATEGORY             |\n");
    printf("============================================\n");
    printf("Enter Category to Search: ");
    getchar();
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;

    if (!fp)
    {
        printf("Error opening file!\n");
        printf("============================================\n");
        return;
    }

    fgets(line, sizeof(line), fp);
    printf("\n===================================================================================================================\n");
    printf("| %-5s | %-30s | %-20s | %-15s | %-6s |\n", "ID", "Title", "Author", "Category", "Copies");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n",
                  &b.id, b.title, b.author, b.category, &b.copies) == 5)
    {
        if (strcmp(b.category, category) == 0)
        {
            printf("| %-5d | %-30s | %-20s | %-15s | %-6d |\n",
                   b.id, b.title, b.author, b.category, b.copies);
            found = 1;
        }
    }

    if (!found)
    {
        printf("| %-98s |\n", "No books found in the specified category.");
    }

    printf("===================================================================================================================\n");
    fclose(fp);
}

// Function for searching book by name
void searchByBook()
{
    FILE *fp = fopen("data/books.csv", "r");
    Book b;
    char bookname[100];
    int found = 0;
    char line[256];

    printf("\n============================================\n");
    printf("|            SEARCH BY TITLE               |\n");
    printf("============================================\n");
    printf("Enter Book Title to Search: ");
    getchar();
    fgets(bookname, sizeof(bookname), stdin);
    bookname[strcspn(bookname, "\n")] = 0;

    if (!fp)
    {
        printf("  Error opening file!\n");
        printf("  ============================================\n");
        return;
    }

    fgets(line, sizeof(line), fp);
    printf("\n===================================================================================================================\n");
    printf("| %-5s | %-30s | %-20s | %-15s | %-6s |\n", "ID", "Title", "Author", "Category", "Copies");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n",
                  &b.id, b.title, b.author, b.category, &b.copies) == 5)
    {
        if (strcmp(b.title, bookname) == 0)
        {
            printf("| %-5d | %-30s | %-20s | %-15s | %-6d |\n",
                   b.id, b.title, b.author, b.category, b.copies);
            found = 1;
        }
    }

    if (!found)
    {
        printf("| %-98s |\n", "No books found with the specified title.");
    }

    printf("===================================================================================================================\n");
    fclose(fp);
}

// Function for getting current date in DD-MM-YYYY format
void getCurrentDate(char *buffer)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// Function to Add days to a given date string
void getFutureDate(char *dateStr, int daysToAdd, char *result)
{
    struct tm tm = {0};
    sscanf(dateStr, "%d-%d-%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;

    time_t rawTime = mktime(&tm);
    rawTime += daysToAdd * 24 * 3600;

    struct tm *newTime = localtime(&rawTime);
    sprintf(result, "%02d-%02d-%d", newTime->tm_mday, newTime->tm_mon + 1, newTime->tm_year + 1900);
}

// Function to Find book by title, author, or ID
int findBook(char *input, Book *foundBook)
{
    FILE *fp = fopen("data/books.csv", "r");
    if (!fp)
        return 0;

    char header[200];
    fgets(header, sizeof(header), fp);

    Book book;
    while (fscanf(fp, "%d,%99[^,],%49[^,],%49[^,],%d\n", &book.id, book.title, book.author, book.category, &book.copies) == 5)
    {
        if (strstr(book.title, input) || strstr(book.author, input) || atoi(input) == book.id)
        {
            *foundBook = book;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Function to Issue Book
void issueBook(char username[])
{
    char search[100];
    Book book;

    printf("\n============================================\n");
    printf("|                 ISSUE BOOK                |\n");
    printf("============================================\n");
    printf("Enter Book Title/Author/ID to Issue: ");
    getchar();
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = 0;

    if (!findBook(search, &book))
    {
        printf("============================================\n");
        printf("Book not found.\n");
        printf("============================================\n");
        return;
    }

    if (book.copies <= 0)
    {
        printf("============================================\n");
        printf("No copies available for issue.\n");
        printf("============================================\n");
        return;
    }

    printf("============================================\n");
    printf("\nBook Found\n");
    printf("================================================================================================\n");
    printf("| ID: %d | Title: %s | Author: %s | Category: %s | Available: %d|\n",
           book.id, book.title, book.author, book.category, book.copies);
    printf("================================================================================================\n");

    char confirm;
    printf("  Do you want to issue this book? (Y/N): ");
    scanf(" %c", &confirm);
    if (confirm != 'Y' && confirm != 'y')
    {
        printf("============================================\n");
        printf("Issue cancelled.\n");
        printf("============================================\n");
        return;
    }

    FILE *fp = fopen("data/issued.csv", "a+");
    if (!fp)
    {
        printf("============================================\n");
        printf("  Error opening issued.csv\n");
        printf("============================================\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        fprintf(fp, "BookID,Username,IssueDate,ReturnDate\n");
    }

    char issueDate[20], returnDate[20];
    getCurrentDate(issueDate);
    getFutureDate(issueDate, 7, returnDate);

    fprintf(fp, "%d,%s,%s,%s\n", book.id, username, issueDate, returnDate);
    fclose(fp);

    FILE *in = fopen("data/books.csv", "r");
    FILE *out = fopen("data/temp.csv", "w");

    if (!in || !out)
    {
        printf("============================================\n");
        printf("Error updating books file.\n");
        printf("============================================\n");
        return;
    }

    char line[200];
    fgets(line, sizeof(line), in);
    fprintf(out, "%s", line);

    Book temp;
    while (fscanf(in, "%d,%99[^,],%49[^,],%49[^,],%d\n", &temp.id, temp.title, temp.author, temp.category, &temp.copies) == 5)
    {
        if (temp.id == book.id)
            temp.copies--;

        fprintf(out, "%d,%s,%s,%s,%d\n", temp.id, temp.title, temp.author, temp.category, temp.copies);
    }

    fclose(in);
    fclose(out);
    remove("data/books.csv");
    rename("data/temp.csv", "data/books.csv");

    printf("==================================================\n");
    printf("Book issued successfully! Return by: %s\n", returnDate);
    printf("==================================================\n");
}

// Function to Renew Book
void renewBook(char username[])
{
    int bookId;
    char confirm;
    printf("\n============================================\n");
    printf("|                RENEW BOOK                 |\n");
    printf("============================================\n");
    printf("Enter Book ID to renew: ");
    scanf("%d", &bookId);
    printf("Renewing this book will cost Rs. 15. Proceed? (Y/N): ");
    scanf(" %c", &confirm);
    if (confirm != 'Y' && confirm != 'y')
    {
        printf("Renewal cancelled.\n");
        return;
    }
    printf("Rs.15 renewal fee applied successfully!\n");

    FILE *in = fopen("data/issued.csv", "r");
    FILE *out = fopen("data/temp.csv", "w");

    if (!in || !out)
    {
        printf("============================================\n");
        printf("Error accessing issue records.\n");
        printf("============================================\n");
        return;
    }

    char line[200];
    int renewed = 0;

    if (fgets(line, sizeof(line), in))
    {
        fprintf(out, "%s", line);
    }

    while (fgets(line, sizeof(line), in))
    {
        IssueRecord record;
        char returnDate[20];

        if (sscanf(line, "%d,%49[^,],%19[^,],%19[^\n]", &record.bookID, record.username, record.issueDate, returnDate) == 4)
        {
            if (record.bookID == bookId && strcmp(record.username, username) == 0)
            {
                char newReturnDate[20];
                getFutureDate(returnDate, 7, newReturnDate);
                fprintf(out, "%d,%s,%s,%s\n", record.bookID, record.username, record.issueDate, newReturnDate);
                renewed = 1;
            }
            else
            {
                fprintf(out, "%d,%s,%s,%s\n", record.bookID, record.username, record.issueDate, returnDate);
            }
        }
    }

    fclose(in);
    fclose(out);
    remove("data/issued.csv");
    rename("data/temp.csv", "data/issued.csv");

    if (renewed)
    {
        printf("============================================\n");
        printf("Book renewed successfully.\n");
    }
    else
    {
        printf("============================================\n");
        printf("No matching issue record found.\n");
        printf("============================================\n");
    }
    printf("============================================\n");
}

// Function to Return Book
void returnBook(char username[])
{
    int bookId;
    printf("\n============================================\n");
    printf("|               RETURN BOOK                 |\n");
    printf("============================================\n");
    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);

    FILE *in = fopen("data/issued.csv", "r");
    FILE *out = fopen("data/temp.csv", "w");

    if (!in || !out)
    {
        printf("============================================\n");
        printf("Error opening issue records.\n");
        printf("============================================\n");
        return;
    }

    char line[200];
    int returned = 0;

    if (fgets(line, sizeof(line), in))
    {
        fprintf(out, "%s", line);
    }

    while (fgets(line, sizeof(line), in))
    {
        IssueRecord record;
        char returnDate[20];

        if (sscanf(line, "%d,%49[^,],%19[^,],%19[^\n]", &record.bookID, record.username, record.issueDate, returnDate) == 4)
        {
            if (record.bookID == bookId && strcmp(record.username, username) == 0)
            {
                returned = 1;
            }
            else
            {
                fprintf(out, "%d,%s,%s,%s\n", record.bookID, record.username, record.issueDate, returnDate);
            }
        }
    }

    fclose(in);
    fclose(out);
    remove("data/issued.csv");
    rename("data/temp.csv", "data/issued.csv");

    if (returned)
    {
        FILE *bin = fopen("data/books.csv", "r");
        FILE *bout = fopen("data/temp.csv", "w");

        if (!bin || !bout)
        {
            printf("============================================\n");
            printf("Error updating books database.\n");
            printf("============================================\n");
            return;
        }

        fgets(line, sizeof(line), bin);
        fprintf(bout, "%s", line);

        Book temp;
        while (fscanf(bin, "%d,%99[^,],%49[^,],%49[^,],%d\n", &temp.id, temp.title, temp.author, temp.category, &temp.copies) == 5)
        {
            if (temp.id == bookId)
                temp.copies++;

            fprintf(bout, "%d,%s,%s,%s,%d\n", temp.id, temp.title, temp.author, temp.category, temp.copies);
        }

        fclose(bin);
        fclose(bout);
        remove("data/books.csv");
        rename("data/temp.csv", "data/books.csv");

        printf("============================================\n");
        printf("Book returned successfully.\n");
    }
    else
    {
        printf("============================================\n");
        printf("No matching issue record found.\n");
    }

    printf("============================================\n");
}

// Alert and Penalty Checker
void checkAlertsAndPenalty(char username[])
{
    FILE *fp = fopen("data/issued.csv", "r");
    if (!fp)
    {
        printf("Error accessing issued records.\n");
        return;
    }

    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    char line[256];
    IssueRecord record;
    char returnDate[20];
    int hasRecords = 0;
    int totalPenalty = 0;

    fgets(line, sizeof(line), fp);
    fprintf(temp, "BookID,Username,IssueDate,ReturnDate,Penalty\n");

    printf("\n=================================================================================================================\n");
    printf("| %-5s | %-30s | %-15s | %-15s | %-15s | %-10s   |\n", "ID", "Title", "Issue Date", "Return Date", "Status", "Penalty");
    printf("-----------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%d,%49[^,],%19[^,],%19[^,\n]", &record.bookID, record.username, record.issueDate, returnDate) == 4)
        {
            int penalty = 0;
            char status[20];

            char today[20];
            getCurrentDate(today);
            int diff = daysBetweenDates(today, returnDate);

            if (diff < 0)
            {
                strcpy(status, "Overdue");
                penalty = (-diff) * 5;
            }
            else
            {
                strcpy(status, "Within Time");
            }

            fprintf(temp, "%d,%s,%s,%s,%d\n", record.bookID, record.username, record.issueDate, returnDate, penalty);

            if (strcmp(record.username, username) == 0)
            {
                hasRecords = 1;

                if (strcmp(status, "Overdue") == 0)
                {
                    totalPenalty += penalty;
                }

                FILE *bookFile = fopen("data/books.csv", "r");
                char bookLine[256];
                Book book;
                char title[100] = "N/A";

                if (bookFile)
                {
                    fgets(bookLine, sizeof(bookLine), bookFile);
                    while (fgets(bookLine, sizeof(bookLine), bookFile))
                    {
                        if (sscanf(bookLine, "%d,%99[^,],%49[^,],%49[^,],%d", &book.id, book.title, book.author, book.category, &book.copies) == 5)
                        {
                            if (book.id == record.bookID)
                            {
                                strcpy(title, book.title);
                                break;
                            }
                        }
                    }
                    fclose(bookFile);
                }

                printf("| %-5d | %-30s | %-15s | %-15s | %-15s | Rs.%-8d |\n",
                       record.bookID, title, record.issueDate, returnDate, status, penalty);
            }
        }
    }

    printf("=================================================================================================================\n");

    if (!hasRecords)
    {
        printf("|                                    No books issued currently by you.                                     |\n");
        printf("=================================================================================================================\n");
    }
    else if (totalPenalty > 0)
    {
        printf("\nNOTE: You have overdue books. Total penalty so far: Rs.%5d /- \n", totalPenalty);
    }

    fclose(fp);
    fclose(temp);

    remove("data/issued.csv");
    rename("data/temp.csv", "data/issued.csv");
}

// Penalty Payment Function
void penaltyPay(char username[])
{
    FILE *fp = fopen("data/issued.csv", "r");
    if (!fp)
    {
        printf("Error opening issued books file.\n");
        return;
    }

    IssueRecord ib;
    char line[256];
    int totalPenalty = 0;
    int overdue = 0;

    fgets(line, sizeof(line), fp);
    printf("\n===================================================================================================================\n");
    printf("| %-5s | %-30s | %-12s | %-12s | %-10s |\n", "ID", "Book Title", "Issue Date", "Return Date", "Penalty");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        int penalty = 0;
        if (sscanf(line, "%d,%49[^,],%19[^,],%19[^,],%d", &ib.bookID, ib.username, ib.issueDate, ib.returnDate, &penalty) == 5)
        {
            if (strcmp(ib.username, username) == 0 && penalty > 0)
            {
                overdue++;
                totalPenalty += penalty;
                printf("| %-5d | %-30s | %-12s | %-12s | Rs.%-8d |\n", ib.bookID, "Book Name", ib.issueDate, ib.returnDate, penalty);
            }
        }
    }

    printf("===================================================================================================================\n");

    fclose(fp);

    if (overdue == 0)
    {
        printf("\nYou have no pending penalties. All dues are up to date!\n");
        return;
    }

    printf("\nTotal Penalty Due: Rs. %d\n", totalPenalty);
    printf("Would you like to pay now? (1 = Yes / 2 = No): ");
    int choice;
    scanf("%d", &choice);
    getchar();
    if (choice == 1)
    {
        printf("\n===============================================\n");
        printf("|  Pay using any UPI app on the given number  |\n");
        printf("|                                             |\n");
        printf("|     |||||||||||||||||||||||||||||||||       |\n");
        printf("|     |PAY Rs. %-5d TO LIBRARY ACCOUNT|       |\n", totalPenalty);
        printf("|     |||||||||||||||||||||||||||||||||       |\n");
        printf("|             [+91 99XXX 99XXX]               |\n");
        printf("|                                             |\n");
        printf("===============================================\n");
        printf("Once paid, notify the admin manually for update.\n");
    }
    else
    {
        printf("\nPlease ensure you pay the penalty on time to avoid restrictions.\n");
    }
}

// Check Penalty Function
void checkPenalty()
{
    FILE *fp = fopen("data/issued.csv", "r");
    if (!fp)
    {
        printf("Error: Could not open issued.csv\n");
        return;
    }
    char line[256];
    IssueRecord record;
    char returnDate[20];

    int alertCount = 0;

    fgets(line, sizeof(line), fp);

    printf("\n=================================================================================================================\n");
    printf("| %-5s | %-30s | %-15s | %-15s | %-15s | %-10s   |\n", "ID", "Title", "Issued To", "Return Date", "Status", "Penalty");
    printf("-----------------------------------------------------------------------------------------------------------------\n");

    char today[20];
    getCurrentDate(today);

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%d,%49[^,],%19[^,],%19[^,],%*d", &record.bookID, record.username, record.issueDate, returnDate) == 4)
        {
            int diff = daysBetweenDates(today, returnDate);
            if (diff < 0)
            {
                int penalty = (-diff) * 5;
                alertCount++;

                FILE *bookFile = fopen("data/books.csv", "r");
                char bookLine[256];
                Book book;
                char title[100] = "N/A";

                if (bookFile)
                {
                    fgets(bookLine, sizeof(bookLine), bookFile);
                    while (fgets(bookLine, sizeof(bookLine), bookFile))
                    {
                        if (sscanf(bookLine, "%d,%99[^,],%49[^,],%49[^,],%d", &book.id, book.title, book.author, book.category, &book.copies) == 5)
                        {
                            if (book.id == record.bookID)
                            {
                                strcpy(title, book.title);
                                break;
                            }
                        }
                    }
                    fclose(bookFile);
                }

                printf("| %-5d | %-30s | %-15s | %-15s | %-15s | Rs.%-8d |\n",
                       record.bookID, title, record.username, returnDate, "Overdue", penalty);
            }
        }
    }

    if (alertCount == 0)
    {
        printf("=================================================================================================================\n");
        printf("|                                     No overdue books today.                                           |\n");
    }

    printf("=================================================================================================================\n");

    fclose(fp);
}

// Date Difference Utility
int daysBetweenDates(const char *d1, const char *d2)
{
    struct tm tm1 = {0}, tm2 = {0};
    time_t t1, t2;

    // Parse date in DD-MM-YYYY format
    sscanf(d1, "%d-%d-%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year);
    sscanf(d2, "%d-%d-%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year);

    tm1.tm_mon -= 1;
    tm1.tm_year -= 1900;
    tm2.tm_mon -= 1;
    tm2.tm_year -= 1900;

    t1 = mktime(&tm1);
    t2 = mktime(&tm2);

    return (int)difftime(t2, t1) / (60 * 60 * 24);
}

// Admin Menu
void adminMenu()
{
    int choice;
    do
    {
        printf("\n========================================\n");
        printf("|               ADMIN PANEL           |\n");
        printf("========================================\n");
        printf("| 1.  Add Book                        |\n");
        printf("| 2.  Display Books                   |\n");
        printf("| 3.  Inventory Report                |\n");
        printf("| 4.  Check Penalty                   |\n");
        printf("| 5.  Exit Admin Panel                |\n");
        printf("========================================\n");
        printf("Select an option [1-5]: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            inventoryReport();
            break;
        case 4:
            checkPenalty();
            break;
        case 5:
            printf("Exiting Admin Panel...\n");
            break;
        default:
            printf("Invalid Choice!\n");
        }
    } while (choice != 5);
}

// User Menu
void userMenu(char username[])
{
    int choice;
    char temp[50];

    do
    {
        printf("\n========================================\n");
        printf("|              USER PANEL               |\n");
        printf("========================================\n");
        printf("| 1. Issue Book                         |\n");
        printf("| 2. Renew Book                         |\n");
        printf("| 3. Return Book                        |\n");
        printf("| 4. Pay Penalty                        |\n");
        printf("| 5. Search Book                        |\n");
        printf("| 6. Exit User Panel                    |\n");
        printf("========================================\n");
        printf("Select an option [1-6]: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            issueBook(username);
            break;
        case 2:
            renewBook(username);
            break;
        case 3:
            returnBook(username);
            break;
        case 4:
            penaltyPay(username);
            break;
        case 5:
            printf("\n");
            printf("==================================================\n");
            printf("|                 SEARCH PANEL                  |\n");
            printf("==================================================\n");
            printf("| 1. Search by Author                            |\n");
            printf("| 2. Search by Category                          |\n");
            printf("| 3. Search by Book Title                        |\n");
            printf("==================================================\n");
            printf("Select an option [1-3]: ");
            int opt;
            scanf("%d", &opt);
            if (opt == 1)
            {
                searchByAuthor(temp);
            }
            else if (opt == 2)
            {
                searchByCategory(temp);
            }
            else if (opt == 3)
            {
                searchByBook(temp);
            }
            else
            {
                printf("Invalid Search Option!\n");
            }
            break;
        case 6:
            printf("Exiting User Panel...\n");
            break;
        default:
            printf("Invalid Choice!\n");
        }
    } while (choice != 6);
}

int authenticateUser(char username[], char password[]);
void adminMenu();
void userMenu(char username[]);

// Main Program
int main()
{
    char username[50], password[50];
    int i = 0;
    char ch;

    printf("========================================\n");
    printf("Username: ");
    scanf("%s", username);
    printf("========================================\n");
    printf("Password: ");

    while ((ch = getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else if (i < sizeof(password) - 1)
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n========================================\n");
    checkAlertsAndPenalty(username);

    if (!authenticateUser(username, password))
    {
        printf("Authentication Failed!\n");
        return 0;
    }
    else if (role == 1)
    {
        int panelChoice;
        printf("\n========================================\n");
        printf("|           SELECT YOUR PANEL           |\n");
        printf("========================================\n");
        printf("| 1. Admin Panel                        |\n");
        printf("| 2. User Panel                         |\n");
        printf("========================================\n");
        printf("Where do you want to go [1-2]: ");
        scanf("%d", &panelChoice);

        if (panelChoice == 1)
            adminMenu();
        else if (panelChoice == 2)
        {
            userMenu(username);
        }
        else
            printf("Invalid choice!\n");
    }
    else
    {
        userMenu(username);
    }

    return 0;
}