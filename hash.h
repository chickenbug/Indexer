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

void print_rec(Record* rec);

Hash* ht_create();

unsigned long hash(unsigned char *str);

unsigned long hash_function(char* hashstring);

int ht_add(Hash* hash_table, char* word, const char* file_path);

Record* ht_search(Hash* hash_table, char* word, const char* file_path);

void ht_free(Hash* hash_table);

Record** hash_pull(Hash* ht);

int rec_compare(const void* vrec1, const void* vrec2);

#endif