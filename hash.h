#ifndef HASH_H
#define HASH_H
/*
 * hash.h
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct FileAndCount{
	char* path;
	int count;
	struct FileAndCount* next;
}FileCount;

typedef struct Record{
	char* word;
	FileAndCount** files;
	struct Record* next;
}Record;

typedef struct hashtable {
	int size;
	Record* table[2000];
}Hash;

#endif