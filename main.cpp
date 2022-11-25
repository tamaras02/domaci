#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node1 { //components
	char ime[31];
	char tip[6];
	double cena;
	int ocena;
	struct node1* next;
} node1;

typedef struct node2 { //combinations
	char cpu[31];
	char board[31];
	char mem[31];
	double odnos; //cpu+board+mem cena/ocena
	struct node2* next;
} node2;

node1* load() { //ucitavanje iz fajla components u ulancanu listu
	node1* head = NULL, * tmp, * tail = NULL;
	FILE* comp = fopen("components.txt", "r");
	if (comp == NULL) return NULL;
	while (!feof(comp)) {
		tmp = (node1*)malloc(sizeof(node1));
		if (tmp == NULL) {
			printf("MEM_GRESKA");
			return NULL;
		}
		fscanf(comp, "%s %s %lf %d", tmp->ime, tmp->tip, &tmp->cena, &tmp->ocena);
		tmp->next = NULL;
		if (!head) head = tmp;
		else tail->next = tmp;
		tail = tmp;
	}
	fclose(comp);
	return head;
}

node1* addNode(node1* head, node1* k) { //dodavanje novog cvora na listu tipa node1
	node1* temp, * p;
	temp = (node1*)malloc(sizeof(node1));
	if (temp == NULL) {
		printf("MEM_GRESKA");
		return NULL;
	}
	strcpy(temp->ime, k->ime);
	strcpy(temp->tip, k->tip);
	temp->cena = k->cena;
	temp->ocena = k->ocena;
	temp->next = NULL;
	if (!head) head = temp;
	else {
		p = head;
		while (p->next != NULL) p = p->next;
		p->next = temp;
	}
	return head;
}

int swapNode(node1* n1, node1* n2) { //menja mesta 2 cvora u listi tipa node1
	node1* temp;
	temp = (node1*)malloc(sizeof(node1));
	if (temp == NULL) {
		printf("MEM_GRESKA");
		return 1;
	}
	strcpy(temp->ime, n1->ime);
	strcpy(temp->tip, n1->tip);
	temp->cena = n1->cena;
	temp->ocena = n1->ocena;
	temp->next = NULL;
	strcpy(n1->ime, n2->ime);
	strcpy(n1->tip, n2->tip);
	n1->cena = n2->cena;
	n1->ocena = n2->ocena;
	strcpy(n2->ime, temp->ime);
	strcpy(n2->tip, temp->tip);
	n2->cena = temp->cena;
	n2->ocena = temp->ocena;
	free(temp);
	return 0;
}

void freeList1(node1 *head) { //oslobadja memoriju koju zauzima lista tipa node1
   node1 *t;
   while (head) {
      t = head;
      head = head->next;
      free(t);
   }
}

void freeList2(node2 *head){ //oslobadja memoriju koju zauzima lista tipa node2
    node2 *t;
   while (head) {
      t = head;
      head = head->next;
      free(t);
   }
}

node2* combinations(node1* head) { //pravi sortiranu listu tipa node2 od liste tipa node1
	node1* cpu = NULL, * board = NULL, * mem = NULL;
	for (; head; head = head->next) {
		if (!strcmp(head->tip, "CPU")) cpu = addNode(cpu, head);
		if (!strcmp(head->tip, "BOARD")) board = addNode(board, head);
		if (!strcmp(head->tip, "MEM")) mem = addNode(mem, head);
	}
	node1* start = cpu, *p;
	for (; cpu; cpu = cpu->next)
		for (p = cpu->next; p; p = p->next) {
			if (strcmp(cpu->ime, p->ime) > 0) {
				if (swapNode(cpu, p)) {
					printf("MEM_GRESKA");
					return NULL;
				}
			}
		}
	cpu = start;
	start = board;
	node1 *q;
	for (; board; board = board->next)
		for (q = board->next; q; q = q->next) {
			if (strcmp(board->ime, q->ime) > 0) {
				if (swapNode(board, q)) {
					printf("MEM_GRESKA");
					return NULL;
				}
			}
		}
	board = start;
	start = mem;
	node1* r;
	for (; mem; mem = mem->next)
		for (r = mem->next; r; r = r->next) {
			if (strcmp(mem->ime, r->ime) > 0) {
				if (swapNode(mem, r)) {
					printf("MEM_GRESKA");
					return NULL;
				}
			}
		}
	mem = start;
	node2* comb = NULL, * temp, * end = NULL;
	node1* scpu, *sboard, *smem;
	for (scpu = cpu; scpu; scpu = scpu->next) {
		for (sboard = board; sboard; sboard = sboard->next) {
			for (smem = mem; smem; smem = smem->next) {
				temp = (node2*)malloc(sizeof(node2));
				if (temp == NULL) {
					printf("MEM_GRESKA");
					return NULL;
				}
				strcpy(temp->cpu, scpu->ime);
				strcpy(temp->board, sboard->ime);
				strcpy(temp->mem, smem->ime);
				temp->odnos = (scpu->cena + sboard->cena + smem->cena) / (scpu->ocena + sboard->ocena + smem->ocena);
				temp->next = NULL;
				if (comb == NULL) comb = temp;
				else end->next = temp;
				end = temp;
			}
		}
	}
	freeList1(cpu);
	freeList1(board);
	freeList1(mem);
	return comb;
}

int new_txt(node2* head) { //pravi novi txt fajl od liste kombinacija
	FILE* fc = fopen("combinations.txt", "w");
	if (fc == NULL) return 1;
	while (head != NULL) {
		fprintf(fc, "%s %s %s %.2lf\n", head->cpu, head->board, head->mem, head->odnos);
		head = head->next;
	}
	fclose(fc);
	return 0;
}

int best_comb(node2* head) { //nalazi najbolju kombinaciju iz liste kombinacija
	node2* best;
	best = (node2*)malloc(sizeof(node2));
	if (best == NULL) {
		printf("MEM_GRESKA");
		return 1;
	}
	strcpy(best->cpu, head->cpu);
	strcpy(best->board, head->board);
	strcpy(best->mem, head->mem);
	best->odnos = head->odnos;
	best->next = NULL;
	head = head->next;
	while (head != NULL) {
		if (head->odnos < best->odnos) {
			strcpy(best->cpu, head->cpu);
			strcpy(best->board, head->board);
			strcpy(best->mem, head->mem);
			best->odnos = head->odnos;
		}
		head = head->next;
	}
	printf("%s %s %s", best->cpu, best->board, best->mem);
	free(best);
	return 0;
}

int main() {
	node1* head;
	head = load();
	if (head == NULL) {
		printf("DAT_GRESKA");
		return 0;
	}
	node2* comb;
	comb = combinations(head);
	if (new_txt(comb)) {
		printf("DAT_GRESKA");
		return 0;
	}
	if (best_comb(comb)) {
		printf("MEM_GRESKA");
		return 0;
	}
	freeList1(head);
	freeList2(comb);
	return 0;
}