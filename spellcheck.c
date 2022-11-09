/*!
@file spellcheck.c
@author Timothy Wong
@course RSE1201
@week 10
@lab09
@date 28/10/2022
@brief This file contains the definitions for the functions to
       spellcheck, covert to uppercase, find shortest and longest
       words, and find words starting with a specific char.
*//*_____________________________________________________________*/

#include <string.h>		// strcpy, strcmp, strlen
#include <stdio.h>		// printf, fopen, fclose
#include <ctype.h>

#include "spellcheck.h"

char* str_to_upper(char* string) { //converts all lowercase letters to uppercase, and returns a pointer to the first character of the string
    int j = 0;
    while (string[j]) {
        string[j] = toupper(string[j]);
        j++;
    }
    return string;
}

WordCount words_starting_with(const char* dictionary, char letter) { //Given the file name of a dictionary text file, counts the number of words that start with a given letter
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL) return FILE_ERR_OPEN;
    letter = toupper(letter);
    char *ch;
    int prevChar = 0;
    int wordCnt = 0;
    while (*ch != EOF) {
        char newRead = (char) toupper(fgetc(dict));
        ch = &newRead;
        if ((*ch == ' ' || *ch == '\n' || *ch == '\r' || *ch == '\t')) prevChar = 0;
        else {
            if (prevChar == 0 && *ch == letter) wordCnt++;
            prevChar = 1;
        }
    }
    fclose(dict);
    return wordCnt;
}

ErrorCode spell_check(const char* dictionary, const char* word) { //Given the file name of a dictionary text file and a word, looks up the word in the dictionary
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL) return FILE_ERR_OPEN;
    char str[46]; //longest english word is 45 words. +1 for \0 char

    while(fgets(str, 46, dict) != NULL) {
        str[strcspn(str, "\n")] = 0; //gets rid of newline char
        int i = 0;
        int legal = 1;
        while (str[i++] != '\0') if (toupper(word[i]) != toupper(str[i])) legal = 0;
        if (legal) return WORD_OK;
    }
    return WORD_BAD;
}

ErrorCode word_lengths(const char* dictionary, WordCount lengths[], WordLength count) { //Given the file name of a dictionary text file, counts the number of words of each length between 1 and count (inclusive) and stores this result in an array lengths at the position corresponding to the length
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL) return FILE_ERR_OPEN;
    char str[46]; //longest english word is 45 words. +1 for \0 char

    while(fgets(str, 46, dict) != NULL) {
        str[strcspn(str, "\n")] = 0; //gets rid of newline char
        int strLength = strlen(str);
        if (strLength <= count) lengths[strLength]++;
    }
    return FILE_OK;
}

ErrorCode info(const char* dictionary, DictionaryInfo* info) { //Given the file name of a dictionary text file, returns its description (the length of the shortest and the longest words, and the count of all words) using the DictionaryInfo structure
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL) return FILE_ERR_OPEN;
    char str[46]; //longest english word is 45 words. +1 for \0 char
    info->longest = 0;
    info->shortest = 45;

    info->count = 0;
    while(fgets(str, 46, dict) != NULL) {
        str[strcspn(str, "\n")] = 0; //gets rid of newline char
        int strLength = strlen(str);
        info->count++;
        if (info->longest < strLength) info->longest = strLength;
        if (info->shortest > strLength) info->shortest = strLength;
    }
    return FILE_OK;
}
