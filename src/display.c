#define CHECK_DB_FN "../check_db.csv"

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

int menu();
void request_anagraphic(char* name, char* surname);
void print_mentees(FILE* pairs_db, FILE* mentee_db, int index);
void print_mentors(FILE* pairs_db, FILE* mentor_db, int index);
void print_check(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db);
int search_db_from_data(FILE* database, char* name, char* surname, int* is_in_database);
int exists_pair(FILE* database, int mentee_key, int mentor_key);
void find_mentees_of_mentor(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db);
void find_mentors_of_mentee(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db);

int main(){
	// decl
	FILE *mentor_db, *mentee_db, *pairs_db;
	int choice, index;
	char name[FIELDS_LEN],surname[FIELDS_LEN];

	// init
	mentor_db = fopen(MENTOR_DB_FN,"a+");
	mentee_db = fopen(MENTEE_DB_FN,"a+");
	pairs_db = fopen(PAIRS_DB_FN,"a+");

	while((choice=menu()) != 0){
		switch(choice){
			case 1:
				find_mentees_of_mentor(mentor_db,mentee_db,pairs_db);
				break;		
			case 2:
				find_mentors_of_mentee(mentor_db,mentee_db,pairs_db);
				break;
			case 3:
				print_check(mentor_db,mentee_db,pairs_db);
				break;
			default:
				printf("bad choice. you may only insert '1','2', or '3'.");
		}
	}

	// tidy up
	fclose(mentor_db);
	fclose(mentee_db);
	fclose(pairs_db);

	return 0;
}

