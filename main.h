enum File_Input_Report
{
    OK,
    NULL_FILE_PTR
};

struct line * get_strings_from_file(size_t * poem_size, char* input_file_name);

struct line * poem_sort(struct line * poem, size_t poem_size);

void my_sort(void * First, size_t number, size_t size, int (* comparator)(const void * s1, const void * s2));

void swap(char* first, char* second, size_t size);

char* read_line(FILE* file);

int StrCmp(const void * s1, const void * s2);

void ptr_increment(const char ** sdown, const char ** sup);

int ReversedStrCmp(const void * S1, const void * S2);

void ptr_decrement(const char ** sup, const char ** sdown);

int isalpha(char a);

enum File_Input_Report files_names_input(char* input_file_name, char * output_file_name);

void File_Output(struct line * poem, size_t poem_size, char* output_file_name);

void Origin_text_output(char* text, long text_size, char * output_file_name);

void Cleaning(struct line ** poem, size_t poem_size);

