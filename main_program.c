#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 1024
#define MAX_WORDS 2000
#define MAX_WORD_LENGTH 50

typedef struct
{
    char word[MAX_WORD_LENGTH]; // word string
    int count;
    int isToxic; // where 0 is non-toxic and 1 is toxic
} WordInfo;

static char g_lines[MAX_LINES][MAX_LINE_LENGTH];      // to store lines from the file
static char g_stopwords[MAX_WORDS][MAX_WORD_LENGTH];  // to store stopwords
static char g_toxicwords[MAX_WORDS][MAX_WORD_LENGTH]; // to store toxic words
static WordInfo g_word[MAX_WORDS];                    // to store word info

// PART1 is to load the main text file
int loadTextFile(const char *filename, char lines[][MAX_LINE_LENGTH], int *lineCount) // function to load text file
{
    FILE *file = fopen(filename, "r"); // open file in read mode

    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    *lineCount = 0;                                                 // initialize line count to zero
    while (fgets(lines[*lineCount], MAX_LINE_LENGTH, file) != NULL) // read each line
    {
        (*lineCount)++;              // increment line count
        if (*lineCount >= MAX_LINES) // check for max lines
        {
            break;
        }
    }
    fclose(file); // close file
    return 1;
}

// PART2 is to clean the word by removing punctuation and converting to lowercase
void cleanWord(char *word)
{
    if (word == NULL || *word == '\0')
    {
        return; // check for null or empty string
    }

    int i = 0, j = 0;

    while (word[i] != '\0' && j < MAX_WORD_LENGTH - 1) // iterate through each character and remove punctuation and convert to lowercase
    {
        char current_char = (char)word[i];                            // get current character
        char lower_char = (char)tolower((unsigned char)current_char); // convert to lowercase
        if (isalpha((unsigned char)lower_char))                       // check if character is alphabetic
        {
            word[j] = lower_char; // store cleaned character
            j++;                  // increment cleaned character index
        }
        i++; // increment original character index
    }

    word[j] = '\0'; // null-terminate cleaned word
}

int loadStopwords(const char *filename, char stopwords[][MAX_WORD_LENGTH], int *stopCount)
{
    FILE *file = fopen(filename, "r"); // open stopwords file in read mode

    if (file == NULL) // check for file open error
    {
        printf("Error opening stopwords file: %s\n", filename);
        return 0;
    }

    *stopCount = 0; // initialize stopword count to zero
    char line[MAX_WORD_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) // read each line
    {
        cleanWord(line); // clean the word
        if (strlen(line) > 0)
        {                                        // check for non-empty word
            strcpy(stopwords[*stopCount], line); // store stopword
            (*stopCount)++;                      // increment stopword count
            if (*stopCount >= MAX_WORDS)
            { // check for max stopwords
                break;
            }
        }
    }
    fclose(file); // close file
    return 1;
}

int loadToxicWords(const char *filename, char toxicwords[][MAX_WORD_LENGTH], int *toxicCount)
{
    FILE *file = fopen(filename, "r"); // open toxic words file in read mode

    if (file == NULL) // check for file open error
    {
        printf("Error opening toxic words file: %s\n", filename);
        return 0;
    }

    *toxicCount = 0; // initialize toxic word count to zero
    char line[MAX_WORD_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) // read each line
    {
        cleanWord(line); // clean the word
        if (strlen(line) > 0)
        {                                          // check for non-empty word
            strcpy(toxicwords[*toxicCount], line); // store toxic word
            (*toxicCount)++;                       // increment toxic word count
            if (*toxicCount >= MAX_WORDS)
            { // check for max toxic words
                break;
            }
        }
    }
    fclose(file); // close file
    return 1;
}

// PART3 is to check if a word is a stopword
int isStopword(const char *word, char stopwords[][MAX_WORD_LENGTH], int stopCount)
{
    for (int i = 0; i < stopCount; i++) // iterate through stopwords
    {
        if (strcmp(word, stopwords[i]) == 0) // compare words
        {
            return 1; // word is a stopword
        }
    }
    return 0; // word is not a stopword
}

