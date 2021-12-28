# include <stdio.h>
# include <stdlib.h>

typedef struct Node {
	unsigned long long id;
	char *data;
	struct Node *next;	
} Node;

typedef struct LinkList {
	unsigned long long node_id;
	Node *head;
	Node *tail;
} LinkList;

LinkList *makeLinkList() {
	LinkList *l = (LinkList *) malloc(sizeof(LinkList));
	l->head = NULL;
	l->tail = NULL;
	return l;
}

Node *makeNode(LinkList *l, char *myData) {
	Node *n = (Node *) malloc(sizeof(Node));
	n->id = ++l->node_id;
	n->data = myData;
	return n;
} 

Node *push(LinkList *l, char *myData) {
	Node *newHead = makeNode(l, myData);
	Node *previousHead = l->head;
	newHead->next = previousHead;
	l->head = newHead;

	if (l->tail == NULL) {
		l->tail = newHead;
	} 
	return l->head;
}

Node *insertAtIndex(LinkList *l, char *myData, int idx) {
	Node *newNode = makeNode(l, myData);
	Node *current = l->head;
	Node *previous;
	int i;

	for (i = 0; i < len(l); i++) {
		if (i == idx) {
			previous->next = newNode;
			newNode->next = current;
			return newNode;
		} else {
			previous = current;
			current = current->next;
		}
	}
	return NULL;
}

void printNode(Node *n) {
        printf("Node { id: %llu, data: '%s', next: %p }\n",
                        n->id, n->data, n->next);
}

void printList(LinkList *l) {
	Node *n = l->head;

	while(n != NULL) {
		printNode(n);
		n = n->next;
	}
	printf("head -> ");
	printNode(l->head);
	printf("tail -> ");
	printNode(l->tail);
}

int len(LinkList *l) {
	int i = 0;
	Node *n = l->head;

	while(n != NULL) {
		i++;
		n = n->next;
	}
	return i;
}

Node *findById(LinkList *l, unsigned long long id) {
	Node *n = l->head;

	while(n != NULL) {
		if (n->id == id) {
			return n;
		} else {
			n = n->next;
		}
	}
	return NULL;
}

Node *findByIndex(LinkList *l, int idx) {
	Node *n = l->head;
	int i;

	for (i = 0; i < len(l); i++) {
		if (i == idx) {
			return n;
		} else {
			n = n->next;
		}
	}
	return NULL;
}

Node *updateById(LinkList *l, char *myData, unsigned long long id) {
	Node *n = l->head;
	while(n->id != id) {
		n = n->next;
	}
	n->data = myData;
	return n;
}

// don't forget to free the popped node
Node *pop(LinkList *l) {
	Node *n = l->head;
	Node *previous;
	while(n->next != NULL) {
		previous = n;
		n = n->next;
	}
	previous->next = n->next;
	l->tail = previous;
	return n;
}

Node *deleteAt(LinkList *l, int idx) {
	Node *current = l->head;
	Node *previous;
	int i;
	int end = len(l);

	if (idx == 0) {
		l->head = current->next;
		return current;
	}

	if (idx == end) {
		return pop(l);
	}

	for (i = 0; i < end; i++) {
		if (i == idx) {
			previous->next = current->next;
			return current;	
		} else {
			previous = current;
			current = current->next;
		}
	}
}

int main () {
	LinkList *l = makeLinkList();
	push(l, "first");
	push(l, "second");
	push(l, "third");
	push(l, "forth");
	push(l, "fifth");
	push(l, "sixth");
	push(l, "seventh");
	push(l, "eighth");
	push(l, "nineth");
	push(l, "tenth");

	printf("--- made linked list\n");
	printList(l);

	printf("--- popping nodes off\n");
	Node *last = pop(l);
	printf("popped -> ");
	printNode(last);
	free(last);
	last = pop(l);
	printf("popped -> ");
	printNode(last);
	free(last);
	last = pop(l);
	printf("popped -> ");
	printNode(last);
	free(last);

	printf("--- updated linked list\n");
	printList(l);
	printf("len %d\n", len(l));

	printf("--- findById 6\n");
	Node *six = findById(l, 6);
	printNode(six);

	printf("--- findByIndex 2\n");
	Node *second = findByIndex(l, 2);
	printNode(second);

	printf("--- insertAtIndex 4 'out of order node'\n");
	Node *ooo = insertAtIndex(l, "out of order node", 4);
	printNode(ooo);

	printf("--- updateById 10 'my update'\n");
	Node *updated = updateById(l, "my update!", 10);
	printNode(updated);

	printf("--- deleteAt head\n");
	Node *deletedAt = deleteAt(l, 0);
	printNode(deletedAt);
	free(deletedAt);

	printf("--- deleteAt 2\n");
	Node *secondDelete = deleteAt(l, 1);
	printNode(secondDelete);
	free(secondDelete);

	printf("--- deleteAt tail\n");
	Node *tailDelete = deleteAt(l, len(l));
	printNode(tailDelete);
	free(tailDelete);

        printf("--- updated linked list\n");
        printList(l);

	return 0;
}
