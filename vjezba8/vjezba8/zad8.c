#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE (256)
#define GRESKA_ALOKACIJE (-1)
struct red;
struct stablo;
struct redSadrzaj;
typedef struct stablo* Position;
typedef struct redSadrzaj* RedSPosition;
typedef struct red* RedPosition;
typedef struct stablo {
	int broj;
	Position left;
	Position right;
}Stablo;
typedef struct redSadrzaj {
	Position data;
	RedSPosition next;

}RedSadrzaj;
typedef struct red {
	RedSPosition front;
	RedSPosition rear;
	
}Red;

int IspisMenu();
int Start(Position root);
Position AlokacijaStabla(int broj);
Position Unos(Position p, Position q);
void Ispis_I(Position p);
void Ispis_Pre(Position p);
void Ispis_Post(Position p);
void Ispis_Level(Position p);
int StaviNaRed(RedPosition red, Position p);
Position SkiniSReda(RedPosition r);
Position Trazi(int x,Position p);
Position Izbrisi(Position p,int broj);
Position findMax(Position p);
Position findMin(Position p);
 int BrisiStablo(Position root);

 int main() {
	int pom = 0;
	Position root = NULL;
	IspisMenu();
	pom = Start(root);
	if (pom != 0)
		return pom;
	return EXIT_SUCCESS;
	}

