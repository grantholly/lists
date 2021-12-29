# include <stdlib.h>
# include <stdio.h>

# define DEFAULT_LIST_SIZE 4

typedef struct Node {
        unsigned long long id;
        char *data;
        struct Node *next;
} Node;

typedef struct NodeList {
	size_t used;
	size_t size;
	Node *innerNodes;
} NodeList;

typedef struct LinkList {
        unsigned long long node_id;
        Node *head;
        Node *tail;
        NodeList *nodes;
} LinkList;

Node *makeNode(char *myData) {
        Node *n = (Node *) malloc(sizeof(Node));
	if (n == NULL) {
		fprintf(stderr, "not enough memory to create node\n");
		exit(1);
	}
        n->data = myData;
        return n;
}

NodeList *makeNodeList() {
	NodeList *nl = (NodeList *) malloc(sizeof(NodeList));

	if (nl == NULL) {
		fprintf(stderr, "not enough memory to create node list\n");
		exit(1);
	}

	nl->size = DEFAULT_LIST_SIZE;
	nl->used = 0;
	nl->innerNodes = (Node *) malloc(sizeof(Node) * nl->size);

	if (nl->innerNodes == NULL) {
		fprintf(stderr, "not enough memory to create inner node list\n");
		exit(1);
	}

	return nl;
}

LinkList *makeLinkList() {
	LinkList *l = (LinkList *) malloc(sizeof(LinkList));

	if (l == NULL) {
		fprintf(stderr, "not enough memory to create link list\n");
		exit(1);
	}

	NodeList *nl = makeNodeList(l);
	l->nodes = nl;
	l->head = NULL;
	l->tail = NULL;
	return l;
}

void printNode(Node *n) {
	printf("Node: { id: %llu, data: %s, next: %p }\n", n->id, n->data, n->next);
}

void printNodeList(NodeList *nl) {
	printf("NodeList: {\n");
	printf("\tused: %zu\n", nl->size);
	printf("\tsize: %zu\n", nl->used);
	printf("\tnodes: {\n");
	for (int i = 0; i < nl->size; i++) {
		printf("\t\t%p - %s\n", &nl->innerNodes[i], nl->innerNodes[i].data);
	}
}

void printLinkList(LinkList *l) {
	Node *n = l->head;
	printf("LinkList: {\n\thead: %p\n\ttail: %p\n\tnodes: {\n", l->head, l->tail);
	while (n != NULL) {
		printf("\t\t");
		printNode(n);
		n = n->next;
	}
	printf("\t}\n");
	printf("}\n");
}

size_t len(LinkList *l) {
	return l->nodes->size;
}

// grow when we are 75% full
void growInnerNodes(NodeList *nl) {
	int i;
	size_t newSize = nl->size * 2;
	Node *newList = (Node *) malloc(sizeof(Node) * newSize);

	if (newList == NULL) {
		fprintf(stderr, "not enough memory to create inner node list");
		exit(1);
	}

	for (i = 0; i < nl->size; i++) {
		newList[i] = nl->innerNodes[i];
	}

	free(nl->innerNodes);
	nl->innerNodes = newList;
	nl->size = newSize;
}

// shrink when we are 75% empty
void shrinkInnerNodes(Node *nl) {}

void insertNode(NodeList *nl, Node *newNode) {
	// need this to more reliably check
	if (nl->used > nl->size / 2) {
		printf("we need to grow\n");
		growInnerNodes(nl);
	}

	nl->innerNodes[nl->used] = *newNode;
	nl->used++;
}

void setNodeId(LinkList *l, Node *n) {
	n->id = ++l->node_id;
}

void push(LinkList *l, char *myData) {
	Node *previousHead = l->head;
	Node *newHead = makeNode(myData);
	setNodeId(l, newHead);
	newHead->next = previousHead;
	l->head = newHead;

	if (l->tail == NULL) {
		l->tail = newHead;
	}

	insertNode(l->nodes, newHead);
}

int main () {
	NodeList *nl = makeNodeList();
	printf("--- starting with this\n");
	printNodeList(nl);
	Node *n0 = makeNode("first data");
	Node *n1 = makeNode("second thing");
	Node *n2 = makeNode("third stuff");
	Node *n3 = makeNode("forth cosa");
	insertNode(nl, n0);
	insertNode(nl, n1);
	insertNode(nl, n2);
	insertNode(nl, n3);
	printf("--- after insert\n");
	printNodeList(nl);

	return 0;
}
