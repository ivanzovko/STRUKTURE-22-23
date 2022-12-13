#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH (256)
#define MAX_NAREDBA (6)
#define PRAZAN_STOG NULL

struct dir;
struct stog;
typedef struct dir* Position;
typedef struct stog* StogPosition;
typedef struct dir {
	char ime[MAX_LENGTH];
	Position child;
	Position sibling;
}Dir;
typedef struct stog {
	Position data;
	StogPosition next;
} Stog;

int IspisMenu();
int UpisCMD(Position p,StogPosition stog);
int Put(Position trenutni, StogPosition stog);
Position NapraviDir(Position q, char* ime_dir);
int Ispis_Dir(Position p);
Position PromijeniDir(Position p, StogPosition stog, char* ime_dir);
Position Povratak(StogPosition stog, Position p);
int PushStog(Position trenutni, StogPosition stog);
Position PopStog(StogPosition stog);
int BrisiStablo(Position p);
int BrisiStog(StogPosition stog);



int main() {
	Dir root = {
		.ime = {0},
		.child = NULL,
		.sibling = NULL
	};
	Stog st = {
		.data = NULL,
		.next = NULL
	};
	IspisMenu();
	UpisCMD(&root,&st);
	return EXIT_SUCCESS;
}


int IspisMenu() {
	printf("\t*********MENU*********");
	printf("\n\tmd - izrada novog direktorija");						//IZRADA DIREKTORIJA
	printf("\n\tcd direktorij - promijena direktorija");	//PROMIJENA DIREKTORIJA
	printf("\n\tcd .. - povratak u prethodni direktorij");		//POVRATAK U PRETHODNI DIREKTORIJ
	printf("\n\tdir - ispis sadrzaja trenutnog direktorija");	//ISPIS SADRZAJA DIREKTORIJA
	printf("\n\texit - izlaz\n");							//IZLAZ
	return EXIT_SUCCESS;
}
int UpisCMD(Position p, StogPosition stog) {
	strcpy(p->ime, "C:");
	Position root = p, trenutni = p;
	StogPosition rootStog = stog;
	char unos[MAX_LENGTH] = { 0 };
	char naredba[MAX_NAREDBA] = { 0 };
	char naziv_dir[MAX_LENGTH] = { 0 };
	do {

		Put(trenutni, stog->next);
		fgets(unos, MAX_LENGTH, stdin);
		sscanf(unos, "%s %s", naredba, naziv_dir);
		if (!strcmp(naredba, "md")) {
			trenutni = NapraviDir(trenutni, naziv_dir);
		}
		else if (!strcmp(naredba, "cd")) {
			trenutni = PromijeniDir(trenutni, stog, naziv_dir);

		}
		else if (!strcmp(naredba, "cd..")) {
			trenutni = Povratak(stog, trenutni);

		}
		else if (!strcmp(naredba, "dir")) {
			Ispis_Dir(trenutni);
		}
		else if (!strcmp(naredba, "exit")) {
			BrisiStablo(root->child);
			BrisiStog(rootStog);
		}
		else {
			printf("Pogresan unos!\n");
		}

	} while (strcmp(naredba, "exit"));
	return EXIT_SUCCESS;
}
int Put(Position trenutni, StogPosition stog)
{
	char ispis[MAX_LENGTH] = { 0 };

	while (stog != NULL)
	{
		strcat(ispis, stog->data->ime);
		strcat(ispis, "\\");
		stog = stog->next;
	}
	if (stog == NULL) {
		strcat(ispis, trenutni->ime);
		strcat(ispis, "\\");
	}
	printf("%s ", ispis);
	return 0;
}
Position NapraviDir(Position q,char* ime_dir) {
	Position novi_dir = NULL,trenutni=q;
	novi_dir = (Position)malloc(sizeof(Dir));
	if (novi_dir == NULL) {
		printf("Greska u alokaciji memorije!\n");
		return trenutni;
	}
	novi_dir->child = NULL;
	novi_dir->sibling = NULL;
	strcpy(novi_dir->ime, ime_dir);
	if (q->child == NULL) {
		q->child = novi_dir;

	}
	else
	{
		
		if (q->child->sibling == NULL && strcmp(q->child->ime,ime_dir)>0) {
			novi_dir->sibling = q->child;
			q->child = novi_dir;
			return trenutni;
		}
		q = q->child;
		while (q->sibling != NULL && strcmp(ime_dir,q->sibling->ime)>0 ) {
			q = q->sibling;

		}
		novi_dir->sibling = q->sibling;
		q->sibling = novi_dir;
	}
	
	return trenutni;
}
int Ispis_Dir(Position p) {
	p = p->child;
	if (p == NULL) {
		printf("Direktorij je prazan");
	}
	while (p != NULL) {
		printf("%s\t", p->ime);
		p = p->sibling;
	}
	printf("\n");
	return EXIT_SUCCESS;
}
Position PromijeniDir(Position p, StogPosition stog, char* ime_dir) {
	Position tmp=NULL;
	tmp = p->child;
	if (tmp == NULL){
		printf("Directory je prazan!\n");
		return p;
	}
	else
	{
		while (tmp!=NULL){
			if (!strcmp(tmp->ime, ime_dir))
			{
				PushStog(p, stog);
				return tmp;
			}
			tmp = tmp->sibling;
		}
		if(tmp==NULL) {
		printf("Direktorij s tim imenom ne postoji\n");
		}
		
		return p;
	}
}
Position Povratak(StogPosition stog,Position p) {
	Position trenutni = PopStog(stog);
	if (trenutni  == NULL) {
		printf("Nalazite se u rootu.\n");
		return p;
	}
	return trenutni;
}
int PushStog(Position trenutni, StogPosition stog) {
	StogPosition q = NULL;
	q = (StogPosition)malloc(sizeof(Dir));
	if (q == NULL) {
		printf("Pogreska u alokaciji\n");
		return EXIT_FAILURE;
	}
	while (stog->next != NULL)
		stog = stog->next;
	q->next = stog->next;
	stog->next = q;
	q->data = trenutni;
	return EXIT_SUCCESS;


}
Position PopStog(StogPosition stog) {
	StogPosition q;
	Position pom = NULL;
	if (stog->next == NULL) {
		return PRAZAN_STOG;
	}		
	while (stog->next->next != NULL) {
		stog = stog->next;
	}
	q = stog->next;
	stog->next = q->next;
	pom = q->data;
	free(q);
	
	return pom;
}
int BrisiStablo(Position p) {
	if (p == NULL)
		return EXIT_SUCCESS;
	BrisiStablo(p->child);
	BrisiStablo(p->sibling);
	free(p);
	return EXIT_SUCCESS;
}
int BrisiStog(StogPosition p) {
	StogPosition q;
	while (p->next != NULL) {
		q = p->next;
		p->next = q->next;
		free(q);
	}
	return EXIT_SUCCESS;
}