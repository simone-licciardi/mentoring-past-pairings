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

int menu();
void gather_name(char* name, char* surname);
int print_mentees(FILE* pairs_db, FILE* mentee_db, int index);
int print_mentors(FILE* pairs_db, FILE* mentor_db, int index);
int search_mentor(FILE* mentor_db, char* name, char* surname, int* found);
int search_mentee(FILE* mentee_db, char* name, char* surname, int* found);

int main(){
	FILE *mentor_db, *mentee_db, *pairs_db;
	int choice, index, found;
	char name[NAME_FIELDS],surname[NAME_FIELDS];

	mentor_db = fopen(MENTOR_DB,"a+");
	mentee_db = fopen(MENTEE_DB,"a+");
	pairs_db = fopen(PAIRS_DB,"a+");

	choice=menu();
	switch(choice){
		case 1:{
			printf("\nYou want to find the mentees of a mentor.\n\n");
			gather_name(name,surname);
			index = search_mentor(mentor_db,name,surname,&found);
			if(!found)
				printf("This mentor does not seem to exist.");
			else
				print_mentees(pairs_db,mentee_db,index);
		}
		break;		
		case 2:{
			printf("\nYou want to find the mentors of a mentee.\n\n");
			gather_name(name,surname);
			index = search_mentee(mentee_db,name,surname,&found);
			if(!found)
				printf("This mentee does not seem to exist.");
			else
				print_mentors(pairs_db,mentor_db,index);
		}
		break;
		case 3:{
			// for another day.
		}
		default:
			printf("bad choice. you may only insert '1','2', or '3'.");
	}

	fclose(mentor_db);
	fclose(mentee_db);
	fclose(pairs_db);

	return 0;
}

int menu(){
	int choice;
	printf("Hello. There are 3 ways of accessing the data. You can\n1. Ask me about a specific mentor, and I will display their mentees\n2. Ask me about a specific mentee, and I will display their mentor\n3. Provide me a file and I will tell you whether any repetition with the past editions arises.\nIn case any of this is of interest, digit the associated number and press enter...");
	scanf("%d",&choice);
	return choice;
}
void gather_name(char* name, char* surname){
	printf("\nYou want to find the mentees of a mentor. If you are not sure about how we memorized his name, do a quick ctrl+f search in the proper database, in this directory. Please provide the first name (LOWERCASE ONLY)..."); 
	scanf("%s",name);
	printf("\nAnd now the surname..."); 
	scanf("%s",surname);	
}
int print_mentees(FILE* pairs_db, FILE* mentee_db, int index){
	int found, pair, year, exit;
	char ins[IST_LEN], compare[IST_LEN];

	rewind(pairs_db);
	while(fgets(ins,IST_LEN,pairs_db) != NULL){
		// troviamo abbinamenti con questo mentor
		if(atoi(strtok(ins,SEP)) == index){
			// 1. salvo dati rimanenti
			pair = atoi(strtok(NULL,SEP));
			year = atoi(strtok(NULL,SEP));
			// 2. inizio frase di return per questo abbinamento
			printf("Year %4d - ",year);
			// 3. ricerca del mentee associato
			rewind(mentee_db);
			exit = 0;
			while(!exit && fgets(compare,IST_LEN,mentee_db) != NULL){
				if(atoi(strtok(compare,SEP)) == pair){
					exit = 1;
					printf("%s ",strtok(NULL,SEP));
					printf("%s\n",strtok(NULL,SEP));
				}	
			}
		}
		// continuo la ricerca con prossimo pairing.
	}
	printf(">>----=0=----<<");
	return 0;
}
int print_mentors(FILE* pairs_db, FILE* mentor_db, int index){
	int found, pair, year, exit;
	char ins[IST_LEN], compare[IST_LEN];

	rewind(pairs_db);
	while(fgets(ins,IST_LEN,pairs_db) != NULL){
		// troviamo abbinamenti con questo mentor
		pair = atoi(strtok(ins,SEP));
		if(atoi(strtok(NULL,SEP)) == index){
			// 1. salvo dati rimanenti
			year = atoi(strtok(NULL,SEP));
			// 2. inizio frase di return per questo abbinamento
			printf("Year %4d - ",year);
			// 3. ricerca del mentee associato
			rewind(mentor_db);
			exit = 0;
			while(!exit && fgets(compare,IST_LEN,mentor_db) != NULL){
				if(atoi(strtok(compare,SEP)) == pair){
					exit = 1;
					printf("%s ",strtok(NULL,SEP));
					printf("%s\n",strtok(NULL,SEP));
				}	
			}
		}
		// continuo la ricerca con prossimo pairing.
	}
	printf(">>----=0=----<<");
	return 0;
}
int search_mentor(FILE* mentor_db, char* name, char* surname, int* found){
	char ins[IST_LEN], id[ID_LEN];
	int i, ni, si, temp, unique;

	unique = 0;
	rewind(mentor_db);
	while(fgets(ins, IST_LEN, mentor_db) != NULL ){
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
int search_mentee(FILE* mentee_db, char* name, char* surname, int* found){
	char ins[IST_LEN], id[ID_LEN];
	int i, ni, si, temp, unique;

	unique = 0;
	rewind(mentee_db);
	while(fgets(ins, IST_LEN, mentee_db) != NULL ){
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