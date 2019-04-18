#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Instances
{
    char name[120];
    int nVar, nClauses;
    int **clauses;
    int *mandatory;
    int *cost;
    int *benefits;
    int budget;
} Instance;


typedef struct adjacencyNodes
{
    int idNext;
    struct adjacencyNodes* next;
}adjacencyNode;

typedef struct adjacencyLists
{
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
adjacencyNode* newAdjacencyListNode(int idNext);
Graph* createGraph(int nNodes);
void addEdge(Graph* graph, int actual, int idNext);
void printGraph(Graph* graph);


// Test Unit
int main(int argc, char **argv)
{

    Instance* instance = NULL;
    Graph* graph = NULL;
    instance = handleInstance("instance.txt");
    graph = createGraph(instance->nVar);

  //  printInstance(instance);
    instanceToGraph(instance, graph);
    exitApplication(instance);

    return 0;
}

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
    fgets(instance->name, 120, input);

    fscanf(input, " %d %d", &instance->nVar, &instance->nClauses);

    instance->clauses =  malloc(sizeof(int*) * instance->nClauses);

    for (int i = 0; i < instance->nClauses; i++)
    {

        fscanf(input, " %1d", &aux);
        instance->clauses[i] =  malloc(sizeof(int) * sizeof(instance->clauses[i]));
        instance->clauses[i][0] = aux;

        for (int j = 1; j <= aux; j++)
        {

            fscanf(input, " %d", &instance->clauses[i][j]);
            printf("%d ", instance->clauses[i][j]);

        }
            printf("\n");
    }

    fscanf(input, " %1d", &aux);
    instance->mandatory =  malloc(sizeof(int*) * aux);
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

void printInstance (Instance* instance)
{

    for (int i = 0; i < instance->nClauses; i++)
    {
        for (int j = 1; j <= instance->clauses[i][0]; j++)
        {

            //printf("%d ", instance->clauses[i][j]);
            newAdjacencyListNode(instance->clauses[i][j]);
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

static void exitApplication(Instance* instance)
{
    free(instance->cost);
    free(instance->mandatory);
    free(instance->benefits);
    for (int i = 0; i < instance->nClauses; i++)
    {

        free(instance->clauses[i]);
    }
    free(instance->clauses);
    free(instance);
    instance = NULL;

    return;
}

void instanceToGraph(Instance* instance, Graph* graph)
{

    for (int i = 0; i < instance->nClauses; i++)
    {
        for (int j = 1; j <= instance->clauses[i][0]; j++)
        {

            for (int k = 0; k < instance->nClauses; k++)
            {
                for (int l = 1; l <= instance->clauses[k][0]; l++)
                {
                    if( i == k)
                    {
                        continue;
                    }

                    if(instance->clauses[i][j] != instance->clauses[k][l])
                    {
                        addEdge(graph, instance->clauses[i][j], instance->clauses[k][l]);
                    }


                }
              }


        }
    }

    printGraph(graph);
    return;
}

// A utility function to create a new adjacency list node
adjacencyNode* newAdjacencyListNode(int idNext)
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
    graph->array = malloc(nNodes * sizeof(adjacencyList));

    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < nNodes; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(Graph* graph, int actual, int idNext)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the begining
    adjacencyNode* newNode = newAdjacencyListNode(idNext);
    newNode->next = graph->array[actual].head;
    graph->array[actual].head = newNode;

    // Since graph is undirected, add an edge from
    // dest to src also
    newNode = newAdjacencyListNode(actual);
    newNode->next = graph->array[idNext].head;
    graph->array[idNext].head = newNode;
}

void printGraph(Graph* graph)
{
    int v;
    for (v = 0; v < graph->nNodes; ++v)
    {
        adjacencyNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->idNext);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}
