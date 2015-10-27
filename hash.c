#include "hash.h"

//Returns an initialzed hash table with 2000 bins
Hash* ht_create(){
	int i;
	Hash* new = malloc(sizeof(Hash));
	new->size = 0;
	for( i = 0; i < 2000; i++){
		new->table[i] = NULL;
	}
	return new;
}

// The djb2 hash function: used to hash strings
unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

//adds a key value pair to the hash table
int ht_add(Hash* hash_table, char* word, char* file_path){
	int index = hash(word) % 2000;
	// create a new initial record if none are found
	if(!hash_table->table[index]){
		Record* new = table[index];
		new = malloc(sizeof(Record));
		new->word = word;
		new->next = NULL;

		new->files = malloc(sizeof(FileAndCount));
		new->files->path = file_path;
		new->files->count = 1;
		new->files->next = NULL;
		printf("Inserted new word to empty\n");
		return 1;
	}
	else{
		Record* rec = hash_table->table[index];
		while(rec){
			// if a matching record is found, update the file count
			if(rec->word == word){
				FileAndCount* fac = rec->files;
				while(fac){
					if(fac->path == file_path){
						fac->count++;
						prinf("Update file count\n");
						return 1;
					}
					fac = fac->next;
				}
				FileAndCount* new_fac = malloc(sizeof(FileAndCount));
				new_fac->path = file_path;
				new_fac->count = 1;
				new_fac->next = rec->files;
				rec->files = new_fac;
				prinf("Add FileAndCount\n");
				return 1;
			}
			rec = rec->next;
		}
		// if no match found in existing bucket, create a new node and add to front
		Record* new = malloc(sizeof(Record));
		new->word = word;
		new->next = table[index];

		new->files = malloc(sizeof(FileAndCount));
		new->files->path = file_path;
		new->files->count = 1;
		new->files->next = NULL;
		table[index] = new;

		printf("Inserted new Record to chain\n");
		return 1;
	}
}

//Searches for the record, using the key. Ruturns NULL if not found
Record* ht_search(Hash* hash_table, char* word){
	int index = hash(word) % 2000;
	if(hash_table->table[index]){
		Record* rec = hash_table->table[index];
		while(rec){
			if(rec->word == word) return rec;
			rec = rec->next;
		}
	}
	return NULL;
}

void ht_free(Hash* hash_table){
	int i;
	Record *rec, *delete_rec;
	FileAndCount *fac, *delete_fac;
	for( i= 0; i<2000; i++){
		rec = hash_table->table[index];
		while(rec){
			fac = rec->files;
			while(fac){
				delete_fac = fac;
				fac = fac->next;
				free(delete);
			}
			delete_rec = rec;
			rec = rec->next;
			free(rec);
		}
	}
	return;
}

void print_rec(Record* rec){
	if(rec){
		printf("Word: %s\n", rec->word);
		FileAndCount fac = rec->files;
		while(fac){
			printf("\t%s :%d\n", fac->path, fac->count);
		}
		return;
	}
	printf("Record is NULL\n");
	return;
}

int main(){
	Hash* mh = ht_create();
	ht_add(mh, "a", "baa");
	ht_add(mh, "a", "boo");
	Record* rec = ht_search("a");
	print_rec(rec);
	return 0;
}