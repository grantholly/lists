# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <assert.h>

typedef struct Node {
	unsigned long long id;
	int data;
	struct Node *next;
} Node;

typedef struct List {
	Node *head, *tail;
	size_t size;
} List;

typedef struct Generator {
	List *innerList;
	Node *last;
	bool done;	
} Generator;

// functors
typedef int intFunc (const int i);
typedef int intAccumulatorFunc (const int i, const int j);

typedef bool intPredicate (const int i);

typedef bool nodePredicate (Node *n);

// don't forget to free
Node *makeNode(int myData, unsigned long long id) {
	Node *n = (Node *)malloc(sizeof(Node));
	if (n == NULL) {
		printf("could not allocate memory for node\n");
		exit(1);
	}
	n->data = myData;
	n->id = id;
	return n;
}

// don't forget to free
List *makeList() {
	List *l = (List *)malloc(sizeof(List));
	if (l == NULL) {
		printf("could not allocat memory for list\n");
		exit(1);
	}
	l->size = 0;
	l->head = NULL;
	l->tail = NULL;
	return l;
}

void printNode(Node *n) {
        printf("Node: { id: %llu, data: %d, next: %p }\n", n->id, n->data, n->next);
}

void printList(List *l) {
	Node *n = l->head;
	printf("List: {\n");
	printf("\tsize: %zu\n", l->size);
	printf("\tnodes: {\n");

	while (n != NULL) {
		printf("\t\t");
		printNode(n);
		n = n->next;
	}
	printf("\t}\n");
	printf("}\n");
}

void push(List *l, Node *n) {
	Node *currentHead = l->head;
	
	if (l->head == NULL) {
		l->head = n;
		l->tail = n;
	} else {
	        l->head = n;
        	n->next = currentHead;
	}
	l->size++;
}

void append(List *l, Node *n) {
	Node *currentTail = l->tail;

	if (l->tail == NULL) {
		l->head = n;
		l->tail = n;
	} else {
		currentTail->next = n;
		n->next = NULL;
		l->tail = n;
	}
	l->size++;
}

Generator *makeGenerator(List *l) {
	Generator *g = (Generator *)malloc(sizeof(Generator));
	g->innerList = l;
	g->last = l->head;
	g->done = false;

}

void printGenerator(Generator *g) {
	printf("Generator: {\n");
	printf("\tdone: %d\n", g->done);
	printf("\tinnerList: {\n");
	printList(g->innerList);
	printf("\t}\n");
	printf("\tlast: ");
	printNode(g->last);
}

void mapIntInPlace(List *l, intFunc f) {
	Node *n = l->head;

	while (n != NULL) {
		n->data = f(n->data);
		n = n->next;
	}
}

Node *yieldFrom(Generator *g) {
	if (g->last->next != NULL) {
		g->last = g->last->next;
	} else {
		g->done = true;
	}

	if (g->done == true) {
		return NULL;
	} else {
		return g->last;
	}
}

List *mapInt(List *l, intFunc f) {
	List *mapped = makeList();
	Node *n = l->head;

	while (n != NULL) {
		append(mapped, makeNode(f(n->data), n->id));
		n = n->next;
	}
	
	return mapped;
}

int reduceInts(List *l, intAccumulatorFunc f) {
	Node *n = l->head;
	int acc = 0;

	while (n->next != NULL) {
		acc = f(n->data, n->next->data);
		n = n->next;
	}
	return acc;
}

void filterInPlace(List *l, nodePredicate f) {
	Node *follower = l->head;
	Node *leader = follower->next;

	while (leader != NULL) {
		// free unmatched nodes
		if (f(follower) == false) {
			Node *oldHead = follower;
			follower = leader;
			l->head = leader;
			leader = leader->next;
			l->size--;
			free(oldHead);
		} else if (f(leader) == true) {
			// might need to only do this if follower->next != leader
			follower->next = leader;
			follower = leader;
			leader = leader->next;
		} else if (f(leader) == false) {
			// free unmatched nodes
			Node *unMatched = leader;
			follower->next = leader->next;
			leader = leader->next;
			l->size--;
			free(unMatched);
		} else if (follower->next == NULL) {
			l->tail = follower;
		}
	}

	l->tail = follower;
}

// filter({1,2,3,4}, isEven) == {2,4}
// filter({1,3,5,7}, isEven) == NULL
// filter({}, isEven) == NULL
// filter({2,4,6,8}, isEven) == {2,4,6,8}
List *filter(List *l, nodePredicate f) {
	List *filtered = makeList();
	Node *n = l->head;

	while (n != NULL) {
		if (f(n) == true) {
			append(filtered, makeNode(n->data, n->id));
		}
		n = n->next;
	}

	return filtered;
}

