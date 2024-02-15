// USER MODIFIABLE

#define NEW_DB "../new.csv"

// DO NOT MODIFY PAST HERE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNAME_LEN 30
#define IST_LEN 410
#define ID_LEN 10
#define NAME_FIELDS 100
#define MENTEE_DB "../databases/mentee_db.csv"
#define MENTOR_DB "../databases/mentor_db.csv"
#define PAIRS_DB "../databases/pairs_db.csv"
#define SEP ",\n"

int check_format();
int search_db(FILE* db, char* name, char* surname, int* found);
void add_person_entry(FILE* db, char* name, char* surname, int id);
void add_pair_entry(FILE* pairs_db, int id_mentor, int id_mentee, int year);

int main(){
	// decl
	FILE *mentor_db, *mentee_db, *pairs_db, *new_db;
	char ins[IST_LEN], *entry;
	char mentor_nm[NAME_FIELDS], mentor_snm[NAME_FIELDS], mentee_nm[NAME_FIELDS], mentee_snm[NAME_FIELDS];
	int year, found, mentor_id, mentee_id;

	// init
	mentor_db = fopen(MENTOR_DB,"a+");
	mentee_db = fopen(MENTEE_DB,"a+");
	pairs_db = fopen(PAIRS_DB,"a+");
	new_db = fopen(NEW_DB,"r");

	// bad input handling
	if(check_format() == 0) return 0;

	/*
	legge il database new.csv riga per riga e popola gli altri tre (dove pairs viene sempre
	popolato, mentre gli altri due vengono popolati solo se la entry è nuova).
	*/
	while(fgets(ins,IST_LEN,new_db) != NULL){

		// dopo aver letto la riga, popola i parametri di lavoro
		strcpy(mentor_nm, strtok(ins,SEP));
		strcpy(mentor_snm, strtok(NULL,SEP));
		strcpy(mentee_nm, strtok(NULL,SEP));
		strcpy(mentee_snm, strtok(NULL,SEP));
		year = atoi(strtok(NULL,SEP));

		// check & add su mentor_db
		mentor_id = search_db(mentor_db,mentor_nm,mentor_snm,&found);
		if(!found) add_person_entry(mentor_db,mentor_nm,mentor_snm,mentor_id);
		// check & add su mentee_db
		mentee_id = search_db(mentee_db,mentee_nm,mentee_snm,&found);
		if(!found) add_person_entry(mentee_db,mentee_nm,mentee_snm,mentee_id);
		// check & add su pairs_db
		add_pair_entry(pairs_db, mentor_id, mentee_id, year);		
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

	printf("Hello. Please, before continuing make sure that the new database is a csv file that contains istances of the type 'nome_mentor,cognome_mentor,nome_mentee,cognome_mentee,anno'. If that is the case, please press 'Y' to confirm, else terminate.\n");
	scanf("%c", &choice);

	if(choice == 'Y') 
		return 1;
	else 
		return 0;
}
int search_db(FILE* db, char* name, char* surname, int* found){
	char ins[IST_LEN], id[ID_LEN];
	int i, ni, si, temp, unique;

	unique = 0;
	rewind(db);
	while(fgets(ins, IST_LEN, db) != NULL ){
		i = 0;
		*found = 1;
		// copio id
		while(ins[i] != ','){
			id[i]=ins[i];
			i++;
		}
		id[i]='\0';
		// check sul nome, in caso passo al next row
		i++;
		ni = 0;
		while(*found && ins[i] != ',' && name[ni] != '\0'){
			if(ins[i]!=name[ni]){
				*found = 0;
			}
			i++;
			ni++;
		}
		// finisco entry per differenze date da secondi nomi
		while(ins[i] != ','){
			i++;
		}
		// check sul cognome, in caso passo al next row
		i++;
		si = 0;
		while(*found && ins[i] != '\n' && surname[si] != '\0'){
			if(ins[i]!=surname[si]){
				*found = 0;
			}
			i++;
			si++;
		}

		temp = atoi(id);
		if(*found) 
			return temp;
		else if (unique < temp)
			unique = temp;
	}
	return unique + 1;
}
void add_person_entry(FILE* db, char* name, char* surname, int id){
	fprintf(db,"%d,%s,%s\n",id,name,surname);
}
void add_pair_entry(FILE* pairs_db, int id_mentor, int id_mentee, int year){
	fprintf(pairs_db,"%d,%d,%d\n",id_mentor,id_mentee,year);
}