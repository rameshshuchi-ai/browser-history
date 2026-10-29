#include <stdio.h>
#include <string.h>

#define MAX 100
#define PAGE_LEN 100

typedef struct {
    char items[MAX][PAGE_LEN];
    int top;
} Stack;

void initStack(Stack *s) { s->top = -1; }

int isEmpty(Stack *s) { return s->top == -1; }

int isFull(Stack *s) { return s->top == MAX - 1; }

int push(Stack *s, const char *page) {
    if (isFull(s)) {
        printf("History stack is full.\n");
        return 0;
    }
    strcpy(s->items[++s->top], page);
    return 1;
}

int pop(Stack *s, char *page) {
    if (isEmpty(s)) return 0;
    strcpy(page, s->items[s->top--]);
    return 1;
}

void showCurrent(const char *current) {
    printf("\nCurrent page: %s\n", current);
}

void showHistory(Stack *back, const char *current, Stack *forward) {
    int i;
    printf("\n--- Back history (oldest to newest) ---\n");
    if (isEmpty(back)) printf("(empty)\n");
    for (i = 0; i <= back->top; i++)
        printf("%d. %s\n", i + 1, back->items[i]);

    printf("Current: %s\n", current);

    printf("--- Forward stack (next page first) ---\n");
    if (isEmpty(forward)) printf("(empty)\n");
    for (i = forward->top; i >= 0; i--)
        printf("%s\n", forward->items[i]);
}

int main(void) {
    Stack backStack, forwardStack;
    char current[PAGE_LEN] = "Home";
    char page[PAGE_LEN];
    int choice;

    initStack(&backStack);
    initStack(&forwardStack);

    do {
        printf("\n===== BROWSER HISTORY SIMULATOR =====\n");
        printf("1. Visit a new page\n");
        printf("2. Go Back\n");
        printf("3. Go Forward\n");
        printf("4. Show Current Page\n");
        printf("5. Show Browsing History\n");
        printf("6. Clear History (return Home)\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        getchar(); /* consume newline */

        switch (choice) {
            case 1:
                printf("Enter page name or URL: ");
                if (!fgets(page, sizeof(page), stdin)) break;
                page[strcspn(page, "\n")] = '\0';
                if (page[0] == '\0') {
                    printf("Page name cannot be empty.\n");
                    break;
                }
                if (strcmp(page, current) == 0) {
                    printf("You are already on this page.\n");
                    break;
                }
                if (push(&backStack, current)) {
                    strcpy(current, page);
                    initStack(&forwardStack); /* new visit clears Forward */
                    printf("Visited: %s\n", current);
                }
                break;

            case 2:
                if (isEmpty(&backStack)) {
                    printf("No previous page available.\n");
                } else if (push(&forwardStack, current)) {
                    pop(&backStack, current);
                    printf("Moved Back.\n");
                }
                break;

            case 3:
                if (isEmpty(&forwardStack)) {
                    printf("No forward page available.\n");
                } else if (push(&backStack, current)) {
                    pop(&forwardStack, current);
                    printf("Moved Forward.\n");
                }
                break;

            case 4:
                showCurrent(current);
                break;

            case 5:
                showHistory(&backStack, current, &forwardStack);
                break;

            case 6:
                initStack(&backStack);
                initStack(&forwardStack);
                strcpy(current, "Home");
                printf("History cleared. Returned to Home.\n");
                break;

            case 0:
                printf("Exiting Browser History Simulator.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
