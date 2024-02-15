#define _CRT_SECURE_NO_WARNINGS
#define inf INT_MAX

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct Komsija {
	int indeks_komsija1;
	int indeks_komsija2;
	struct Komsija* sledeci;
	struct Komsija* prethodni;
}Komsija;

typedef struct Cvor {
	int distance;
	int broj_cvora;
	struct Cvor* sledeci;
}Cvor;

typedef struct Tacka {
	int row;
	int column;
}Tacka;//strukutura za skladistenje pocetne tacke i krajnje tacke

char* grid;
int* set;
int* sz;
int* matricaGrafa;
Komsija* susedniCvorovi = NULL;
int rows;
int columns;
int rowsMatriceGrafa;
int columnsMatriceGrafa;
int defaultOpcija = 1;
Tacka cilj;
Tacka start;

int indeksPocetka;
int computerDistance = 0;
int *visited;
int *distance;
int *previous;
Cvor* pq = NULL;

Cvor* napraviNoviCvor(int distance, int broj_cvora) {
	Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
	if (novi == NULL) return NULL;

	novi->sledeci = NULL;
	novi->distance = distance;
	novi->broj_cvora = broj_cvora;

	return novi;
}
void insertAfter(Cvor* p, Cvor* q) {
	Cvor* pom = p->sledeci;
	p->sledeci = q;
	q->sledeci = pom;
}
void ubaciURed(Cvor** lista, Cvor* novi) {
	//pronadji de da ga roknem
	Cvor* pomocni = *lista;
	Cvor* pom2 = NULL;

	while (pomocni != NULL && pomocni->distance <= novi->distance) {
		pom2 = pomocni;
		pomocni = pomocni->sledeci;
	}
	//insertAfter
	if (pom2 == NULL) {
		Cvor* pom3 = *lista;
		(*lista) = novi;
		novi->sledeci = pom3;
		return;
	}
	insertAfter(pom2, novi);
}
Cvor* deque(Cvor** lista) {
	if (*lista == NULL) {
		return NULL;
	}
	else if ((*lista)->sledeci == NULL) {
		Cvor* pom = (*lista);
		(*lista) = NULL;
		return pom;
	}
	Cvor* pom = (*lista);
	(*lista) = (*lista)->sledeci;

	return pom;
}
void ispis(Cvor* lista) {
	Cvor* pom = lista;

	while (pom != NULL) {
		printf("%d Cvor: %d", pom->distance, pom->broj_cvora);
		pom = pom->sledeci;
	}
	printf("\n");
}

void inicijalizacijaDijkstrinogAlgoritma() {
	for (int i = 0; i < rows * columns; i++) {
		distance[i] = inf;
		previous[i] = inf;
	}
	start.row = 0;
	start.column = 0;
	indeksPocetka = start.row * columns + start.column;
	visited[indeksPocetka] = 1;
	distance[indeksPocetka] = 0;
	previous[indeksPocetka] = -1;

	for (int i = 0; i < rows * columns; i++) {
		if (!visited[i] && matricaGrafa[indeksPocetka * columnsMatriceGrafa + i] == 1) {

			if (distance[indeksPocetka] + 1 < distance[i]) {
				distance[i] = distance[indeksPocetka] + 1;
				previous[i] = indeksPocetka;
				ubaciURed(&pq, napraviNoviCvor(distance[i], i));
			}
		}
	}

}
void DijkstrinAlgoritam() {
	while (pq != NULL) {
		Cvor* trenutni = deque(&pq);
		visited[trenutni->broj_cvora] = 1;
		//poseti sve susede trenutnog cvora
		for (int i = 0; i < rows * columns; i++) {
			if (!visited[i] && matricaGrafa[trenutni->broj_cvora * columnsMatriceGrafa + i] == 1) {
				//update distance
				if (distance[trenutni->broj_cvora] + 1 < distance[i]) {
					distance[i] = distance[trenutni->broj_cvora] + 1;
					previous[i] = trenutni->broj_cvora;
					ubaciURed(&pq, napraviNoviCvor(distance[i], i));
				}
			}
		}
	}
	//update computerDistance
	printf("CILJE JE: %d %d\n", cilj.row, cilj.column);
	computerDistance = distance[cilj.row * columns + cilj.column];
}