int IspisMenu() {
	printf("\t*********MENU*********");
	printf("\n\tunos broj ... [unos 5] - unos novog elementa u stablo");					
	printf("\n\tbrisi broj ... [brisi 5] - brisanje elementa iz stabla");	
	printf("\n\ttrazi broj ... [trazi 5] - pronalazak broja u stablu");		
	printf("\n\tispis i, pre, post, lev - ispis stabla, i - inorder, pre - preoder, post - postoder, lev - level");	
	printf("\n\texit - izlaz\n");
	return EXIT_SUCCESS;
}
int Start(Position root) {
	Position tmp = NULL, nadi = NULL;
	char unos[MAX_LINE] = { 0 };
	char naredba[MAX_LINE] = { 0 };
	int broj = 0;
	char ispis_nar[MAX_LINE] = { 0 };
	do {
		printf("\tVas unos:   ");
		fgets(unos, MAX_LINE, stdin);
		sscanf(unos, "%s", naredba);
		if (!strcmp(naredba, "ispis")) {
			sscanf(unos, "%s %s", naredba, ispis_nar);
			if (!strcmp(ispis_nar, "i")) {
				printf("Inorder: ");
				Ispis_I(root);
				printf("\n");
			}
			else if (!strcmp(ispis_nar, "pre")) {
				printf("Preorder: ");
				Ispis_Pre(root);
				printf("\n");
			}
			else if (!strcmp(ispis_nar, "post")) {
				printf("Postorder: ");
				Ispis_Post(root);
				printf("\n");
			}
			else if (!strcmp(ispis_nar, "level")) {
				printf("Level order:  ");
				Ispis_Level(root);
				printf("\n");
			}
			else printf("Neispravan unos\n");
		}
		else {
			sscanf(unos, "%s %d", naredba, &broj);
			if (!strcmp(naredba, "unos")) {
				tmp = AlokacijaStabla(broj);
				if (tmp == NULL)
					return GRESKA_ALOKACIJE;
				root = Unos(root, tmp);
				broj = -1;
			}
			else if (!strcmp(naredba, "brisi")) {
				root = Izbrisi(root, broj);
			}
			else if (!strcmp(naredba, "trazi")) {
				nadi = Trazi(broj, root);
				if (nadi == NULL)
					printf("Taj element ne postoji.\n");
				else printf("Trazeni broj je: %d\n", nadi->broj);
				broj = -1;
			}
			else if (!strcmp(naredba, "exit")) {
				BrisiStablo(root);
			}
			else printf("Pogresan unos\n");
		}

		;
	} while (strcmp(naredba, "exit"));
	return EXIT_SUCCESS;
}
Position Unos(Position p, Position q) {
	if (p == NULL)
		return q;
	if (p->broj < q->broj)
		p->right = Unos(p->right, q);
	else if (p->broj > q->broj)
		p->left = Unos(p->left, q);
		else free(q);							//UPITNO ZBOG DEALOCIRANJA MEMORIJE U REKURZIJI
	return p;
}
Position AlokacijaStabla(int broj) {
	Position q = NULL;
	q = (Position)malloc(sizeof(Stablo));
	if (q == NULL) {
		printf("Greska u alokaciji memorije.\n");
		return NULL;
	}
	q->left = NULL;
	q->right = NULL;
	q->broj = broj;
	return q;
}
void Ispis_I(Position p) {
	if (p == NULL)
		return ;
		
	Ispis_I(p->left);
	printf("%d ", p->broj);
	Ispis_I(p->right);

}
void Ispis_Pre(Position p) {
	if (p == NULL)
		return;

	printf("%d ", p->broj);
	Ispis_Pre(p->left);
	Ispis_Pre(p->right);

}
void Ispis_Post(Position p) {
	if (p == NULL)
		return;
	Ispis_Post(p->left);	
	Ispis_Post(p->right);
	printf("%d ", p->broj);

}
void Ispis_Level(Position p) {
	Position tmp = p;
	if (tmp == NULL) {
		printf("Nema elemenata u stablu!");
		return;
	}
	RedPosition r = NULL;
	r= (RedPosition)malloc(sizeof(Stablo));
	if (r == NULL) {
		printf("Greska alokacije reda!\n");
		return;
	}
	r->front = NULL;
	r->rear = NULL;
	
	while (tmp != NULL) {
		printf("%d ", tmp->broj);
		if (tmp->left != NULL) {
			StaviNaRed(r,tmp->left);
		}
		if (tmp->right != NULL) {
			StaviNaRed(r,tmp->right);
		}
		tmp = SkiniSReda(r);
	}
	free(r);
}
int StaviNaRed(RedPosition red, Position p) {
	RedSPosition sad = NULL;
	sad= (RedSPosition)malloc(sizeof(RedSadrzaj));
	if (sad == NULL) {
		printf("Greska alokacije sadrzaja reda.\n");
		return EXIT_FAILURE;
	}
	sad->data = p;
	sad->next = NULL;
	if (red->front == NULL) {
		red->front = red->rear = sad;
		return EXIT_SUCCESS;
	}
	else {
		red->rear = red->rear->next = sad;
	}
	
}
Position SkiniSReda(RedPosition r) {
	Position pom = NULL;
	if (r->front == NULL) {
		return NULL;
	}
	RedSPosition sad = r->front;
	pom = r->front->data;
	r->front = r->front->next;
	if (r->front == NULL)
		r->rear = NULL;
	free(sad);
	return pom;
}
Position Trazi(int x, Position p) {
	if (p == NULL)
		return NULL;
	if (x < p->broj)
		return Trazi(x, p->left);
	else if (x > p->broj)
		return Trazi(x, p->right);
	return p;
}
Position Izbrisi(Position p, int broj) {
	if (p == NULL) {
		return NULL;
	}
	if (p->broj < broj) {
		p->right = Izbrisi(p->right, broj);
	} else if (p->broj > broj) {
		p->left = Izbrisi(p->left, broj);
	}
	else {
		if (p->left) {
			Position tmp = findMax(p->left);
			p->broj = tmp->broj;
			p->left = Izbrisi(p->left, tmp->broj);
		}
		else if (p->right) {
			Position tmp = findMin(p->right);
			p->broj = tmp->broj;
			p->right = Izbrisi(p->right, tmp->broj);
		}
		else {
			free(p);
			return NULL;
		}
	}
	return p;
}
Position findMax(Position p) {
	if (p == NULL)
		return NULL;
	while (p->right != NULL) {
		p = p->right;
	}
	return p;
}
Position findMin(Position p) {
	if (p == NULL)
		return NULL;
	while (p->left != NULL) {
		p = p->left;
	}
	return p;
}
int BrisiStablo(Position root) {
	if (root == NULL)
		return EXIT_SUCCESS;
	BrisiStablo(root->right);
	BrisiStablo(root->left);
	free(root);
	return EXIT_SUCCESS;
}
