struct line * get_strings_from_file(size_t * poem_size);

struct line * poem_sort(struct line * poem, size_t poem_size);

void my_sort(void * First, size_t number, size_t size, int (* comparator)(const void * s1, const void * s2));

char* read_line(FILE* file);

int StrCmp(const void * s1, const void * s2);

int ReversedStrCmp(const void * S1, const void * S2);

int isalpha(char a);

void File_Output(struct line * poem, size_t poem_size);

void Origin_text_output(char* text, long text_size);

void Cleaning(struct line ** poem, size_t poem_size);