// <---------- DO OVDE IDE UMETNUT KOD ----------------------->
Komsija* napraviNovi(int prvi, int drugi) {
	Komsija* novi = (Komsija*)malloc(sizeof(Komsija));
	novi->indeks_komsija1 = prvi;
	novi->indeks_komsija2 = drugi;
	novi->sledeci = NULL;
	novi->prethodni = NULL;
	return novi;
}
void ubaciUListu(Komsija* novi, Komsija** lista) {
	if (*lista == NULL) {
		(*lista) = novi;
		return;
	}

	Komsija* pom = (*lista);
	(*lista) = novi;
	novi->sledeci = pom;
	pom->prethodni = novi;
}
void ispisi(Komsija* lista) {
	Komsija* pom = lista;
	while (pom != NULL) {
		printf("%d - %d\n", pom->indeks_komsija1, pom->indeks_komsija2);
		pom = pom->sledeci;
	}
}
int brojCvorovaUListi(Komsija* lista) {
	Komsija* pom = lista;
	int brojCvorova = 0;

	while (pom != NULL) {
		brojCvorova++;
		pom = pom->sledeci;
	}

	return brojCvorova;
}
Komsija* pronadjiCvorPoIndeksu(int indeks, Komsija* lista) {
	if (lista == NULL) {
		return NULL;
	}
	Komsija* pom = lista;
	int i = 0;
	while (i != indeks) {
		pom = pom->sledeci;
		i++;
	}

	return pom;
}
void obrisiCvor(Komsija** lista, Komsija* cvor) {
	if ((*lista) == NULL || cvor == NULL) {
		return;
	}
	else if (cvor->sledeci == NULL && cvor->prethodni != NULL) {
		cvor->prethodni->sledeci = NULL;
		free(cvor);
	}
	else if (cvor->prethodni == NULL && cvor->sledeci != NULL) {
		(*lista) = (*lista)->sledeci;
		(*lista)->prethodni = NULL;
		free(cvor);
	}
	else if (cvor->prethodni == NULL && cvor->sledeci == NULL) {
		(*lista) = NULL;
		free(cvor);
	}
	else {
		cvor->sledeci->prethodni = cvor->prethodni;
		cvor->prethodni->sledeci = cvor->sledeci;
		free(cvor);
	}
}


void inicijalizacija() {
	visited = (int*)malloc(sizeof(int) * rows * columns); //broj cvorova
	distance = (int*)malloc(sizeof(int) * rows * columns);
	previous = (int*)malloc(sizeof(int) * rows * columns);
	grid = (char*)malloc(sizeof(char) * rows * columns);
	set = (int*)malloc(sizeof(int) * rows * columns);
	sz = (int*)malloc(sizeof(int) * rows * columns);

	for (int i = 0; i < rows * columns; i++) {
		grid[i] = ' ';
		set[i] = i;
		sz[i] = 1;
		visited[i] = 0;
		distance[i] = 0;
		previous[i] = 0;
	}
	rowsMatriceGrafa = rows * columns;
	columnsMatriceGrafa = rows * columns;


	int n = rows * columns;

	//generisati matricu grada
	matricaGrafa = (int*)malloc(sizeof(int) * (n * n));

	//
	if (defaultOpcija) {
		cilj.row = rows - 1;
		cilj.column = columns - 1;
		grid[(rows - 1) * columns + (columns - 1)] = '*';
	}
	else {
		grid[(cilj.row)* columns + (cilj.column)] = '*';
	}

}


int komsijaNaIstokuPostoji(int trenutniCvor) {
	int row = trenutniCvor / columns;
	int column = trenutniCvor % columns;

	if (column + 1 >= columns) {
		return 0;
	}

	return 1;
}
int komsijaNaIstoku(int trenutniCvor) {
	int row = trenutniCvor / columns;
	int column = trenutniCvor % columns;

	return row * columns + (column + 1);

}
int komsijaNaJuguPostoji(int trenutniCvor) {
	int row = trenutniCvor / columns;
	int column = trenutniCvor % columns;

	if (row + 1 >= rows) {
		return 0;
	}

	return 1;
}
int komsijaNaJugu(int trenutniCvor) {
	int row = trenutniCvor / columns;
	int column = trenutniCvor % columns;

	return (row + 1) * columns + column;

}

void generisiSvePutanje() {
	for (int i = 0; i < rows * columns; i++) {
		if (komsijaNaIstokuPostoji(i)) {
			ubaciUListu(napraviNovi(i, komsijaNaIstoku(i)), &susedniCvorovi);
		}
		if (komsijaNaJuguPostoji(i)) {
			ubaciUListu(napraviNovi(i, komsijaNaJugu(i)), &susedniCvorovi);
		}
	}
}


int root(int indeks) {
	int i = indeks;
	while (set[i] != i) {
		i = set[i];
	}
	return set[i];
}
int postojiCiklus(int i, int j) {
	return root(i) == root(j) ? 1 : 0;
}
void unija(int p, int q) {
	int i = root(p);
	int j = root(q);
	if (i == j) return;

	if (sz[i] < sz[j]) {
		set[i] = j;
		sz[j] += sz[i];
	}
	else {
		set[j] = i;
		sz[i] += sz[j];
	}
}


