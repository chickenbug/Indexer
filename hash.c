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
// Which is actually giving me throuble
unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)){
    	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

unsigned long hash_function(char* hashstring){
	return hash((unsigned char*)(hashstring))%2000;
}

//adds a key value pair to the hash table
int ht_add(Hash* hash_table, char* word, char* file_path){

	char hashstring [strlen(word) + strlen(file_path) + 1];
	sprintf(hashstring, "%s%s", word, file_path);
	
	unsigned long index = hash_function(hashstring);
	// create a new initial record if none are found
	if(!hash_table->table[index]){
		Record* new = malloc(sizeof(Record));
		new->word = word;
		new->hash_string = hashstring;
		new->file_path = file_path;
		new->count = 1;
		new->next = NULL;

		hash_table->table[index] = new;
		printf("Inserted new word to empty bucket\n");
		return 1;
	}
	else{
		Record* rec = hash_table->table[index];
		while(rec){
			// if a matching record is found, update the file count
			if(rec->word == word && rec->file_path == file_path){
				rec->count++;
				printf("increment existing entry\n");
				return 1;
			}
			rec = rec->next;
		}
		// if no match found in existing bucket, create a new node and add to front
		Record* new = malloc(sizeof(Record));
		new->word = word;
		new->file_path = file_path;
		new->hash_string = hashstring;
		new->count = 1;
		new->next = hash_table->table[index];

		hash_table->table[index] = new;

		printf("Inserted new Record to chain\n");
		return 1;
	}
}

//Searches for the record, using the key. Ruturns NULL if not found
Record* ht_search(Hash* hash_table, char* word, char* file_path){
	char hashstring [strlen(word) + strlen(file_path) + 1];
	sprintf(hashstring, "%s%s", word, file_path);

	unsigned long index = hash_function(hashstring);
	if(hash_table->table[index]){
		Record* rec = hash_table->table[index];
		while(rec){
			if(rec->word == word && rec->file_path == file_path) return rec;
			rec = rec->next;
		}
	}
	return NULL;
}

void ht_free(Hash* hash_table){
	int i;
	Record *rec, *delete_rec;
	for( i= 0; i<2000; i++){
		rec = hash_table->table[i];
		while(rec){
			delete_rec = rec;
			rec = rec->next;
			free(delete_rec);
		}
	}
	return;
}

void print_rec(Record* rec){
	if(rec){
		printf("Word: %s File: %s  Count: %d\n", rec->word, rec->file_path, rec->count);
		return;
	}
	printf("Record is NULL\n");
	return;
}

int main(){
	Hash* mh = ht_create();
	ht_add(mh, "a", "baa");
	ht_add(mh, "a", "boo");
	ht_add(mh, "a", "boo");
	Record* rec = ht_search( mh, "a", "baa");
	Record* rec1 = ht_search( mh, "a", "boo");

	print_rec(rec);
	print_rec(rec1);
	return 0;
}