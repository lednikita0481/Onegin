#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>

#include "main.h"

const int FILE_NAME_LENTH = 100;
char* text = nullptr;
long text_size = 0;

struct line
{
    const char* line_begin;
    const char* line_end;
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    char input_file_name[FILE_NAME_LENTH] = {};
    char output_file_name[FILE_NAME_LENTH] = {};
    File_Input_Report code;
    while ((code = files_names_input(input_file_name, output_file_name)) == NULL_FILE_PTR)
    {
        printf("Oops :( This file doesn't exist. Try again pls.\n");
    }


    size_t poem_size = 0;
    struct line * poem = get_strings_from_file(&poem_size, input_file_name);

    qsort(poem, poem_size, sizeof(struct line), StrCmp);
    File_Output(poem, poem_size, output_file_name);

    my_sort(poem, poem_size, sizeof(struct line), ReversedStrCmp);
    File_Output(poem, poem_size, output_file_name);

    Origin_text_output(text, text_size, output_file_name);

    Cleaning(&poem, poem_size);
    return 0;
}


struct line * get_strings_from_file(size_t * poem_size, char* input_file_name)
{
    FILE *file = fopen(input_file_name, "rb");

    size_t Strings = 2000;
    struct line * poem = (struct line *)calloc(Strings, sizeof(struct line));


    fseek(file, 0L, SEEK_END);
    text_size = ftell(file);
    rewind(file);

    text = (char*)calloc(text_size, sizeof(char));

    fread(text, sizeof(char), text_size, file);

    fclose(file);

    poem[0].line_begin = text;
    size_t string = 0;
    int symbol = 0;

    while (symbol < text_size)
    {
        if (text[symbol] == '\n')
        {
            poem[string].line_end = text+symbol;

            text[symbol] = '\0';

            while (text[symbol+1]=='\n') symbol++;

            string++;
            poem[string].line_begin = text + symbol + 1;

            if (string == Strings-1)
            {
                Strings *= 2;
                poem = (struct line *)realloc(poem, sizeof(struct line)*Strings);
                assert(poem != NULL);
            }
        }
        symbol++;
    }

    *poem_size = string;
    return poem;
}


void my_sort(void * First, size_t number, size_t size, int (* comparator)(const void * s1, const void * s2))
{
    char* first = (char*) First;
    for (int i = 1; i < number; i++)
    {
        for (int j = 0; j < number - i; j++)
        {

            if ((*comparator)((void*)(first+((j+1)*size)), (void*)(first+(size*j))) < 0)
            {
                swap((first+(size*j)), (first+((j+1)*size)), size);
            }
        }
    }
}

void swap(char* first, char* second, size_t size)
{
    char *Temp = (char *)calloc(size, sizeof(char));
    for (int k = 0; k < size; k++)
    {
        Temp[k] = *(first + k);
    }

    for (int k = 0; k < size; k++)
    {
        *(first + k) = *(second + k);
    }

    for (int k = 0; k < size; k++)
    {
        *(second + k) = Temp[k];
    }

    free(Temp);
}

int StrCmp(const void * S1, const void * S2)
{
    const char * s1down = ((const struct line *)S1)->line_begin;
    const char * s1up = ((const struct line *)S1)->line_end;
    const char * s2down = ((const struct line *)S2)->line_begin;
    const char * s2up = ((const struct line *)S2)->line_end;


    while (!isalpha(*s1down) && (s1down <= s1up)) s1down++;
    while (!isalpha(*s2down) && (s2down <= s2up)) s2down++;

    while (*s1down == *s2down)
    {
        if (*s1down == '\0') return 0;

        s1down++;
        ptr_increment(&s1down, &s1up);

        s2down++;
        ptr_increment(&s2down, &s2up);
    }
    return *s1down - *s2down;
}

void ptr_increment(const char ** sdown, const char ** sup)
{
    if (!isalpha(**sdown))
    {
        while (!isalpha(**sdown) && (*sdown <= *sup)) (*sdown)++;
    }
}

int ReversedStrCmp(const void * S1, const void * S2)
{
    const char * s1up = ((const struct line *)S1)->line_end-1;
    const char * s2up = ((const struct line *)S2)->line_end-1;
    const char * s1down = ((const struct line *)S1)->line_begin;
    const char * s2down = ((const struct line *)S2)->line_begin;

    while (!isalpha(*s1up) && s1up >= s1down) s1up--;
    while (!isalpha(*s2up) && s2up >= s2down) s2up--;

    while ((*s1up == *s2up) && (s1up > s1down) && (s2up > s2down))
    {
        s1up--;
        /*
        if (!isalpha(*s1up))
        {
            while (!isalpha(*s1up) && s1up >= s1down) s1up--;
        }
        */
        ptr_decrement(&s1up, &s1down);

        s2up--;
        /*
        if (!isalpha(*s2up))
        {
            while (!isalpha(*s2up) && s2up >= s2down) s2up--;
        }
        */
        ptr_decrement(&s2up, &s2down);
    }
    return *s1up - *s2up;
}

void ptr_decrement(const char ** sup, const char ** sdown)
{
    if (!isalpha(**sup))
    {
        while (!isalpha(**sup) && (*sup >= *sdown)) (*sup)--;
    }
}

int isalpha(char a)
{
    if ((a >= -64 && a <= -1) ||
        (a >=  65 && a <= 90) ||
        (a >= 97 && a <= 122)) return 1;
    return 0;
}

enum File_Input_Report files_names_input(char* input_file_name, char * output_file_name)
{
    printf("Please, enter the name of input file with name lenth to 100 symbols\n>");
    fgets(input_file_name, FILE_NAME_LENTH, stdin);
    for (int i = 0; i < FILE_NAME_LENTH; i++)
    {
        if (*(input_file_name+i) == '\n')
        {
            *(input_file_name + i) = '\0';
            break;
        }
    }

    FILE* in_file = fopen(input_file_name, "r");
    if (in_file == NULL)
    {
        return NULL_FILE_PTR;
    }
    fclose(in_file);


    printf("Please, enter the name of output file with name lenth to 100 symbols\n>");
    fgets(output_file_name, FILE_NAME_LENTH, stdin);
    for (int i = 0; i < FILE_NAME_LENTH; i++)
    {
        if (*(output_file_name+i) == '\n')
        {
            *(output_file_name+i) = '\0';
            break;
        }
    }

    FILE* out_file = fopen(output_file_name, "w");
    fclose(out_file);
    return OK;
}

void get_rid_of_slash_n(char* s, int lendth)
{
    for (int i = 0; i < lendth; i++)
    {
        if (*(s+i) == '\n')
        {
            *(s+i) = '\0';
            break;
        }
    }
}

void File_Output(struct line * poem, size_t poem_size, char* output_file_name)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(output_file_name, "a");

    for (int i = 0; i < poem_size; i++)
    {
        fputs((poem + i)->line_begin, file);
        fputc('\n', file);
    }

    fputc('\n', file);

    fclose(file);
}

void Origin_text_output(char* text, long text_size, char * output_file_name)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(output_file_name, "a");

    for (long i = 0; i < text_size - 1; i++)
    {
        if (text[i] == '\0') text[i] = '\n';
    }

    fprintf(file, text);
}

void Cleaning(struct line ** poem, size_t poem_size)
{
    free(*poem);
    free(text);
}
