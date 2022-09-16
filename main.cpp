#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>

char ** get_strings_from_file(size_t * poem_size);
char ** poem_sort(char ** poem, size_t poem_size);
char* read_line(FILE* file);
int StrCmp(const void * s1, const void * s2);
void File_Output(char ** poem, size_t poem_size);
void Cleaning(char *** poem, size_t poem_size);


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    size_t poem_size = 0;
    char ** poem = get_strings_from_file(&poem_size);

    //qsort(poem, poem_size, sizeof(char*), StrCmp);
    poem = poem_sort(poem, poem_size);

    File_Output(poem, poem_size);
    Cleaning(&poem, poem_size);
    return 0;
}


char ** get_strings_from_file(size_t * poem_size)
{
    char file_name[] = "avidreaders.ru__evgeniy-onegin.txt";
    FILE *file = fopen(file_name, "r");

    size_t Strings = 1000;
    char ** poem = (char **)calloc(Strings, sizeof(char*));

    fseek(file, 0L, SEEK_END);
    long text_size = ftell(file);
    rewind(file);

    char* text = (char*)calloc(text_size, sizeof(char));

    fread(text, sizeof(char), text_size, file);

    fclose(file);
    
    poem[0] = text;
    size_t string = 1;
    int symbol = 0;

    while (symbol < text_size)
    {
        if (text[symbol] == '\n')
        {
            text[symbol] = '\0';

            while (text[symbol+1]=='\n') symbol++;

            poem[string] = text + symbol + 1;
            string++;

            if (string == Strings)
            {
                Strings *= 2;
                poem = (char**)realloc(poem, sizeof(char*)*Strings);
            }
        }
        symbol++;
    }
    
    *poem_size = string;
    return poem;
}

char ** poem_sort(char ** poem, size_t poem_size)
{
    for (int i = 1; i < poem_size; i++)
    {
        for (int j = 0; j < poem_size - i; j++)
        {
            if (StrCmp(*(poem+j+1), *(poem+j)) < 0)
            {
                char * temp = *(poem+j);
                *(poem+j) = *(poem+j+1);
                *(poem+j+1) = temp;
            }
        }
    }

    return poem;
}

int StrCmp(const void * S1, const void * S2)
{
    const char * s1 = (const char*) S1;
    const char * s2 = (const char*) S2;
    while (*s1 == '\"' || *s1 == '«' || *s1 == '–' || *s1 == ' ') s1++;
    while (*s2 == '\"' || *s2 == '«' || *s2 == '–' || *s2 == ' ') s2++;

    while (*s1 == *s2)
    {
        if (*s1 == '\0') return 0;

        s1++;
        if (*s1 == ' ') s1++;

        s2++;
        if (*s2 == ' ') s2++;
    }
    return *s1 - *s2;
}

void File_Output(char ** poem, size_t poem_size)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(file_name, "w");

    for (int i = 0; i < poem_size; i++)
    {
        fputs(*(poem + i), file);
        fputc('\n', file);
    }

    fclose(file);
}

void Cleaning(char *** poem, size_t poem_size)
{
    for (int i = 0; i < poem_size; i++)
    {
        free(**(poem + i));
    }
    free(*poem);
}