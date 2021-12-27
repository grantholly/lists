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

	Node *last = pop(l);
	last = pop(l);
	last = pop(l);
	printf("popped -> ");
	printNode(last);

	printList(l);
	printf("len %d\n", len(l));

	return 0;
}