// PART4 is for function to add or word in the list
void addOrUpdateWord(const char *word, WordInfo words[], int *wordCount, char stopWords[][MAX_WORD_LENGTH], int stopCount)
{ // function to add or update word in the list
    if (word == NULL || *word == '\0' || *wordCount >= MAX_WORDS)
    {           // to check for null or empty string or max words
        return; // check for null or empty string or max words
    }

    char cleanedWord[MAX_WORD_LENGTH];
    strncpy(cleanedWord, word, MAX_WORD_LENGTH - 1); // copy word to cleanedWord
    cleanedWord[MAX_WORD_LENGTH - 1] = '\0';         // null-terminate cleaned word
    cleanWord(cleanedWord);                          // normalize punctuation and case

    if (*cleanedWord == '\0' || isStopword(cleanedWord, stopWords, stopCount))
    {           // check for empty cleaned word or stopword
        return; // check for empty cleaned word or stopword
    }

    int found = 0;                       // initialize found flag to zero
    for (int i = 0; i < *wordCount; i++) // iterate through existing words
    {
        if (strcmp(words[i].word, cleanedWord) == 0) // compare words
        {
            words[i].count++; // increment word count
            found = 1;        // set found flag
            break;
        }
    }

    if (!found)
    {                                                                      // if word not found, add new word
        strncpy(words[*wordCount].word, cleanedWord, MAX_WORD_LENGTH - 1); // copy cleaned word to words array
        words[*wordCount].word[MAX_WORD_LENGTH - 1] = '\0';                // null-terminate word
        words[*wordCount].count = 1;                                       // initialize word count to 1
        (*wordCount)++;                                                    // increment word count
    }
}

// PART5 is for Functio to process all text lines and build the word list
void processTextLines(char lines[][MAX_LINE_LENGTH], int lineCount, char stopwords[][MAX_WORD_LENGTH], int stopCount, WordInfo words[], int *wordCount, int *totalWordCount)
{
    for (int i = 0; i < lineCount; i++)
    {                                                      // iterate through each line
        char line_copy[MAX_LINE_LENGTH];                   // copy of the line to tokenize
        strncpy(line_copy, lines[i], MAX_LINE_LENGTH - 1); // copy line to line_copy
        line_copy[MAX_LINE_LENGTH - 1] = '\0';             // null-terminate line_copy

        char *token = strtok(line_copy, " \t\n\r"); // tokenize line by whitespace

        while (token != NULL)
        {
            (*totalWordCount)++;                                            // increment total word count
            addOrUpdateWord(token, words, wordCount, stopwords, stopCount); // add or update word in the list
            token = strtok(NULL, " \t\n\r");                                // get next token
        }
    }
}

// PART6 is Toxic word analayser
int markToxicWords(WordInfo words[], int wordCount, char toxicWords[][MAX_WORD_LENGTH], int toxicCount) // function to mark toxic words
{
    int totalToxic = 0;                 // initialize total toxic word count to zero
    for (int i = 0; i < wordCount; i++) // iterate through each word
    {
        words[i].isToxic = 0;                // set isToxic flag to 0
        for (int j = 0; j < toxicCount; j++) // iterate through toxic words
        {
            if (strcmp(words[i].word, toxicWords[j]) == 0) // compare words
            {
                words[i].isToxic = 1;         // mark word as toxic
                totalToxic += words[i].count; // increment total toxic word count
                break;
            }
        }
    }
    return totalToxic; // return total toxic word count
}

double calculateToxicPercentage(int totalToxicWords, int totalWords)
{ // function to calculate toxic word percentage
    if (totalWords == 0)
    {               // check for zero total words
        return 0.0; // return 0.0 to avoid division by zero
    }
    return ((double)totalToxicWords / (double)totalWords) * 100.0; // calculate and return toxic word percentage
}

