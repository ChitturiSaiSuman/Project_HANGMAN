/*
*           HANGMAN - PROJECT BY CHITTURI SAI SUMAN, PRANEETH KAPILA
*              This is a Command line application yet too powerful
*           Implemented Dynamically using Data Structures and Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

#define or ||
#define and &&
#define not !
#define is ==

void newline(int n)
{
    while(n--)
        printf("\n");
}

void tab(int n)
{
    while(n--)
        printf("\t");
}

void clear()
{
    system("clear");
}

char input()
{
    char ch;
    system("/bin/stty raw");
    ch=getchar();
    system("/bin/stty cooked");
    return ch;
}

void printIntroduction()
{
    FILE *file = fopen("Introduction.txt","r");
    char ch;
    while(fscanf(file,"%c",&ch)!=-1)
        printf("%c",ch);
    fclose(file);
}

bool printRules()
{
    printf("Greetings! Let's Begin");
    newline(2);
    FILE *file = fopen("Rules.txt","r");
    char ch;
    while(fscanf(file,"%c",&ch)!=-1)
        printf("%c",ch);
    fclose(file);
    tab(3);
    ch = input();
    return (ch is 'S' or ch is 's');
}

void printConclusion()
{
    FILE *file = fopen("Conclusion.txt","r");
    char ch;
    while(fscanf(file,"%c",&ch)!=-1)
        printf("%c",ch);
    fclose(file);
}

typedef struct Category
{
    int count;
    char *categoryName;
    char **words;
} Category;

Category **category;
int totalCategories;
void load()
{
    DIR * d;
    struct dirent *dir;
    chdir("Words");
    d = opendir(".");
    char currentWorkingDirectory[50];
    getcwd(currentWorkingDirectory,49);
    if(d)
    {
        totalCategories = 0;
        while((dir = readdir(d))!=NULL)
            totalCategories++;
        totalCategories-=2;
        closedir(d);
        category = (Category **)malloc(sizeof(Category*)*totalCategories);
        d = opendir(".");
        int cat = 0;
        while((dir = readdir(d))!=NULL)
        {
            if(strcmp(dir->d_name,".") is 0 or strcmp(dir->d_name,"..") is 0)
                continue;
            char categoryName[50];
            strcpy(categoryName,dir->d_name);
            FILE *file = fopen(categoryName,"r");
            category[cat] = (Category *)malloc(sizeof(Category));
            categoryName[strlen(categoryName)-4] = '\0';
            category[cat]->categoryName = (char *)malloc(sizeof(char)*strlen(categoryName));
            strcpy(category[cat]->categoryName,categoryName);
            fscanf(file,"%d",&category[cat]->count);
            char word[50];
            category[cat]->words = (char **)malloc(sizeof(char*)*category[cat]->count);
            for(int i=0;i<category[cat]->count;i++)
            {
                fscanf(file,"%s",word);
                category[cat]->words[i] = (char *)malloc(sizeof(char)*strlen(word));
                strcpy(category[cat]->words[i],word);
            }
            fclose(file);
            newline(1);
            cat++;
        }
        closedir(d);
    }
    else
    {
        printf("Error Occured!\n");
    }
    chdir("..");
}

void printHangman(int count)
{
    char path[] = "Gallows/0.txt";
    path[8] = count+'0';
    FILE *file = fopen(path,"r");
    char ch;
    while(fscanf(file,"%c",&ch)!=-1)
        printf("%c",ch);
    fclose(file);
}

void startPlaying(int cat)
{
    bool used[26] = {false};
    for(int i=0;i<26;i++)
        used[i] = false;
    char *underscore;
    char *word;
    int index = rand()%(category[cat-1]->count);
    word = (char *)malloc(sizeof(char)*(strlen(category[cat-1]->words[index])+1));
    strcpy(word,category[cat-1]->words[index]);
    int length = strlen(word);
    underscore = (char *)malloc(sizeof(char)*(length+1));
    for(int i=0;i<length;i++)
        underscore[i] = '_';
    underscore[length]='\0';
    int count = 1;
    bool guessed = false;
    clear();
    printHangman(count);
    newline(2);
    tab(2);
    printf("Category: %s",category[cat-1]->categoryName);
    newline(2);
    tab(4);
    for(int i=0;i<length;i++)
        printf("%c ",underscore[i]);
    newline(2);
    tab(4);
    printf("Enter a Character: ");
    while(count<7 and !guessed)
    {
        char ch = input();
        if(ch>='a' and ch<='z')
            ch-=32;
        bool found = false;
        bool alreadyUsed = false;
        if(!used[ch-'A'])
        {
            used[ch-'A'] = true;
            for(int i=0;i<length;i++)
            {
                if(word[i] is ch)
                {
                    underscore[i] = ch;
                    found = true;
                }
            }
            if(!found)
                count++;
        }
        else
        {
            alreadyUsed = true;
        }
        clear();
        printHangman(count);
        newline(2);
        tab(2);
        printf("Category: %s",category[cat-1]->categoryName);
        newline(2);
        tab(4);
        for(int i=0;i<length;i++)
            printf("%c ",underscore[i]);
        tab(2);
        if(alreadyUsed)
            printf("Letter Already Used!");
        else
            printf(found?"Correct :)":"Wrong :(");
        newline(2);
        tab(4);
        printf("Letters Used: ");
        for(int i=0;i<26;i++)
            if(used[i])
                printf("%c ",i+'A');
        newline(2);
        tab(3);
        printf("Number of Chances left: %d",7-count);
        if(count is 7)
        {
            newline(3);
            tab(4);
            printf("You LOSE! :(");
            newline(2);
            tab(4);
            printf("Correct Word: %s",word);
        }
        int remaining = length;
        for(int i=0;i<length;i++)
            remaining -= (underscore[i] is '_'?0:1);
        guessed = (remaining is 0);
    }
    newline(3);
    tab(4);
    if(count<7 and guessed)
        printf("YOU WIN! :)");
    free(underscore);
    free(word);
}

bool play()
{
    newline(2);
    tab(3);
    printf("Select from the Following Categories: ");
    newline(2);
    tab(3);
    printf("0. Random Category");
    newline(2);
    for(int i=0;i<totalCategories;i++)
    {
        tab(3);
        printf("%d. %s",i+1,category[i]->categoryName);
        newline(2);
    }
    tab(3);
    printf("Integral Choice: ");
    char cat = input();
    if(cat is '0')
        cat = rand()%totalCategories+1+'0';
    int current = cat-'0';
    if(current<1 or current>totalCategories)
        printf("Invalid Choice");
    else
        startPlaying(current);
    newline(2);
    tab(3);
    printf("Play Again(Y/N):\t\t");
    char choice = input();
    return (choice is 'Y' or choice is 'y');
}

void delete()
{
    for(int i=0;i<totalCategories;i++)
    {
        for(int j=0;j<category[i]->count;j++)
            free(category[i]->words[j]);
        free(category[i]->words);
        free(category[i]->categoryName);
    }
    free(category);
}

int main()
{
    srand(time(0));
    clear();
    printIntroduction();
    bool flag = printRules();
    if(flag)
    {
        load();
        while(flag)
        {
            clear();
            flag = play();
        }
    }
    clear();
    printConclusion();
    newline(4);
    tab(5);
    printf("Clearing Memory...");
    newline(1);
    delete();
    sleep(10);
    clear();
    clear();
    return 0;
}