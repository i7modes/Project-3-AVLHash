#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 211
#define R 199

typedef struct AVLnode *AVLNode;

struct AVLnode
{
    char Word[50];
    int Frequency;
    AVLNode Left;
    AVLNode Right;
    int Height;
};

typedef struct hashTable *hashTableP;

struct hashTable
{
    char Word[50];
    int Frequency;
    int Status; // Empty -> 0, Occupied -> 1, Deleted -> -1
};

int currentTableSize = 0;

int isStringContainOnlyLetters(char *);
char toLowerCase(char);

AVLNode MakeEmpty(AVLNode);
AVLNode Find(char *, AVLNode);
AVLNode FindMin(AVLNode);
AVLNode FindMax(AVLNode);
int Height(AVLNode);
int Max(int, int);
AVLNode SingleRotateWithLeft(AVLNode);
AVLNode SingleRotateWithRight(AVLNode);
AVLNode DoubleRotateWithLeft(AVLNode);
AVLNode DoubleRotateWithRight(AVLNode);
AVLNode Insert(char *, AVLNode);
AVLNode Delete(char *, AVLNode);
void PrintInOrder(AVLNode);

void hashInitialize(hashTableP, int);
int hashFunction(char *, int);
void hashInsert(hashTableP, char *, int);
void hashDisplay(hashTableP, int);
int hashSearch(hashTableP, char *, int);
void hashDelete(hashTableP, char *, int);
void insertInOrder(AVLNode, hashTableP, int);
void hashInsertWithFrequency(hashTableP, char *, int, int);
void displayStatistics(hashTableP, int);

