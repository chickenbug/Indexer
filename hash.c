
Hash* ht_create(){
	int i;

	Hash* new = malloc(sizeof(Hash));
	new->size = 0;
	for( i = 0; i < 2000; i++){
		table[i] = NULL;
	}
	return new;
}

// using djb2 hash function
unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int ht_add(Hash* hash_table, char* file_path, char* word){
	int index = hash(word) % 2000;

	if(!hash_table->table[index]){
		Record* new = table[index];
		new = malloc(sizeof(Record));
		new->word = word;
		new->next = NULL;

		new->files = malloc(sizeof(FileAndCount));
		new->files->path = file_path;
		new->files->count = 1;
		new->files->next = NULL;
		printf("Inserted new word\n");
		return 1;
	}
	else{
		Record* rec = hash_table->table[index];
		while(rec){
			if(rec->word == word){
				FileAndCount* fac = rec->files;
				while(fac){
					if(fac->path == file+path){
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
		printf("bad JUJU\n");
		return 0;
	}
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