// every(l, n) calls `next` n - 1 times on l
// every({1,2,3,4,5,6,7,8}, 2) == {2,3,5,8}
// every({1,2,3,4,5,6,7,8}, 1) == {1,2,3,4,5,6,7,8}
// every({1,2,3,4,5,6,7,8}, 7) == {7}
// every({1,2,3,4,5,6,7,8}, 4) == {4,8}
// every({1,2,3,4,5,6,7,8}, 3) == {3,6}
// every({}, 1) == NULL
// every({}, 0) == NULL
// every({1,2,3}, 8) == NULL
// not quite there yet I need to handle case #3
List *every(List *l, const size_t skip) {
	List *onlyEvery = makeList();
	Node *n = l->head;

	while (n != NULL) {
		for (int i = 1; i < skip; i++) {
			n = n->next;
			if (n == NULL) {
				return onlyEvery;
			}
		}
		append(onlyEvery, makeNode(n->data, n->id));
		if (n->next != NULL) {
			n = n->next;
		} else {
			return onlyEvery;
		}
	}

	return onlyEvery;
}

// take(l, n) calls `next` n times on l
// take({1, 2}, 3) == {1, 2}
// take({}, 3) == NULL
// take({1, 2}, 2) == {1, 2}
// take({1, 2}, 0) == NULL
// take({1, 2, 3}, 2) == {1, 2}
List *take(List *l, const size_t times) {
	List *took = makeList();
	Node *n = l->head;

	if ((times == 0) || (l == NULL)) {
		return NULL;
	}

	for (int i = 0; i < times && n != NULL; i++) {
		append(took, makeNode(n->data, n->id));
		n = n->next;
	}

	return took;
}

List *concat(List *ll, List *rl) {
	ll->size += rl->size;
	ll->tail->next = rl->head;
	return ll;
}

int timesTwo(const int i) {
	return i + i;
}

int squareIt(const int i) {
	return i * i;
}

int sum(const int i, const int j) {
	return i + j;
}

int prod(const int i, const int j) {
	return i * j;
}

bool isEven(Node *n) {
	Node val = *n;
	return val.data % 2 == 0;
}

bool isInFavorites(Node *n) {
	int favorites[] = {4, 9};

	for (int j = 0; j < 2; j++) {
		if (n->data == favorites[j]) {
			return true;
		}
	}
	return false;
}

int main() {
	unsigned long long nodeId = 0;
	List *l = makeList(); 
	Node *n1 = makeNode(1, ++nodeId);
	Node *n2 = makeNode(2, ++nodeId);
	Node *n3 = makeNode(3, ++nodeId);
	Node *n4 = makeNode(4, ++nodeId);

	push(l, n1);
	push(l, n2);
	push(l, n3);
	push(l, n4);

	printf("--- starting with 3\n");
	printList(l);
	List *squares = mapInt(l, &squareIt);

	printf("--- new list now squared\n");
	printList(squares);
	
	printf("--- new list filtering out odds\n");
	List *filteredList = filter(squares, &isEven);
	printList(filteredList);

	printf("--- reducing with product\n");
	int reduction = reduceInts(filteredList, &prod);
	printf("reduction = %d\n", reduction);

	List *takeSome = take(filteredList, 1);
	printf("--- taking one\n");
	if (takeSome == NULL) {
		printf("only took none\n");
	} else {
		printList(takeSome);
	}

	// maybe invent a `growBy` function for testing?
	Node *sixteen = makeNode(16, ++nodeId);
	Node *seventeen = makeNode(17, ++nodeId);
	Node *eighteen = makeNode(18, ++nodeId);
	push(takeSome, sixteen);
	push(takeSome, seventeen);
	push(takeSome, eighteen);

	printf("--- pushing some new nodes in\n");
	printList(takeSome);

	printf("--- new list after taking every other\n");
	List *everyOther = every(takeSome, 2);
	printList(everyOther);
	Node *twenty = makeNode(20, ++nodeId);
	Node *twentyOne = makeNode(21, ++nodeId);
	Node *twentyThree = makeNode(23, ++nodeId);
	Node *twentyTwo = makeNode(22, ++nodeId);

	append(everyOther, twenty);
	append(everyOther, twentyOne);
	append(everyOther, twentyTwo);
	append(everyOther, twentyThree);
	printf("--- adding more nodes before filtering for evens\n");

	filterInPlace(everyOther, &isEven);
	printList(everyOther);

	printf("--- concatenating\n");
	List *other = makeList();
	push(other, n1);
	push(other, n2);
	push(other, n3);
	push(other, n4);

	printList(other);
	List *both = concat(everyOther, other);
	printList(both);

	printf("--- yielding values from generator\n");
	Generator *gen = makeGenerator(both);
	printGenerator(gen);

	return 0;
}