int main()
{
    AVLNode myAvlTree = NULL;
    hashTableP myTable;

    FILE *input;

    int isLoaded = 0;
    int isAvlCreated = 0;
    int isHashTableCreated = 0;

    char tempText[200];
    char inputText[1000] = "";

    int choice;

    do
    {
        printf("Please Select one of the option you want:\n");
        printf("1. Load data from the file\n");
        printf("2. Create the AVL tree\n");
        printf("3. Insert a word to the AVL tree\n");
        printf("4. Delete a word from the AVL tree\n");
        printf("5. Print the words as sorted in the AVL tree\n");
        printf("6. Create the Hash Table\n");
        printf("7. Insert a word to the Hash table\n");
        printf("8. Delete a word from the hash table\n");
        printf("9. Search for a word in the hash table and print its frequency\n");
        printf("10. Print words statistics\n");
        printf("11. Exit\n\n");
        printf("Enter the option: ");

        scanf("%d", &choice);

        switch (choice)
        {

        case 1:

            if (isLoaded == 1)
            {
                printf("\nYou have already loaded the text\n\n");
            }
            else
            {
                input = fopen("input.txt", "r");

                if (input == NULL)
                {
                    printf("\nUnable to read the text from the file!\n\n");
                    exit(1);
                }
                else
                {
                    while (fgets(tempText, sizeof(tempText), input))
                    {
                        for (int i = 0; i < strlen(tempText); i++)
                        {
                            if (((tempText[i] >= 'A' && tempText[i] <= 'Z') || (tempText[i] >= 'a' && tempText[i] <= 'z') || tempText[i] == ' '))
                            {
                                strncat(inputText, &tempText[i], 1);
                            }
                        }
                        if (!(inputText[strlen(inputText) - 1] == ' '))
                        {
                            strcat(inputText, " ");
                        }
                    }
                    printf("\nThe Text has been loaded successfully\n");
                    printf("The input Text: %s\n\n", inputText);
                    isLoaded = 1;
                    fclose(input);
                }
            }
            break;

        case 2:

            if (inputText[0] == '\0')
            {
                printf("\nYou have to load the text first\n\n");
            }
            else
            {
                if (isAvlCreated == 0)
                {
                    myAvlTree = MakeEmpty(myAvlTree);

                    char *token = strtok(inputText, " ");
                    while (token != NULL)
                    {
                        myAvlTree = Insert(token, myAvlTree);
                        token = strtok(NULL, " ");
                    }

                    printf("\nThe AVL Tree has been created successfully\n\n");
                    isAvlCreated = 1;
                }
                else
                {
                    printf("\nYou have already created one\n\n");
                }
            }
            break;

        case 3:

            if (isAvlCreated == 1)
            {
                printf("Please Enter your word: ");
                scanf("%s", tempText);
                if (isStringContainOnlyLetters(tempText) == 1)
                {
                    myAvlTree = Insert(tempText, myAvlTree);
                    printf("\nThe word have been inserted successfully\n\n");
                }
                else
                {
                    printf("\nYou have to enter a string contains only letters!\n\n");
                    exit(1);
                }
            }
            else
            {
                printf("\nYou have to create the AVL Tree first\n\n");
            }
            break;

        case 4:

            if (isAvlCreated == 1)
            {
                printf("Please Enter tour word: ");
                scanf("%s", tempText);
                if (isStringContainOnlyLetters(tempText) == 1)
                {
                    myAvlTree = Delete(tempText, myAvlTree);
                }
                else
                {
                    printf("\nYou have to enter a string contains only letters!\n\n");
                    exit(1);
                }
            }
            else
            {
                printf("\nYou have to create the AVL Tree first\n\n");
            }
            break;

        case 5:

            if (isAvlCreated == 1)
            {
                printf("Tree (inOrder):\n");
                PrintInOrder(myAvlTree);
                printf("\n");
            }
            else
            {
                printf("\nYou have to create the AVL Tree first\n\n");
            }
            break;

        case 6:

            if (isAvlCreated == 1)
            {
                if (isHashTableCreated == 0)
                {
                    myTable = (struct hashTable *)malloc(sizeof(struct hashTable) * TABLE_SIZE);

                    if (myTable == NULL)
                    {
                        printf("\nOut of memory\n\n");
                        exit(1);
                    }

                    hashInitialize(myTable, TABLE_SIZE);
                    insertInOrder(myAvlTree, myTable, TABLE_SIZE);
                    // hashDisplay(myTable, TABLE_SIZE);
                    isHashTableCreated = 1;
                    printf("\nThe hash table has been created successfully\n\n");
                }
                else
                {
                    printf("\nYou have already created the hash table\n\n");
                }
            }
            else
            {
                printf("\nYou have to create the AVL Tree first\n\n");
            }
            break;

        case 7:

            if (isHashTableCreated == 1)
            {
                printf("Please Enter your word: ");
                scanf("%s", tempText);
                if (isStringContainOnlyLetters(tempText) == 1)
                {
                    hashInsert(myTable, tempText, TABLE_SIZE);
                }
                else
                {
                    printf("\nYou have to enter a string contains only letters!\n\n");
                    exit(1);
                }
            }
            else
            {
                printf("\nYou have to create the Hash Table first\n\n");
            }
            break;

        case 8:

            if (isHashTableCreated == 1)
            {
                printf("Please Enter your word: ");
                scanf("%s", tempText);
                if (isStringContainOnlyLetters(tempText) == 1)
                {
                    hashDelete(myTable, tempText, TABLE_SIZE);
                }
                else
                {
                    printf("\nYou have to enter a string contains only letters!\n\n");
                    exit(1);
                }
            }
            else
            {
                printf("\nYou have to create the Hash Table first\n\n");
            }
            break;

        case 9:

            if (isHashTableCreated == 1)
            {
                printf("Please Enter your word: ");
                scanf("%s", tempText);
                if (isStringContainOnlyLetters(tempText) == 1)
                {
                    int index = hashSearch(myTable, tempText, TABLE_SIZE);
                    if (index == -1)
                    {
                        printf("\nThe hash table is empty\n\n");
                    }
                    else if (index == -2)
                    {
                        printf("\nThe word you entered not found!\n\n");
                    }
                    else
                    {
                        printf("\nWord: %s | Frequency: %d\n\n", myTable[index].Word, myTable[index].Frequency);
                    }
                }
                else
                {
                    printf("\nYou have to enter a string contains only letters!\n\n");
                    exit(1);
                }
            }
            else
            {
                printf("\nYou have to create the Hash Table first\n\n");
            }
            break;

        case 10:

            if (isHashTableCreated == 1)
            {
                displayStatistics(myTable, TABLE_SIZE);
            }
            else
            {
                printf("\nYou have to create the Hash Table first\n\n");
            }
            break;

        case 11:

            printf("\nWe are gonna miss you <3\n");
            break;

        default:

            printf("\nAre you sure you entered the number between 1 - 11 :/\n\n");
        }
    } while (choice != 11);

    return 0;
}

int isStringContainOnlyLetters(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (!((string[i] >= 'A' && string[i] <= 'Z') || (string[i] >= 'a' && string[i] <= 'z')))
        {
            return 0;
        }
    }
    return 1;
}

char toLowerCase(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        letter = (char)(letter + 32);
    }

    return letter;
}

