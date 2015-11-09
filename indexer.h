#ifndef INDEXER_H
#define INDEXER_H
/*
 * indexer.h
 */

#define _XOPEN_SOURCE 700

#ifndef USE_FDS
#define USE_FDS 15
#endif

#include "hash.h"
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

Hash *ht;
char* out_path;

int get_file_info(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo);

int get_Files(const char *const dirpath);

void print_json( Record** rec_array);

#endif