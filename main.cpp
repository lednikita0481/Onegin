#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>

char ** get_strings_from_file(int * poem_size);
char ** poem_sort(char ** poem, int poem_size);
char* read_line(FILE* file);
int StrCmp(const char * s1, const char * s2);
void File_Output(char ** poem, int poem_size);
void Cleaning(char *** poem, int poem_size);


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    int poem_size = 0;
    char ** poem = get_strings_from_file(&poem_size);
    poem = poem_sort(poem, poem_size);

    /*
    for (int j = 0; j < poem_size; j++)
    {
        printf("%d %s", j, poem[j]);
    }
    */

    File_Output(poem, poem_size);
    Cleaning(&poem, poem_size);
    return 0;
}


char ** get_strings_from_file(int * poem_size)
{
    char file_name[] = "avidreaders.ru__evgeniy-onegin.txt";
    FILE *file = fopen(file_name, "r");

    size_t Strings = 1000;
    char ** poem = (char **)calloc(Strings, sizeof(char*));

    char* string;

    while ((string = read_line(file)) != NULL)
    {
        printf("%s", string);

        if (string[0] != '\n')
        {
            poem[*poem_size] = string;

            (*poem_size)++;
        }

        if (*(poem_size)>=Strings)
        {
            poem = (char **) realloc(poem, (Strings+100)*sizeof(char*));
            Strings += 100;
        }
    }
    
    fclose(file);
    return poem;
}

char ** poem_sort(char ** poem, int poem_size)
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

char* read_line(FILE* file)
{
    const size_t DEFAULT_SIZE = 100;
    char* string = (char*)calloc(DEFAULT_SIZE, sizeof(char));

    int current_size = 0;
    int max_current_size = DEFAULT_SIZE-1;
    int c = 0;
    while((c = fgetc(file)) != '\n' && c != EOF)
    {
        *(string+current_size) = c;

        current_size++;
        if (current_size == max_current_size)
        {
            max_current_size += 10;
            string = (char*)realloc(string, (max_current_size+1) * sizeof(char));
        }
    }

    if (current_size == 0 && c == EOF) return NULL;

    string[current_size] = '\n';
    return string;
}

int StrCmp(const char * s1, const char * s2)
{
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

void File_Output(char ** poem, int poem_size)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(file_name, "w");

    for (int i = 0; i < poem_size; i++)
    {
        fputs(*(poem + i), file);
    }

    fclose(file);
}

void Cleaning(char *** poem, int poem_size)
{
    for (int i = 0; i < poem_size; i++)
    {
        free(**(poem + i));
    }
    free(*poem);
}