int exists_pair(FILE* database, int mentee_key, int mentor_key){
	char instance[INSTANCE_LEN];

	rewind(database);
	fgets(instance, INSTANCE_LEN, database); // header
	while(fgets(instance, INSTANCE_LEN, database)){		
		if(mentor_key == atoi(strtok(instance, SEP)) 
		&& mentee_key == atoi(strtok(NULL, SEP)))
			return 1;
	}
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
	fgets(instance, INSTANCE_LEN, database); // header
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

void print_from_key(FILE* database, int key){
	char instance[INSTANCE_LEN];

	rewind(database);
	fgets(instance, INSTANCE_LEN, database); // header
	while(fgets(instance, INSTANCE_LEN, database)){
		if(key == atoi(strtok(instance, SEP))){
			printf("%s ", strtok(NULL, SEP));
			printf("%s\n", strtok(NULL, SEP));
			return;
		}	
	}	
}

int menu(){
	int choice;
	printf("\n\
>> Hello. There are 3 ways of accessing the data. You can\n\
  1. Ask me about a specific mentor, and I will display their mentees\n\
  2. Ask me about a specific mentee, and I will display their mentor\n\
  3. Provide me a file and I will tell you whether any repetition with the past editions arises.\n\
  0. Terminate program\n\
---\n\
>> Digit the associated number and press enter to select..."); scanf("%d",&choice);
	printf("\n");
	return choice;
}

void request_anagraphic(char* name, char* surname){
	printf("\
>> Provide their name. If you are not sure about how we memorized it,\n\
   do a quick Ctrl+F search in the proper database (see documentation).\n\
>> Please provide the first name (LOWERCASE ONLY)..."); 
	scanf("%s", name);
	printf(">> And now the surname (LOWERCASE ONLY)..."); 
	scanf("%s", surname);
	printf("\n---\n");	
}

void print_mentees(FILE* pairs_db, FILE* mentee_db, int mentor_key){
	int curr_mentee_key, year;
	char instance[INSTANCE_LEN];

	rewind(pairs_db);
	fgets(instance, INSTANCE_LEN, pairs_db); // header
	while(fgets(instance, INSTANCE_LEN, pairs_db)){
		if(mentor_key == atoi(strtok(instance, SEP))){
			curr_mentee_key = atoi(strtok(NULL, SEP));
			year = atoi(strtok(NULL, SEP));
			printf(" - Year: %4d, ", year);
			print_from_key(mentee_db, curr_mentee_key);
		}
	}
}

void print_mentors(FILE* pairs_db, FILE* mentor_db, int mentee_key){
	int curr_mentor_key, year;
	char instance[INSTANCE_LEN];

	rewind(pairs_db);
	fgets(instance, INSTANCE_LEN, pairs_db); // header
	while(fgets(instance, INSTANCE_LEN, pairs_db)){
		curr_mentor_key = atoi(strtok(instance, SEP));
		if(mentee_key == atoi(strtok(NULL, SEP))){
			year = atoi(strtok(NULL, SEP));
			printf(" - Year: %4d, ", year);
			print_from_key(mentor_db, curr_mentor_key);
		}
	}
}

void find_mentees_of_mentor(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db){
	int mentor_index, is_in_database;
	char name[FIELDS_LEN],surname[FIELDS_LEN];

	printf(">> You want to find the mentees of a mentor.\n");
	request_anagraphic(name,surname);
	mentor_index = search_db_from_data(mentor_db,name,surname,&is_in_database);
	printf(">> You asked for mentor N.%d: %s %s.\n",mentor_index,name,surname);
	if(is_in_database){
		printf(">> These are their mentees:\n");
		print_mentees(pairs_db,mentee_db,mentor_index);
	}else{
		printf(">> This mentor does not seem to be registered.");
	}
	printf("---\n");
}

void find_mentors_of_mentee(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db){
	int mentee_index, is_in_database;
	char name[FIELDS_LEN],surname[FIELDS_LEN];

	printf(">> You want to find the mentors of a mentee.\n");
	request_anagraphic(name,surname);
	mentee_index = search_db_from_data(mentee_db,name,surname,&is_in_database);
	printf(">> You asked for mentee N.%4d: %s %s.\n",mentee_index,name,surname);
	if(is_in_database){
		printf(">> These are their mentors:\n");
		print_mentors(pairs_db,mentor_db,mentee_index);
	}else{
		printf(">> This mentee does not seem to be registered.");
	}
	printf("---\n");
}

void print_check(FILE* mentor_db, FILE* mentee_db, FILE* pairs_db){
	FILE *check_db;
	char instance[INSTANCE_LEN];
	char mentor_name[FIELDS_LEN], mentor_surname[FIELDS_LEN], mentee_name[FIELDS_LEN], mentee_surname[FIELDS_LEN];
	int mentee_key, mentor_key, is_in_database;

	check_db = fopen(CHECK_DB_FN,"r");
	printf("\n---\n");
	fgets(instance, INSTANCE_LEN, check_db); // header
	while(fgets(instance,INSTANCE_LEN,check_db) != NULL){
		// dopo aver letto la riga, popola i parametri di lavoro
		strcpy(mentor_name, strtok(instance,SEP));
		strcpy(mentor_surname, strtok(NULL,SEP));
		strcpy(mentee_name, strtok(NULL,SEP));
		strcpy(mentee_surname, strtok(NULL,SEP));

		// controlla che entrambi, mentee e mentor, siano già presenti. Altrimenti almeno uno dei due è nuovo e puoi passare al prossimo controllo
		mentee_key=search_db_from_data(mentee_db,mentee_name,mentee_surname,&is_in_database);
		if(!is_in_database) continue;
		mentor_key=search_db_from_data(mentor_db,mentor_name,mentor_surname,&is_in_database);
		if(!is_in_database) continue;

		// se esiste già stessa coppia, stampa messaggio di avviso e continua.
		if(exists_pair(pairs_db,mentee_key,mentor_key)){
			printf("ACHTUNG: repetition found. mentee: %s %s, mentor: %s %s\n",
				mentee_name,mentee_surname,mentor_name,mentor_surname);
		}	
	}
	fclose(check_db);
	printf("\n---\n");
}


// snippet_1
/*
instance_i = 0;
found_entry = 1;

while(instance[instance_i] != ','){
	key_string[instance_i] = instance[instance_i];
	++instance_i;
}
key_string[instance_i] = '\0';
++instance_i;

field_i = 0;
while(found_entry && instance[instance_i] != ',' && name[field_i] != '\0'){
	if(instance[instance_i] != name[field_i]){
		found_entry = 0;
	}
	++instance_i;
	++field_i;
}
while(found_entry && instance[instance_i] != ',') 
	++instance_i;
++instance_i;

field_i = 0;
while(found_entry && instance[instance_i] != '\n' && surname[field_i] != '\0'){
	if(instance[instance_i] != surname[field_i]){
		found_entry = 0;
	}
	++instance_i;
	++field_i;
}
*/