void sortByFrequency(WordInfo words[], int wordCount)
{                                           // function to sort words by frequency in descending order
    for (int i = 0; i < wordCount - 1; i++) // bubble sort algorithm
    {
        for (int j = 0; j < wordCount - i - 1; j++) // iterate through words
        {
            if (words[j].count < words[j + 1].count) // compare word counts
            {
                WordInfo temp = words[j]; // swap words
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void printTopWords(WordInfo words[], int wordCount, int N)
{                                                // function to print top N words
    printf("\nTop %d words by frequency:\n", N); // print header
    for (int i = 0; i < N && i < wordCount; i++) // iterate through top N words
    {
        printf("%s: %d\n", words[i].word, words[i].count); // print word and count
    }
}

void printTopToxicWords(WordInfo words[], int wordCount, int N)
{                                        // function to print top N toxic words
    printf("\nTop %d Toxic Words\n", N); //
    int printed = 0;                     // counter for printed toxic words
    for (int i = 0; i < wordCount; i++)  // iterate through words
    {
        if (words[i].isToxic) // check if word is toxic
        {
            printf("%d. %s (Count: %d)\n", printed + 1, words[i].word, words[i].count); // print toxic word and count
            printed++;                                                                  // increment printed counter
        }
        if (printed >= N) // limit to top N toxic words
            break;
    }
    if (printed == 0) // if no toxic words found
    {
        printf("(No toxic words found)\n");
    }
}

// PART7 is the menu and save report
static void clearBadInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clear input buffer
}

void saveReport(const char *filename, WordInfo words[], int wordCount, int totalWords, int totalToxicWords, double toxicPercentage)
{
    FILE *f = fopen(filename, "w"); // open report file in write mode
    if (f == NULL)
    { // check for file open error
        printf("Error opening report file: %s\n", filename);
        return;
    }
    fprintf(f, "==== Cyberbullying / Toxic Text Analysis Report ====\n\n");
    fprintf(f, "Total words (excluding stopwords): %d\n", totalWords);
    fprintf(f, "Total toxic words: %d\n", totalToxicWords);
    fprintf(f, "Percentage toxic words: %.2f%%\n\n", toxicPercentage);
    fprintf(f, "---- All Word Frequencies (excluding stopwords) ----\n");

    sortByFrequency(words, wordCount); // sort words by frequency

    fprintf(f, "Top 20 Frequent Words (with Toxic flag)\n");
    fprintf(f, "---------------------------------------\n");

    int limit = wordCount < 20 ? wordCount : 20; // determine limit for top words
    for (int i = 0; i < limit; i++)              // print top words
    {
        fprintf(f, "%2d. %-20s  Count: %-5d %s\n", i + 1, words[i].word, words[i].count, words[i].isToxic ? "(toxic)" : ""); // print word details
    }

    fprintf(f, "\nTop 20 Toxic Words\n");
    fprintf(f, "-------------------\n");
    int printed = 0;
    for (int i = 0; i < wordCount && printed < 20; i++)
    {
        if (words[i].isToxic)
        {
            fprintf(f, "%2d. %-20s  Count: %d\n",
                    printed + 1, words[i].word, words[i].count);
            printed++;
        }
    }
    if (printed == 0)
    {
        fprintf(f, "(No toxic words found)\n");
    }

    fclose(f);                                // close report file
    printf("Report saved to %s\n", filename); // print confirmation messag
}

// PART8 is the main function where everything comes together
int main(void)
{
    int lineCount = 0, stopCount = 0, toxicCount = 0, wordCount = 0, totalWords = 0, totalToxicWords = 0;
    double toxicPercentage = 0.0;
    int dataLoaded = 0, toxicDone = 0, choice;

    do
    {
        printf("\n==== Cyberbullying Text Analyzer ====\n");
        printf("1. Load text + word lists\n");
        printf("2. Show general word statistics\n");
        printf("3. Show toxic word statistics\n");
        printf("4. Show top N words / toxic words\n");
        printf("5. Save report to file\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        { // read user choice
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            clearBadInput(); // clear bad input
            continue;
        }

        if (choice == 1)
        {
            char textFile[100]; // buffer for text file name
            printf("Enter text file name (eg. input.txt): ");
            scanf("%s", textFile);

            if (!loadTextFile(textFile, g_lines, &lineCount))
                continue; // load text file
            if (!loadStopwords("stopwords.txt", g_stopwords, &stopCount))
                continue; // load stopwords file
            if (!loadToxicWords("toxicwords.txt", g_toxicwords, &toxicCount))
                continue; // load toxic words file

            wordCount = 0;
            totalWords = 0;
            totalToxicWords = 0;
            toxicPercentage = 0.0;
            toxicDone = 0;

            processTextLines(g_lines, lineCount, g_stopwords, stopCount, g_word, &wordCount, &totalWords); // process text lines
            dataLoaded = 1;

            printf("\nLoaded successfully!\n");
            printf("Lines read: %d\n", lineCount);
            printf("Total words: %d\n", totalWords);
            printf("Unique (non-stopwords): %d\n", wordCount);
        }
        else if (choice == 2)
        {
            if (!dataLoaded)
            { // check if data is loaded
                printf("Please load data first (option 1).\n");
                continue;
            }
            printf("\nGeneral Word Statistics:\n");
            printf("Total words (excluding stopwords): %d\n", totalWords);
            printf("Unique words (excluding stopwords): %d\n", wordCount);
        }
        else if (choice == 3)
        {
            if (!dataLoaded)
            { // check if data is loaded
                printf("Please load data first (option 1).\n");
                continue;
            }

            totalToxicWords = markToxicWords(g_word, wordCount, g_toxicwords, toxicCount); // mark toxic words
            toxicPercentage = calculateToxicPercentage(totalToxicWords, totalWords);       // calculate toxic percentage
            toxicDone = 1;

            printf("\n--- Toxic Word Statistics ---\n");
            printf("Total toxic occurrences: %d\n", totalToxicWords);
            printf("Toxic percentage: %.2f%%\n", toxicPercentage);
        }
        else if (choice == 4)
        {
            if (!dataLoaded)
            { // check if data is loaded
                printf("Please load text first (option 1).\n");
                continue;
            }
            if (!toxicDone)
            {                                                                                  // check if toxic analysis is done
                totalToxicWords = markToxicWords(g_word, wordCount, g_toxicwords, toxicCount); // mark toxic words
                toxicPercentage = calculateToxicPercentage(totalToxicWords, totalWords);       // calculate toxic percentage
                toxicDone = 1;                                                                 // set toxic done flag
            }

            int N;
            printf("Enter N (e.g. 10): ");      // prompt for N
            if (scanf("%d", &N) != 1 || N <= 0) // read N and validate
            {
                clearBadInput(); // clear bad input
                printf("Invalid N.\n");
                continue;
            }

            sortByFrequency(g_word, wordCount);       // sort words by frequency
            printTopWords(g_word, wordCount, N);      // print top N words
            printTopToxicWords(g_word, wordCount, N); // print top N toxic words
        }
        else if (choice == 5)
        {
            if (!dataLoaded)
            { // check if data is loaded
                printf("Please load text first (option 1).\n");
                continue;
            }
            if (!toxicDone)
            {                                                                                  // check if toxic analysis is done
                totalToxicWords = markToxicWords(g_word, wordCount, g_toxicwords, toxicCount); // mark toxic words
                toxicPercentage = calculateToxicPercentage(totalToxicWords, totalWords);       // calculate toxic percentage
                toxicDone = 1;                                                                 // set toxic done flag
            }

            char reportFile[100]; // buffer for report file name
            printf("Enter report file name (e.g. report.txt): ");
            scanf("%s", reportFile);

            saveReport(reportFile, g_word, wordCount, totalWords, totalToxicWords, toxicPercentage); // save report to file
        }
        else if (choice == 6)
        {
            printf("Exiting program. Goodbye!\n");
        }
        else
        {
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }

    } while (choice != 6); // loop until user chooses to exit
    return 0; // exit program
}
