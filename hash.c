#include "hash.h"

void print_rec(Record* rec){
	if(rec){
		printf("Word: \"%s\" File: %s  Count: %d\n", rec->word, rec->file_path, rec->count);
		return;
	}
	printf("Record is NULL\n");
	return;
}

/*Returns an initialzed hash table with 2000 bins*/
Hash* ht_create(){
	int i;
	Hash* new = malloc(sizeof(Hash));
	new->size = 0;
	for( i = 0; i < 2000; i++){
		new->table[i] = NULL;
	}
	return new;
}

/*The djb2 hash function: used to hash strings*/
/*Which is actually giving me */
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

/*adds a key value pair to the hash table*/
int ht_add(Hash* hash_table, char* word, const char* file_path){

	char hashstring [strlen(word) + strlen(file_path) + 1];
	sprintf(hashstring, "%s%s", word, file_path);
	
	unsigned long index = hash_function(hashstring);
	/*create a new initial record if none are found*/
	if(!hash_table->table[index]){
		Record* new = malloc(sizeof(Record));

		new->word = malloc(strlen(word)+1);
		strcpy(new->word,word);
		
		new->file_path = malloc(strlen(file_path)+1);
		strcpy(new->file_path,file_path);

		new->hash_string = malloc(strlen(hashstring)+1);
		strcpy(new->hash_string, hashstring);

		new->count = 1;
		new->next = NULL;

		hash_table->table[index] = new;
		hash_table->size++;

		return 1;
	}
	else{
		Record* rec = hash_table->table[index];
		while(rec){
			/*if a matching record is found, update the file count*/
			if(strcmp(rec->hash_string,hashstring) == 0){
				rec->count++;
				return 1;
			}
			rec = rec->next;
		}
		/*if no match found in existing bucket, create a new node and add to front*/
		Record* new = malloc(sizeof(Record));

		new->word = malloc(strlen(word)+1);
		strcpy(new->word,word);

		new->file_path = malloc(strlen(file_path)+1);
		strcpy(new->file_path,file_path);

		new->hash_string = malloc(strlen(hashstring)+1);
		strcpy(new->hash_string, hashstring);

		new->count = 1;
		new->next = hash_table->table[index];

		hash_table->table[index] = new;
		hash_table->size++;

		return 1;
	}
}

/*Searches for the record, using the key. Ruturns NULL if not found*/
Record* ht_search(Hash* hash_table, char* word, const char* file_path){
	char hashstring[strlen(word) + strlen(file_path) + 1];
	sprintf(hashstring, "%s%s", word, file_path);

	unsigned long index = hash_function(hashstring);
	if(hash_table->table[index]){
		Record* rec = hash_table->table[index];
		while(rec){
			if(strcmp(rec->hash_string,hashstring)==0) return rec;
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
			free(delete_rec->word);
			free(delete_rec->file_path);
			free(delete_rec->hash_string);
			free(delete_rec);
		}
	}
	return;
}

Record** hash_pull(Hash* ht){
	Record** rec_array = malloc((ht->size)*sizeof(Record*));
	int i;
	int r;
	r = 0;
	Record* rec;
	for(i = 0; i < 2000; i++){
		if(ht->table[i]){
			rec = ht->table[i];
			while(rec){
				rec_array[r] = rec;
				r++;
				rec = rec->next;
			}
		}
	}
	return rec_array;
}

// Compares 2 records by hash_string
int rec_compare(const void* vrec1, const void* vrec2){
	Record* rec1 = *(Record* const*)(vrec1);
	Record* rec2 = *(Record* const*)(vrec2);
	if(strcmp(rec1->word, rec2->word) == 0){
		if(rec1->count - rec2->count == 0){
			return strcmp(rec1->file_path, rec2->file_path);
		}
		else if(rec1->count - rec2->count < 1) return 1;
		else return -1;
	}
	return strcmp(rec1->word, rec2->word);
}
