#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Clauses
{
    int *clause;
} Clause;

typedef struct Instances
{
    char name[120];
    int nVarTotal, nVar, nClauses;
    Clause* clauses;
    int *mandatory;
    int *cost;
    int *benefits;
    int budget;
} Instance;

typedef struct Ids{
    int value;
    int id;
}Id;

typedef struct adjacencyNodes
{
    Id idNext;
    struct adjacencyNodes* next;
}adjacencyNode;

typedef struct adjacencyLists
{
    Id idMy;
    adjacencyNode* head;
}adjacencyList;

typedef struct Graphs
{
    int nNodes;
    adjacencyList* array;
}Graph;



int errno;

// Implemented Functions
Instance *handleInstance (char *fileName);
static void closeFile (FILE *fp);
static FILE *openFileRead (char *fileName, char *mode);
void printInstance (Instance* instance);
static void exitApplication(Instance* instance);
void instanceToGraph(Instance* instance, Graph* graph);
adjacencyNode* newAdjacencyListNode(Id idNext);
Graph* createGraph(int nNodes);
void addEdge(Graph* graph, Id actual, Id idNext);
void printGraph(Graph* graph);


/////////////////////////////////////////////////////////////////////
// Test Unit
int main(int argc, char **argv)
{

    Instance* instance = NULL;
    Graph* graph = NULL;
    instance = handleInstance("instance.txt");
    graph = createGraph(instance->nVarTotal);

    printInstance(instance);
    instanceToGraph(instance, graph);
    exitApplication(instance);
    return 0;
}



///////////////////////////////////////////////////////////////////


// Function receives a char* fileName  and a char* mode
// Return a non-NULL pointer file.
static FILE *openFileRead(char *fileName, char *mode)
{
    FILE *fp = NULL;
    if ((fp = fopen(fileName, mode)) == NULL)
    {
        fprintf(stderr, "File cannot been opened.");
    }
    return fp;
}

// Function receives a FILE* fp and close the FILE*
// Return nothing
static void closeFile(FILE *fp)
{
    fclose(fp);
    fp = NULL;
    return;
}

// Function that receives a char * fileName
// Return a Intance * intance
Instance *handleInstance(char *fileName)
{
    Instance *instance = NULL;
    FILE *input = NULL;
    int aux = 0;

    input = openFileRead("instance.txt", "r");

    instance = malloc(sizeof(instance) * sizeof(Instance));
    instance->nVarTotal = 0;

    fgets(instance->name, 120, input);

    fscanf(input, " %d %d", &instance->nVar, &instance->nClauses);

    instance->clauses =  malloc(sizeof(Clause*) * instance->nClauses);

    for (int i = 0; i < instance->nClauses; i++)
    {

        fscanf(input, " %1d", &aux);
        instance->nVarTotal = instance->nVarTotal + aux;
        instance->clauses[i].clause =  malloc(sizeof(int) * sizeof(instance->clauses[i]));
        instance->clauses[i].clause[0] = aux;

        for (int j = 1; j <= aux; j++)
        {

            fscanf(input, " %d", &instance->clauses[i].clause[j]);
            printf("%d ", instance->clauses[i].clause[j]);

        }
            printf("\n");
    }
        instance->nVarTotal = instance->nVarTotal + 1;

    fscanf(input, " %1d", &aux);
    instance->mandatory =  malloc(sizeof(int*) * (aux + 1));
    instance->mandatory[0] = aux;

    for (int j = 1; j <= aux; j++)
    {

        fscanf(input, " %d", &instance->mandatory[j]);
        printf("%d ", instance->mandatory[j]);

    }
    printf("\n");


    instance->cost =  malloc(sizeof(int*) * instance->nVar);

    for (int j = 0; j < instance->nVar; j++)
    {

        fscanf(input, " %d", &instance->cost[j]);
        printf("%d ", instance->cost[j]);

    }
    printf("\n");

    instance->benefits =  malloc(sizeof(int*) * instance->nVar);

    for (int j = 0; j < instance->nVar; j++)
    {

        fscanf(input, " %d", &instance->benefits[j]);
        printf("%d ", instance->benefits[j]);

    }

    printf("\n");

    fscanf(input, " %d", &instance->budget);
    printf("%d ", instance->budget);
    printf("\n\n");
    closeFile(input);

    return instance;
}


