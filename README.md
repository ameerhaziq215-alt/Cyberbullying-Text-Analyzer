# Cyberbullying-Text-Analyzer
ReadMe

**How to compile and build ?**
This program was built using C language and compiled using GCC on Windows.
Firstly, terminal is opened or command is prompted in the directory containing the source code. To compile the code a command is needed.

Using Visual Studio Code: This program can be compiled within an IDE using a C compiler like MinGW, configured with C language. 

Input files required: The program is hardcoded to look for the following 3 text files. They must be present in the same directory as the executable file while running.
File Name: input.txt, cyberbullying_tweets.csv
It’s purpose is to analyze the main text file.
File Name: stopwords.txt
It’s purpose is that it contains a list of words to be loaded as stopwords.
File Name: toxicwords.txt
It’s purpose is that it consists of a list of words as toxic words.

Example of command line usage: 
It ensures that the 3 required input files are ready and to run an executable file from the command line.
Example Output: The program will attempt to open and load all 3 files and report the count of entries found. If a file is missing, it will print an error.
“Loaded 150 stopwords.”
 “Loaded 20 toxic words.“
“Loaded 5 lines of input text.”

**Dependencies:**
Libraries: The program relies only on the standard C (stdio.h) and string manipulation (string.h) libraries.
Special Headers: There is no external or special header files are required.
File Error Handling: The program includes basic checks to print an error message if any of the required input files cannot be opened
Limitations of constants: Hardcoded constants define the maximum limits for file loading
