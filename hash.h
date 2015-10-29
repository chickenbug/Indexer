#ifndef HASH_H
#define HASH_H
/*
 * hash.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record{
	char* word;
	char* file_path;
	char* hash_string;

	int count;
	struct Record* next;
}Record;

typedef struct hashtable {
	int size;
	Record* table[2000];
}Hash;

#endif