void KruskalovAlgoritam() {
	//izaberi random cvor iz komsija dokle god ima komsija u listi
	int ukupanBrojCvorovaUListi = brojCvorovaUListi(susedniCvorovi);
	while (susedniCvorovi != NULL) {
		srand(time(NULL));
		int broj = rand() % ukupanBrojCvorovaUListi;
		Komsija* cvor = pronadjiCvorPoIndeksu(broj, susedniCvorovi);
		//popuni matricu grafa sa dobijenim podacija
		int prviCvor = cvor->indeks_komsija1;
		int drugiCvor = cvor->indeks_komsija2;

		////check if when merged into one set will form a cycle
		if (!postojiCiklus(prviCvor, drugiCvor)) {
			unija(prviCvor, drugiCvor);

			//popuni matricu grafa
			matricaGrafa[prviCvor * columnsMatriceGrafa + drugiCvor] = 1;
			matricaGrafa[drugiCvor * columnsMatriceGrafa + prviCvor] = 1;
			obrisiCvor(&susedniCvorovi, cvor);
			ukupanBrojCvorovaUListi--;
		}
		else {

			obrisiCvor(&susedniCvorovi, cvor);
			ukupanBrojCvorovaUListi--;
		}

	}
}

void dealocirajMemoriju() {
	free(matricaGrafa);
	free(visited);
	free(distance);
	free(previous);
	free(grid);
	free(set);
	free(sz);
}

void nacrtajMapU() {
	//top
	printf("+");
	for (int i = 0; i < columns; i++) {
		printf("---+");
	}
	printf("\n");


	for (int i = 0; i < rows; i++) {
		char top = '|';
		char bottom = '+';

		printf("%c", top);
		for (int j = 0; j < columns; j++) {

			//char telo[4] = "   ";
			char istok = '|';
			int indeksTrenutnog = i * columns + j;
			if (komsijaNaIstokuPostoji(indeksTrenutnog)) {
				int indeksKomsije = komsijaNaIstoku(indeksTrenutnog);
				if (matricaGrafa[indeksTrenutnog * columnsMatriceGrafa + indeksKomsije] == 1) {
					istok = ' ';
				}
				else {
					istok = '|';
				}
			}
			else {
				istok = '|';
			}
			char* str1 = "\033[1;32mo\033[0m";
			char* str2 = " ";
			printf(" %s %c", grid[i * columns + j] == 'o' || grid[i * columns + j] == '*'?str1:str2 , istok);
		}
		printf("\n");
		printf("%c", bottom);
		for (int j = 0; j < columns; j++) {

			char jug[4] = "   ";
			char ugao = '+';
			int indeksTrenutnog = i * columns + j;
			if (komsijaNaJuguPostoji(indeksTrenutnog)) {
				int indeksKomsije = komsijaNaJugu(indeksTrenutnog);
				if (matricaGrafa[indeksTrenutnog * columnsMatriceGrafa + indeksKomsije] == 1) {
					strcpy(jug, "   ");
				}
				else {
					strcpy(jug, "---");
				}
			}
			else {
				strcpy(jug, "---");
			}
			printf("%s%c", jug, ugao);

		}

		printf("\n");
	}
}
int proveraUnetihVrednosti(int a, int b) {
	if (a >= rows || a < 0 || b >= columns || b < 0) {
		printf("Koordinate cilje moraju da budu u opsegu x:[0, %d]; y:[0, %d]\n\n", rows - 1, columns - 1);
		return 0;
	}
	return 1;
}

void updateGridSaIzracunatomPutanjom() {

	int i = cilj.row * columns + cilj.column;
	while (previous[i] != -1) {
		int row1 = previous[i] / columns;
		int column1 = previous[i] % columns;
		grid[row1 * columns + column1] = 'o';
		i = previous[i];
	}

}



int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("Niste uneli nijedan parametar putem komandne linije\n");
		printf("Unesi broj redova: \n");
		scanf(" %d", &rows);
		printf("unesi broj kolona: \n");
		scanf(" %d", &columns);
	}
	else if (argc == 3) {
		rows = atoi(argv[1]);
		columns = atoi(argv[2]);
		printf("broj redova: %s\n", argv[1]);
		printf("broj kolona: %s\n", argv[2]);
	}
	else if (argc == 6) {
		rows = atoi(argv[1]);
		columns = atoi(argv[2]);
		printf("broj redova: %s\n", argv[1]);
		printf("broj kolona: %s\n", argv[2]);
		if (strcmp(argv[3], "-dest") == 0) {
			if (proveraUnetihVrednosti(atoi(argv[4]), atoi(argv[5]))) {
				cilj.row = atoi(argv[4]);
				cilj.column = atoi(argv[5]);
				defaultOpcija = 0;
			}
		}
		else {
			printf("Nepravilan unos koordinata cilja\n");
		}
	}
	else {
		printf("pokusajte ponovo\n");
	}



	inicijalizacija();

	generisiSvePutanje();

	KruskalovAlgoritam();

	inicijalizacijaDijkstrinogAlgoritma();
	DijkstrinAlgoritam();

	nacrtajMapU();

	updateGridSaIzracunatomPutanjom();

	nacrtajMapU();

	dealocirajMemoriju();
	return 0;

}