AVLNode MakeEmpty(AVLNode T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

AVLNode Find(char *Word, AVLNode T)
{
    if (T == NULL)
        return NULL;
    if (strcasecmp(Word, T->Word) < 0)
        return Find(Word, T->Left);
    else if (strcasecmp(Word, T->Word) > 0)
        return Find(Word, T->Right);
    else
        return T;
}

AVLNode FindMin(AVLNode T)
{
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

AVLNode FindMax(AVLNode T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

int Height(AVLNode T)
{
    if (T == NULL)
        return -1;
    else
        return T->Height;
}

int Max(int Lhs, int Rhs)
{
    return Lhs > Rhs ? Lhs : Rhs;
}

AVLNode SingleRotateWithLeft(AVLNode K2)
{
    AVLNode K1;
    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K2->Left), K2->Height) + 1;

    return K1;
}

AVLNode SingleRotateWithRight(AVLNode K1)
{
    AVLNode K2;
    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;

    return K2;
}

AVLNode DoubleRotateWithLeft(AVLNode K3)
{
    K3->Left = SingleRotateWithRight(K3->Left);
    return SingleRotateWithLeft(K3);
}

AVLNode DoubleRotateWithRight(AVLNode K1)
{
    K1->Right = SingleRotateWithLeft(K1->Right);
    return SingleRotateWithRight(K1);
}

AVLNode Insert(char *Word, AVLNode T)
{
    if (T == NULL)
    {
        T = malloc(sizeof(struct AVLnode));

        if (T == NULL)
        {
            printf("Out of space");
            exit(1);
        }
        else
        {
            strcpy(T->Word, Word);
            T->Frequency = 1;
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if (strcasecmp(Word, T->Word) < 0)
    {
        T->Left = Insert(Word, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
        {
            if (strcasecmp(Word, T->Left->Word) < 0)
            {
                T = SingleRotateWithLeft(T);
            }
            else
            {
                T = DoubleRotateWithLeft(T);
            }
        }
    }
    else if (strcasecmp(Word, T->Word) > 0)
    {
        T->Right = Insert(Word, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
        {
            if (strcasecmp(Word, T->Right->Word) > 0)
            {
                T = SingleRotateWithRight(T);
            }
            else
            {
                T = DoubleRotateWithRight(T);
            }
        }
    }
    else if (strcasecmp(Word, T->Word) == 0)
    {
        T->Frequency++;
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

AVLNode Delete(char *Word, AVLNode T)
{
    if (T == NULL)
    {
        printf("\nThe word you entered not found\n\n");
        return NULL;
    }

    if (strcasecmp(Word, T->Word) < 0)
    {
        T->Left = Delete(Word, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
        {
            if (strcasecmp(Word, T->Left->Word) < 0)
            {
                T = SingleRotateWithLeft(T);
            }
            else
            {
                T = DoubleRotateWithLeft(T);
            }
        }
    }
    else if (strcasecmp(Word, T->Word) > 0)
    {
        T->Right = Delete(Word, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
        {
            if (strcasecmp(Word, T->Right->Word) > 0)
            {
                T = SingleRotateWithRight(T);
            }
            else
            {
                T = DoubleRotateWithRight(T);
            }
        }
    }
    else
    {
        AVLNode temp;
        if (T->Left && T->Right)
        {
            temp = FindMin(T->Right);
            strcpy(T->Word, temp->Word);
            T->Frequency = temp->Frequency;
            T->Right = Delete(temp->Word, T->Right);

            if (Height(T->Right) - Height(T->Left) == 2)
            {
                if (strcasecmp(Word, T->Right->Word) > 0)
                {
                    T = SingleRotateWithRight(T);
                }
                else
                {
                    T = DoubleRotateWithRight(T);
                }
            }
        }
        else
        {
            temp = T;
            if (T->Left == NULL)
            {
                T = T->Right;
            }
            else if (T->Right == NULL)
            {
                T = T->Left;
            }
            free(temp);
            printf("\nThe word have been deleted successfully\n\n");
        }
    }

    if (T == NULL)
    {
        return T;
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

void PrintInOrder(AVLNode T)
{
    if (T != NULL)
    {
        PrintInOrder(T->Left);
        printf(" - Word: %s | Frequency: %d\n", T->Word, T->Frequency);
        PrintInOrder(T->Right);
    }
}

void hashInitialize(hashTableP hashTable, int TableSize)
{
    for (int i = 0; i < TableSize; i++)
    {
        hashTable[i].Status = 0;
    }
}

int hashFunction(char *Word, int TableSize)
{
    long long int hashValue = 0;

    while (*Word != '\0')
    {
        hashValue = (hashValue << 5) + toLowerCase(*Word++);
    }

    return (hashValue % TableSize);
}

void hashInsert(hashTableP hashTable, char *Word, int TableSize)
{
    int firstCaseIndex = hashFunction(Word, TableSize);
    int index = firstCaseIndex;
    int i = 1;

    if (currentTableSize == TableSize)
    {
        printf("Hash Table is full!\n\n");
        return;
    }

    while (hashTable[index].Status == 1)
    {
        if (strcasecmp(hashTable[index].Word, Word) == 0)
        {
            hashTable[index].Frequency++;
            return;
        }

        index = ((firstCaseIndex + (i * (R - (firstCaseIndex % R)))) % TableSize);

        if (index == firstCaseIndex)
        {
            printf("\nHash Table is full!!\n\n");
        }

        i++;
    }

    hashTable[index].Status = 1;
    strcpy(hashTable[index].Word, Word);
    hashTable[index].Frequency = 1;
    currentTableSize++;

    printf("\nThe word has been inserted successfully\n\n");
}

void hashDisplay(hashTableP hashTable, int TableSize)
{
    for (int i = 0; i < TableSize; i++)
    {
        if (hashTable[i].Status == 1)
        {
            printf("%d. Word = %s | Frequency = %d\n", i, hashTable[i].Word, hashTable[i].Frequency);
        }
        else if (hashTable[i].Status == -1)
        {
            printf("%d. Deleted\n", i);
        }
        else
        {
            printf("%d. Empty\n", i);
        }
    }
}

int hashSearch(hashTableP hashTable, char *Word, int TableSize)
{
    int firstCaseIndex = hashFunction(Word, TableSize);
    int index = firstCaseIndex;
    int i = 1;

    if (currentTableSize == 0)
    {
        return -1;
    }

    while (hashTable[index].Status != 0)
    {
        if (strcasecmp(hashTable[index].Word, Word) == 0 && hashTable[index].Status == 1)
        {
            return index;
        }

        index = ((firstCaseIndex + (i * (R - (firstCaseIndex % R)))) % TableSize);

        if (index == TableSize)
        {
            break;
        }

        i++;
    }

    return -2;
}

void hashDelete(hashTableP hashTable, char *Word, int TableSize)
{
    int index = hashSearch(hashTable, Word, TableSize);
    if (index == -1)
    {
        printf("\nThe hash table is empty\n\n");
    }
    else if (index == -2)
    {
        printf("\nThe word you entered not found\n\n");
    }
    else
    {
        hashTable[index].Status = -1;
        currentTableSize--;
        printf("\nThe word has been deleted successfully\n\n");
    }
}

void hashInsertWithFrequency(hashTableP hashTable, char *Word, int Frequency, int TableSize)
{
    int firstCaseIndex = hashFunction(Word, TableSize);
    int index = firstCaseIndex;
    int i = 1;

    if (currentTableSize == TableSize)
    {
        printf("\nHash Table is full!\n\n");
    }

    while (hashTable[index].Status == 1)
    {
        if (strcasecmp(hashTable[index].Word, Word) == 0)
        {
            hashTable[index].Frequency++;
            return;
        }

        index = ((firstCaseIndex + (i * (R - (firstCaseIndex % R)))) % TableSize);

        if (index == firstCaseIndex)
        {
            printf("\nHash Table is full!!\n\n");
            return;
        }

        i++;
    }

    hashTable[index].Status = 1;
    strcpy(hashTable[index].Word, Word);
    hashTable[index].Frequency = Frequency;
    currentTableSize++;
}

void insertInOrder(AVLNode T, hashTableP hashTable, int TableSize)
{
    if (T != NULL)
    {
        insertInOrder(T->Left, hashTable, TableSize);
        hashInsertWithFrequency(hashTable, T->Word, T->Frequency, TableSize);
        insertInOrder(T->Right, hashTable, TableSize);
    }
}

void displayStatistics(hashTableP hashTable, int TableSize)
{
    int uniqueWordCount = 0;
    char mostFrequentWord[50];
    int mostFrequency = 0;
    int counter = 0;

    int threshold;
    printf("Please Enter the threshold: ");
    int isInteger = scanf("%d", &threshold);

    if (isInteger == 1)
    {
        printf("\nThe Words that repeated more that the threshold\n");

        for (int i = 0; i < TableSize; i++)
        {
            if (hashTable[i].Status == 1)
            {
                uniqueWordCount++;
                if (hashTable[i].Frequency > mostFrequency)
                {
                    mostFrequency = hashTable[i].Frequency;
                    strcpy(mostFrequentWord, hashTable[i].Word);
                }
                if (hashTable[i].Frequency > threshold)
                {
                    counter++;
                    printf(" - Word: %s | Frequency: %d\n", hashTable[i].Word, hashTable[i].Frequency);
                }
            }
        }

        if (counter == 0)
        {
            printf(" - No Words\n");
        }

        printf("\nThe Total number of unique words: %d\n", uniqueWordCount);
        printf("Most frequent (Word: %s | Frequency: %d)\n\n", mostFrequentWord, mostFrequency);
    }
    else
    {
        printf("\nYou have to enter an integer only!\n\n");
        exit(1);
    }
}
