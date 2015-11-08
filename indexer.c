#include "indexer.h"
#include "hash.c"

Hash *ht;
char* out_path;

int get_file_info(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){
    FILE* fp;
    char word[1024];
    char* begin;
    char* end;
    char* p;
    char yn;
    if(strcmp(filepath,out_path)==0){
        printf("Warning:\nThe ouput file you have chosen is also a data file.  Do you wish to continue and overwrite file at the end? (y or n) ");
        scanf("%c", &yn);
        if(yn == 'n') exit(0);
    }
    if(typeflag == FTW_F){
        fp = fopen(filepath, "r");
        if(fp==NULL){
            printf("Input file is null\n");
        }

        while (fscanf(fp, "%s", word) != EOF) {
            begin = word;
            while(*begin != '\0'){
                while(!isalpha(*begin)&& *begin != '\0') begin++;
                
                if(*begin == '\0') break;

                end = begin;
                while(isalnum(*(end+1))) end++;

                char word_to_check[end - begin + 2];
                word_to_check[end - begin + 1] ='\0';
                strncpy(word_to_check, begin, end - begin + 1 );

                for(p = word_to_check; *p; p++) *p = tolower(*p);
            	ht_add(ht, word_to_check, filepath);
                begin = ++end; 
            }    
        }
        
        fclose(fp);
    
  }
    return 0;
}

int get_Files(const char *const dirpath){
    
    int result;

    if (dirpath == NULL || *dirpath == '\0')
        return errno = EINVAL;
    result = nftw(dirpath, get_file_info, USE_FDS, FTW_PHYS);
    if (result >= 0)
        errno = result;

    return errno;
    
}

void print_json( Record** rec_array){
    char* curr_word;
    Record *curr_record, *next_record;
    int i;

    FILE* fp = fopen(out_path, "w");
    if(!fp) {
         printf("Output file is null\n");
         return;
    }

    fprintf(fp, "{\"list\" : [\n");
    if(rec_array){
        curr_word = rec_array[0]->word;
        curr_record = rec_array[0];
        fprintf(fp, "\t{\"%s\" : [\n", curr_word);

        for(i = 0; i < ht->size - 1; i++){
            curr_record = rec_array[i];
            next_record = rec_array[i + 1];

            if(strcmp(curr_word, next_record->word) == 0){
                fprintf(fp, "\t\t{\"%s\" : %d},\n", curr_record->file_path, curr_record->count);
            }
            else{
                fprintf(fp, "\t\t{\"%s\" : %d}\n", curr_record->file_path, curr_record->count);
                fprintf(fp, "\t]},\n");
                fprintf(fp, "\t{\"%s\" : [\n", next_record->word);
                curr_word = next_record->word;
            }
        }
        curr_record = next_record;
        fprintf(fp, "\t\t{\"%s\" : %d}\n", curr_record->file_path, curr_record->count);
        fprintf(fp, "\t]}\n");
    }
    fprintf(fp, "]}\n");
    fclose(fp);
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {

        printf("Incorrect number of arguments.  Run again\n");
        return EXIT_FAILURE;

    } 
    else {
    	ht = ht_create();
        out_path = argv[1];
        get_Files(argv[2]);
        
    }
    Record** rec_array;
    rec_array = hash_pull(ht);
    qsort(rec_array, ht->size, sizeof(Record*), rec_compare);
    print_json(rec_array);
    ht_free(ht);
    free(rec_array);
    return 0;
}
