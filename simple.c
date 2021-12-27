# include <stdio.h>
# include <stdlib.h>

static unsigned long long NODE_ID;

typedef struct Node {
	unsigned long long id;
	char *data;
	struct Node *next;
} Node;

Node *head;
Node *tail;

void printNode(Node *n) {
	printf("Node { id: %llu, data: '%s', next: %p }\n", 
			n->id, n->data, n->next);
}

void printList(Node *n) {
	while (n != NULL) {
		printNode(n);
		n = n->next;
	}
}

Node *makeNode(char *myData) {
	Node *n = (Node *) malloc(sizeof(Node));
	n->id = ++NODE_ID;
	n->data = myData;
	return n;
}

void link(Node *left, Node *right) {
	left->next = right;
}

void unlink(Node *n) {
	n->next = NULL;
}

void init() {
	head = NULL;
	tail = NULL;
	NODE_ID = 0;
}

int main() {
	init();

	Node *first = makeNode("this is the first node");
	Node *second = makeNode("this is some more data for another node");
	Node *third = makeNode("data data data");

	link(first, second);
	link(second, third);

	printList(first);

	head = first;
	tail = third;

	return 0;
}
