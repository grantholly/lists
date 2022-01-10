# include <stdlib.h>
# include <stdio.h>

# define DEFAULT_LIST_SIZE 4
# define MAX_LIST_USAGE_RATIO 0.9

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
	size_t len;
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
	l->len = 0;
	l->head = NULL;
	l->tail = NULL;
	return l;
}

void printNode(Node *n) {
	printf("Node: { id: %llu, data: %s, next: %p }\n", n->id, n->data, n->next);
}

void printNodeList(NodeList *nl) {
	printf("NodeList: {\n");
	printf("\tused: %zu\n", nl->used);
	printf("\tsize: %zu\n", nl->size);
	printf("\tnodes: {\n");
	for (int i = 0; i < nl->size; i++) {
		printf("\t\t%p - %s\n", &nl->innerNodes[i], nl->innerNodes[i].data);
	}
	printf("\t}\n");
	printf("}\n");
}

void printLinkList(LinkList *l) {
	printf("LinkList: {\n\thead: %p\n\ttail: %p\n\tlen: %zu\n\tnodes: {\n", l->head, l->tail, l->len);

	for (int i = 0; i < l->nodes->used; i++) {
		printf("\t\t%p - %s\n", &l->nodes->innerNodes[i], l->nodes->innerNodes[i].data);
	}

	printf("\t}\n");
	printf("}\n");
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
void shrinkInnerNodes(NodeList *nl) {
	int i;
	size_t newSize = nl->size / 2;

	Node *newList = (Node*) malloc(sizeof(Node) * newSize);

	if (newList == NULL) {
		fprintf(stderr, "not enough memory to creat inner node list");
		exit(1);
	}
	
	for (i = 0; i < newSize; i++) {
		newList[i] = nl->innerNodes[i];
	}

	free(nl->innerNodes);
	nl->innerNodes = newList;
	nl->size = newSize;
}

// make this operate on a LinkList
void insertNode(LinkList *l, Node *newNode) {
	float usageRatio = (float)l->nodes->used / (float)l->nodes->size;

	if (usageRatio >= MAX_LIST_USAGE_RATIO) {
		printf("we need to grow. used -> %zu, size -> %zu\n", l->nodes->used, l->nodes->size);
	        printf("usage ratio: %f\n", usageRatio);
		growInnerNodes(l->nodes);
	} else {
		printf("inserting node\n");
	}

	if (l->head == NULL) {
		l->head = &l->nodes->innerNodes[0];
	}

	l->nodes->innerNodes[l->nodes->used] = *newNode;
	l->nodes->used++;
	l->len++;
	l->tail = &l->nodes->innerNodes[l->nodes->used];
}

void setNodeId(LinkList *l, Node *n) {
	n->id = ++l->node_id;
}

// not used
void push(LinkList *l, char *myData) {
	Node *previousHead = l->head;
	Node *newHead = makeNode(myData);
	setNodeId(l, newHead);
	newHead->next = previousHead;
	l->head = newHead;

	if (l->tail == NULL) {
		l->tail = newHead;
	}

	insertNode(l, newHead);
}

// up to caller to free the returned node
Node *pop(LinkList *l) {
	Node *oldTail = l->tail;

	l->tail = &l->nodes->innerNodes[l->nodes->used - 1];
	l->nodes->used--;
	return oldTail;
}

Node *popNode(LinkList *l) {
	Node *n = pop(l);
	l->nodes->used--;
	size_t newSize = l->nodes->size / 2;
        float usageRatio = (float)l->nodes->used / (float)l->nodes->size;

	printf("usageRatio of %f\n", usageRatio);

        // check if `newSize` goes to 0
        // if so, then use the DEFAULT_LIST_SIZE
        if (usageRatio <= 0.25) {
		printf("shrinking down\n");
		if (newSize <= DEFAULT_LIST_SIZE) {
			printf("using minimum size of %d\n", DEFAULT_LIST_SIZE);
		}
		shrinkInnerNodes(l->nodes);
	}

	return n;
}

int main () {
	LinkList *l = makeLinkList();
	printf("--- starting with this\n");
	printNodeList(l->nodes);
	/*
	Node *n0 = makeNode("first data");
	Node *n1 = makeNode("second thing");
	Node *n2 = makeNode("third stuff");
	Node *n3 = makeNode("forth cosa");
	Node *n4 = makeNode("fifth time around");
	insertNode(l, n0);
	insertNode(l, n1);
	insertNode(l, n2);
	insertNode(l, n3);
	insertNode(l, n4);
	*/

	int numInserts = 8;
	for (int i = 0; i < numInserts; i++) {
		char *nodeData;
		sprintf(nodeData, "node #%d", i);
		Node *n = makeNode(nodeData);
		printNode(n);
		insertNode(l, n);
	}

	printf("%s\n", l->nodes->innerNodes[2].data);

	printf("--- after insert\n");
	printNodeList(l->nodes);
	printf("--- popping some nodes\n");
	popNode(l);

	printLinkList(l);
	printNodeList(l->nodes);
	return 0;
}
