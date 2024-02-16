// USER MODIFIABLE

#define NEW_DB_FN "../new_db.csv"

// DO NOT MODIFY PAST HERE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNAME_LEN 30
#define INSTANCE_LEN 410
#define KEY_LEN 10
#define FIELDS_LEN 100
#define MENTEE_DB_FN "../databases/mentee_db.csv"
#define MENTOR_DB_FN "../databases/mentor_db.csv"
#define PAIRS_DB_FN "../databases/pairs_db.csv"
#define SEP ",\n"

int check_format();
int search_db_from_data(FILE* database, char* name, char* surname, int* is_in_database);
void add_person_entry(FILE* database, char* name, char* surname, int id);
void add_pair_entry(FILE* pairs_db, int id_mentor, int id_mentee, int year);

int main(){
	// decl
	FILE *mentor_db, *mentee_db, *pairs_db, *new_db;
	char instance[INSTANCE_LEN];
	char mentor_name[FIELDS_LEN], mentor_surname[FIELDS_LEN], mentee_name[FIELDS_LEN], mentee_surname[FIELDS_LEN];
	int year, is_in_database, mentor_key, mentee_key;

	// init
	mentor_db = fopen(MENTOR_DB_FN, "a+");
	mentee_db = fopen(MENTEE_DB_FN, "a+");
	pairs_db = fopen(PAIRS_DB_FN, "a+");
	new_db = fopen(NEW_DB_FN, "r");

	// bad input handling
	if(check_format() == 0) return 0;

	/*
	legge il database new.csv riga per riga e popola gli altri tre (dove pairs viene sempre
	popolato, mentre gli altri due vengono popolati solo se la entry è nuova).
	*/
	while(fgets(instance, INSTANCE_LEN, new_db)){

		// dopo aver letto la riga, popola i parametri di lavoro
		strcpy(mentor_name, strtok(instance,SEP));
		strcpy(mentor_surname, strtok(NULL,SEP));
		strcpy(mentee_name, strtok(NULL,SEP));
		strcpy(mentee_surname, strtok(NULL,SEP));
		year = atoi(strtok(NULL, SEP));

		// check & add su mentor_db
		mentor_key = search_db(mentor_db, mentor_name, mentor_surname, &is_in_database);
		if(!is_in_database) add_person_entry(mentor_db, mentor_name, mentor_surname, mentor_key);
		// check & add su mentee_db
		mentee_key = search_db(mentee_db, mentee_name, mentee_surname, &is_in_database);
		if(!is_in_database) add_person_entry(mentee_db, mentee_name, mentee_surname, mentee_key);
		// check & add su pairs_db
		add_pair_entry(pairs_db, mentor_key, mentee_key, year);		
	}

	// tidy up
	fclose(mentor_db);
	fclose(mentee_db);
	fclose(pairs_db);
	fclose(new_db);
	return 0;
}

int check_format(){
	char choice;

	printf("\
>> Hello. Please, before continuing make sure that the new database is a csv file that contains\n\
   instances of the type 'nome_mentor,cognome_mentor,nome_mentee,cognome_mentee,anno'.\n\
   If that is the case, please press 'Y' to confirm, else terminate.\n");
	scanf("%c", &choice);
	printf("\n");

	if(choice == 'Y') return 1;
	return 0;
}
int search_db_from_data(FILE* database, char* name, char* surname, int* is_in_database){

	/*
	Format of database:
	key,name,surname
	
	The field name in input and that of the database must be the exact same, for a match
	*/

	char instance[INSTANCE_LEN], key_string[KEY_LEN];
	int instance_i, field_i;
	int curr_key, new_key, found_entry;

	new_key = 0;
	found_entry = 0;
	rewind(database);

	while(fgets(instance, INSTANCE_LEN, database) && !found_entry){
		
		strcpy(key_string,strtok(instance,SEP));
		curr_key = atoi(key_string);
		if (new_key < curr_key)
			new_key = curr_key;

		if(strcmp(name, strtok(NULL,SEP)) == 0
		&& strcmp(surname, strtok(NULL,SEP)) == 0)
			found_entry = 1;

		// here was snippet_1
	}
	
	*is_in_database = found_entry;
	if(found_entry) 
		return curr_key;
	return new_key + 1;
}
void add_person_entry(FILE* database, char* name, char* surname, int id){
	fprintf(database,"%d,%s,%s\n",id,name,surname);
}
void add_pair_entry(FILE* pairs_db, int id_mentor, int id_mentee, int year){
	fprintf(pairs_db,"%d,%d,%d\n",id_mentor,id_mentee,year);
}