//printInstance
void printInstance (Instance* instance)
{
    Id newId;
    int aux = 1;

    for (int i = 0; i < instance->nClauses; i++)
    {
        for (int j = 1; j <= instance->clauses[i].clause[0]; j++)
        {
            newId.id = aux;
            newId.value = instance->clauses[i].clause[j];
            printf("%d ",  newId.value);
            newAdjacencyListNode(newId);
            aux++;
        }
            printf("\n");
    }

    for (int j = 1; j <= instance->mandatory[0]; j++)
    {

        printf("%d ", instance->mandatory[j]);

    }
    printf("\n");

    for (int j = 0; j < instance->nVar; j++)
    {

        printf("%d ", instance->cost[j]);

    }
    printf("\n");

    for (int j = 0; j < instance->nVar; j++)
    {
        printf("%d ", instance->benefits[j]);
    }
    printf("\n");
    printf("%d ", instance->budget);
    printf("\n\n");


    return;
}


//exitApplication
static void exitApplication(Instance* instance)
{
    free(instance->cost);
    free(instance->mandatory);
    free(instance->benefits);
    for (int i = 0; i < instance->nClauses; i++)
    {

        free(instance->clauses[i].clause);
    }
    free(instance->clauses);
    free(instance);
    instance = NULL;

    return;
}


//instanceToGraph
void instanceToGraph(Instance* instance, Graph* graph)
{
    Id idNew_a;
    Id idNew_b;
    int aux1 = 0;
    int aux2 = 0;

    for (int i = 0; i < instance->nClauses; i++)
    {

        for (int j = 1; j <= instance->clauses[i].clause[0]; j++)
        {
            aux2 = 0;
            for (int k = i + 1; k < instance->nClauses; k++)
            {
                aux2 = aux2 + instance->clauses[k - 1].clause[0];


                for (int l = 1; l <= instance->clauses[k].clause[0]; l++)
                {
                    int a = instance->clauses[i].clause[j] == instance->clauses[k].clause[l];
                    int b = instance->clauses[i].clause[j] > instance->nVar &&
                    instance->clauses[i].clause[j] - instance->nVar !=  instance->clauses[k].clause[l];
                    int c = instance->clauses[k].clause[l] > instance->nVar &&
                    instance->clauses[k].clause[l] - instance->nVar !=  instance->clauses[i].clause[j];
                    int d = instance->clauses[i].clause[j] <= instance->nVar &&
                    instance->clauses[k].clause[l] <= instance->nVar &&
                    instance->clauses[i].clause[j] != instance->clauses[k].clause[l];

                    if( a || b || c || d)
                    {
                       idNew_a.id = aux1 + j;
                       idNew_a.value = instance->clauses[i].clause[j];
                       idNew_b.id = aux1 + aux2 + l;
                       idNew_b.value = instance->clauses[k].clause[l];
                       addEdge(graph, idNew_a, idNew_b);
                    }
                    else{}

                }


            }
        }
        aux1 = aux1 + instance->clauses[i].clause[0];
    }

    printGraph(graph);

    return;
}

// A utility function to create a new adjacency list node
adjacencyNode* newAdjacencyListNode(Id idNext)
{
    adjacencyNode* newNode = malloc(sizeof(adjacencyNode));

    newNode->idNext = idNext;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
Graph* createGraph(int nNodes)
{
    Graph* graph = malloc(sizeof(Graph));
    graph->nNodes = nNodes;

    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array = malloc(nNodes * sizeof(graph->array[0]));
    adjacencyList* newList = NULL;
    newList = malloc(sizeof * newList);
    newList->head = NULL;
    newList->idMy.id = 0;
    newList->idMy.value = 0;


    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < nNodes; i++)
        graph->array[i] = *newList;

    free(newList);
    newList = NULL;

    return graph;
}



// Adds an edge to an undirected graph
void addEdge(Graph* graph, Id actual, Id idNext)
{


    adjacencyNode* newNode = newAdjacencyListNode(idNext);
    graph->array[actual.id].idMy = actual;
    newNode->next = graph->array[actual.id].head;
    graph->array[actual.id].head = newNode;

    newNode = newAdjacencyListNode(actual);
    graph->array[idNext.id].idMy = idNext;
    newNode->next = graph->array[idNext.id].head;
    graph->array[idNext.id].head = newNode;
}


//printGraph
void printGraph(Graph* graph)
{
    int v;
    for (v = 0; v < graph->nNodes; v++)
    {
        adjacencyNode* pCrawl = graph->array[v].head;
        if (graph->array[v].idMy.value == 0)
        {
            continue;
        }
        printf("\n Adjacency list of vertex %d and Id %d\n head ", graph->array[v].idMy.id, graph->array[v].idMy.value);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->idNext.value);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}
