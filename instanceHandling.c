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
    struct adjacencyLists* head;
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
void instanceToGraph(Instance* instance);

// Test Unit
int main(int argc, char **argv)
{

    Instance* instance = NULL;
    instance = handleInstance("instance.txt");
    // printInstance(instance);
    instanceToGraph(instance);
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
    instance = malloc(sizeof(instance));

    fgets(instance->name, 120, input);
    fscanf(input, " %d %d", &instance->nVar, &instance->nClauses);

    instance->clauses =  malloc(sizeof(int*) * instance->nClauses * sizeof(instance->clauses) * 100);

    for (int i = 0; i < instance->nClauses; i++)
    {

        fscanf(input, " %1d", &aux);
        instance->clauses[i] =  malloc(sizeof(int) * sizeof(instance->clauses[i]) * (aux + 100));
        instance->clauses[i][0] = aux;

        for (int j = 1; j <= aux; j++)
        {

            fscanf(input, " %d", &instance->clauses[i][j]);
            printf("%d ", instance->clauses[i][j]);

        }
            printf("\n");
    }

    fscanf(input, " %1d", &aux);
    instance->mandatory =  malloc(sizeof(int*) * aux * 1000);
    instance->mandatory[0] = aux;

    for (int j = 1; j <= aux; j++)
    {

        fscanf(input, " %d", &instance->mandatory[j]);
        printf("%d ", instance->mandatory[j]);

    }
    printf("\n");

//!!
    instance->cost =  malloc(sizeof(int*) * instance->nVar * 1000);

    for (int j = 0; j < instance->nVar; j++)
    {

        fscanf(input, " %d", &instance->cost[j]);
        printf("%d ", instance->cost[j]);

    }
    printf("\n");

    instance->benefits =  malloc(sizeof(int*) * instance->nVar * 1000);

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

            printf("%d ", instance->clauses[i][j]);

        }
            printf("\n");
    }

//    for (int j = 1; j <= instance->mandatory[0]; j++)
  //  {

    //    printf("%d ", instance->mandatory[j]);

   // }
   // printf("\n");

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
    free(instance);

    instance = NULL;

    return;
}

void instanceToGraph(Instance* instance)
{
    for (int i = 0; i < instance->nClauses; i++)
    {
        for (int j = 1; j <= instance->clauses[i][0]; j++)
        {

            printf("%d ", instance->clauses[i][j]);

        }
            printf("\n");
    }  
    
    return;
}