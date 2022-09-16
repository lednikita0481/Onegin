#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>

struct line * get_strings_from_file(size_t * poem_size);
struct line * poem_sort(struct line * poem, size_t poem_size);
char* read_line(FILE* file);
int StrCmp(const void * s1, const void * s2);
int ReversedStrCmp(const void * S1, const void * S2);
void File_Output(struct line * poem, size_t poem_size);
void Cleaning(struct line ** poem, size_t poem_size);

struct line
{
    const char* line_begin;
    const char* line_end;
    int lenth;
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
    //poem = poem_sort(poem, poem_size);
    File_Output(poem, poem_size);

    qsort(poem, poem_size, sizeof(struct line), ReversedStrCmp);
    File_Output(poem, poem_size);

    Cleaning(&poem, poem_size);
    return 0;
}


struct line * get_strings_from_file(size_t * poem_size)
{
    char file_name[] = "Onegin.txt";
    FILE *file = fopen(file_name, "r");

    size_t Strings = 2000;
    struct line * poem = (struct line *)calloc(Strings, sizeof(struct line));


    fseek(file, 0L, SEEK_END);
    long text_size = ftell(file);
    rewind(file);

    char* text = (char*)calloc(text_size, sizeof(char));

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
            poem[string].lenth = poem[string].line_end - poem[string].line_end;

            text[symbol] = '\0';

            while (text[symbol+1]=='\n') symbol++;

            string++;
            poem[string].line_begin = text + symbol + 1;

            if (string == Strings-1)
            {
                Strings *= 2;
                printf("%zd\n", sizeof(line));
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

struct line * poem_sort(struct line * poem, size_t poem_size)
{
    for (int i = 1; i < poem_size; i++)
    {
        for (int j = 0; j < poem_size - i; j++)
        {
            if (StrCmp( (poem+j+1)->line_begin, (poem+j)->line_begin) < 0)
            {
                struct line temp = *(poem+j);
                *(poem+j) = *(poem+j+1);
                *(poem+j+1) = temp;
            }
        }
    }

    return poem;
}

int StrCmp(const void * S1, const void * S2)
{
    const char * s1 = ((const struct line *)S1)->line_begin;
    const char * s2 = ((const struct line *)S2)->line_begin;

    while (*s1 == '\"' || *s1 == 'Ђ' || *s1 == 'Ц' || *s1 == ' ') s1++;
    while (*s2 == '\"' || *s2 == 'Ђ' || *s2 == 'Ц' || *s2 == ' ') s2++;

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

int ReversedStrCmp(const void * S1, const void * S2)
{
    const char * s1up = ((const struct line *)S1)->line_end-1;
    const char * s2up = ((const struct line *)S2)->line_end-1;
    const char * s1down = ((const struct line *)S1)->line_begin;
    const char * s2down = ((const struct line *)S2)->line_begin;

    while ((*s1up == '\"' || *s1up == 'Ђ' || *s1up == 'Ц' || *s1up == ' ') && s1up >= s1down) s1up--;
    while ((*s2up == '\"' || *s2up == 'Ђ' || *s2up == 'Ц' || *s2up == ' ') && s2up >= s2down) s2up--;

    while ((*s1up == *s2up) && (s1up > s1down) && (s2up > s2down))
    {
        s1up--;
        s2up--;
    }
    return *s1up - *s2up;
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

void Cleaning(struct line ** poem, size_t poem_size)
{
    /*
    for (int i = 0; i < poem_size; i++)
    {
        free(**(poem + i));
    }
    */

    free(*poem);
}
