#include "indexer.h"
#include "hash.c"

Hash *ht;

int get_file_info(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){
    FILE* fp;
    // FILE* opf;
    char word[1024];
    char* begin;
    char* end;
    char* p;
    if(typeflag == FTW_F){
        fp = fopen(filepath, "r");
        if(fp==NULL){
            printf("Input file is null\n");
        }

        while (fscanf(fp, "%s", word) != EOF) {
            begin = word;
            while(*begin != '\0'){
                while(!isalpha(*begin)) begin++;
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

void jsonify( Record** rec_array){
    char* curr_word;
    Record *curr_record, *next_record;
    int i;

    printf("{\"list\" : [\n");
    if(rec_array){
        curr_word = rec_array[0]->word;
        curr_record = rec_array[0];
        printf("\t{\"%s\" : [\n", curr_word);

        for(i = 0; i < ht->size - 1; i++){
            curr_record = rec_array[i];
            next_record = rec_array[i + 1];

            if(strcmp(curr_word, next_record->word) == 0){
                printf("\t\t{\"%s\" : %d},\n", curr_record->file_path, curr_record->count);
            }
            else{
                printf("\t\t{\"%s\" : %d}\n", curr_record->file_path, curr_record->count);
                printf("\t]},\n");
                printf("\t{\"%s\" : [\n", next_record->word);
                curr_word = next_record->word;
            }
        }
        curr_record = next_record;
        printf("\t\t{\"%s\" : %d}\n", curr_record->file_path, curr_record->count);
        printf("\t]}\n");
    }
    printf("]}\n");
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
       
        get_Files(argv[2]);
        
    }

    Record** rec_array;
    rec_array = hash_pull(ht);
    qsort(rec_array, ht->size, sizeof(Record*), rec_compare);
    jsonify(rec_array);

    ht_free(ht);

    return EXIT_SUCCESS;
}
