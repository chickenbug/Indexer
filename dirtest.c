#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef USE_FDS
#define USE_FDS 15
#endif

int print_entry(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo)
{
    if (typeflag == FTW_F)
        printf(" %s\n", filepath);
    if (typeflag == FTW_D || typeflag == FTW_DP)
        printf(" %s/\n", filepath);

    return 0;
}


int print_directory_tree(const char *const dirpath)
{
    int result;

    if (dirpath == NULL || *dirpath == '\0')
        return errno = EINVAL;

    result = nftw(dirpath, print_entry, USE_FDS, FTW_PHYS);
    if (result >= 0)
        errno = result;

    return errno;
}

int get_file_info(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){
    FILE* fp;
    FILE* opf;
    char word[1024];
    char* begin;
    char* end;
    char* p;
    // printf("About to check typeflag\n");
    if(typeflag == FTW_F){
        printf(" %s is a file\n", filepath);
        fp = fopen(filepath, "r");
        if(fp==NULL){
            printf("Input file is null\n");
        }
        opf = fopen("output.txt","w");
        while (fscanf(fp, "%s", word)!=EOF) {
            printf("Testing %s for word type\n", word);
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
                
                printf("word_to_check is %s\n", word_to_check);
                // matchStr(word_to_check);
                begin = ++end; 
            }    
        }        
        fclose(fp);
        fclose(opf);
    
  }

    return 0;
}

int get_Files(const char *const dirpath){
    int result;

    if (dirpath == NULL || *dirpath == '\0')
        return errno = EINVAL;
    // printf("About to enter get_file_info()\n");
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

    } else {
        if (print_directory_tree(argv[2])) {
            fprintf(stderr, "%s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        // printf("About to enter get_Files()\n");
        get_Files(argv[2]);
    }

    return EXIT_SUCCESS;
}