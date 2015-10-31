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
        // opf = fopen("output.txt","w");
        while (fscanf(fp, "%s", word)!=EOF) {
            begin = word;
            if(!isalpha(*begin)){
                begin++;
            }
            while(*begin != '\0'){
                while(!isalnum(*begin) && *begin != '\0') begin++;
                if(*begin == '\0') break;
                end = begin;

                while(isalnum(*(end+1))){
                    end++;
                }
                char word_to_check[end - begin + 2];
                word_to_check[end - begin + 1] ='\0';
                strncpy(word_to_check, begin, end - begin + 1 );
                for(p = word_to_check; *p; p++) *p = tolower(*p);
                
            	ht_add(ht, word_to_check, filepath);
                // matchStr(word_to_check);
                begin = ++end; 
            }    
        }
        
        fclose(fp);
        // fclose(opf);
    
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
    
    int i;
    for(i = 0; i < ht->size; i++){
        print_rec(rec_array[i]);
    }
    ht_free(ht);
    return EXIT_SUCCESS;
}