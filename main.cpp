#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>

#include "main.h"

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

    size_t poem_size = 0;
    struct line * poem = get_strings_from_file(&poem_size);

    char file_name[] = "Output.txt";
    FILE* file = fopen(file_name, "w");
    fclose(file);

    assert(poem!=NULL);

    qsort(poem, poem_size, sizeof(struct line), StrCmp);
    File_Output(poem, poem_size);

    my_sort(poem, poem_size, sizeof(struct line), ReversedStrCmp);
    File_Output(poem, poem_size);

    /*
    fopen(file_name, "a");
    fwrite(text, sizeof(char), text_size, file);
    fclose(file);
    */
    Origin_text_output(text, text_size);

    Cleaning(&poem, poem_size);
    return 0;
}


struct line * get_strings_from_file(size_t * poem_size)
{
    char file_name[] = "Onegin.txt";
    FILE *file = fopen(file_name, "rb");

    size_t Strings = 2000;
    struct line * poem = (struct line *)calloc(Strings, sizeof(struct line));


    fseek(file, 0L, SEEK_END);
    text_size = ftell(file);
    rewind(file);

    text = (char*)calloc(text_size, sizeof(char));

    long g = fread(text, sizeof(char), text_size, file);
    printf("%ld\n", g);

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
    printf("я вышел из вз€ти€ строк");

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
            //printf("%d %d %d\n", i, j, number);

            if ((*comparator)((void*)(first+((j+1)*size)), (void*)(first+(size*j))) < 0)
            {
                char *Temp = (char *)calloc(size, sizeof(char));
                for (int k = 0; k < size; k++)
                {
                    Temp[k] = *(first + (j*size) + k);
                }

                for (int k = 0; k < size; k++)
                {
                    *(first + (size*j) + k) = *(first + (j + 1)*size + k);
                }

                for (int k = 0; k < size; k++)
                {
                    *(first + size*(j + 1) + k) = Temp[k];
                }

                free(Temp);
            }
        }
    }
    printf(" ажетс€ € отсортировал\n");
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
        if (!isalpha(*s1down))
        {
            while (!isalpha(*s1down) && (s1down <= s1up)) s1down++;
        }

        s2down++;
        if (!isalpha(*s2down))
        {
            while (!isalpha(*s2down) && (s2down <= s2up)) s2down++;
        }
    }
    return *s1down - *s2down;
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
        if (!isalpha(*s1up))
        {
            while (!isalpha(*s1up) && s1up >= s1down) s1up--;
        }

        s2up--;
        if (!isalpha(*s2up))
        {
            while (!isalpha(*s2up) && s2up >= s2down) s2up--;
        }
    }
    return *s1up - *s2up;
}

int isalpha(char a)
{
    if ((a >= -64 && a <= -1) ||
        (a >=  65 && a <= 90) ||
        (a >= 97 && a <= 122)) return 1;
    return 0;
}

void File_Output(struct line * poem, size_t poem_size)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(file_name, "a");

    for (int i = 0; i < poem_size; i++)
    {
        fputs((poem + i)->line_begin, file);
        fputc('\n', file);
    }

    fputc('\n', file);

    fclose(file);
}

void Origin_text_output(char* text, long text_size)
{
    char file_name[] = "Output.txt";
    FILE* file = fopen(file_name, "a");

    for (long i = 0; i < text_size - 1; i++)
    {
        if (text[i] == '\0') text[i] = '\n';
    }

    fprintf(file, text);
}

void Cleaning(struct line ** poem, size_t poem_size)
{
    free(*poem);
}
