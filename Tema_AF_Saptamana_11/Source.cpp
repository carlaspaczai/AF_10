/*
* Student: Spaczai Carla Noemi
* Grupa: 30229
* Am implementat algoritmul de cautare in adancime (Depth-First Search - DFS), algoritmul Tarjan pentru componente tare conexe si sortarea topologica.
* 
* DFS:
* Functia DFS parcurge toate nodurile, iar pentru cele albe apeleaza functia DFSVisit.
* Functia DFSVisit ii ofera nodului timpul la care a fost descoperit, il coloreaza in gri si parcurge toate nodurile adiacente acestuia in mod recursiv, prin
* intermediul functiei DFSVisit. Cand toate nodurile adiacente acestuia au fost vizitate, nodul este colorat in negru si primeste timpul la care s-a finalizat vizita
* lui.
* Complexitate: O(V + E).
* 
* Sortarea Topologica:
* Nodurile grafului vor fi sortate in functie de timpul de finalizare al vizitei (ultimul nod ca finalizare a vizitei va fi primul in sirul sortat). Pentru a realiza
* acest lucru ne folosim de o lista in care inseram la inceput fiecare nod in momentul in care a devenit negru. La final, obtinem nodurile sortate in lista (de la
* inceput la final).
* Daca graful nu este aciclic, nu putem sa efectuam sortarea topologica.
* Complexitate: O(V + E).
* 
* Tarjan:
* Acest algoritm presupune o parcurgere DFS cu ajutorul unei stive si a unor atribute (Index, lowLink, onStack, Component).
* Functia Tarjan parcurge toate nodurile, iar pentru cele care au Index = -1 (nu au fost descoperite) se apeleaza functia TarjanSC.
* Functia TarjanSC pune pe stiva nodul primit, ii ofera Index-ului si lowLink-ului valoarea Index-ului declarat in afara functiei si creste valoarea Index-ului
* declarat in afara functiei cu 1. Se verifica toate nodurile adiacente acestuia, iar daca Index-ul nodului adiacent este -1, se apeleaza in mod recursiv functia
* TarjanSC pentru acest nod si lowLink-ul nodului initial ia valoarea lowLink-ului nodului adiacent daca este mai mic. Daca Index-ul nodului adiacent nu este -1,
* dar acesta se afla pe stiva, lowLink-ul nodului initial ia valoarea Index-ului nodului adiacent daca este mai mic. La final, daca lowLink-ul si Index-ul nodului
* initial au valori egale, toate elementele extrase din stiva pana la nodul initial (inclusiv) fac parte din aceeasi componenta tare conexa.
* Complexitate: O(V + E).
*/

#include <stdio.h>
#include "Profiler.h"

Profiler P("DFS");

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct List {
    struct Node* First;
} List;

typedef struct Node {
    int adjSize;
    struct Node** Adj;
    int Color;
    int Discovered;
    int Finished;
    int Data;
    struct Node* Parent;
    struct Node* Next;
    int Index;
    int lowLink;
    bool onStack;
    int Component;
} Node;

typedef struct Graph {
    int numberNodes;
    struct Node** V;
    struct List* topologicalSort;
} Graph;

typedef struct Stack {
    int Top;
    struct Node** V;
} Stack;

Node* createNode(int Data, int nrNodes)
{
    Node* N = (Node*)malloc(sizeof(Node));
    N->adjSize = 0;
    N->Adj = (Node**)malloc(nrNodes * sizeof(Node*));
    N->Color = COLOR_WHITE;
    N->Discovered = 0;
    N->Finished = 0;
    N->Data = Data;
    N->Parent = NULL;
    N->Next = NULL;
    N->Index = -1;
    N->lowLink = -1;
    N->onStack = FALSE;
    N->Component = 0;
    return N;
}

int T;

void DFSVisit(Graph* G, Node* N)
{
    T = T + 1;
    N->Discovered = T;
    N->Color = COLOR_GRAY;
    for (int i = 0; i < N->adjSize; i++)
    {
        if (N->Adj[i]->Color == COLOR_WHITE)
        {
            N->Adj[i]->Parent = N;
            DFSVisit(G, N->Adj[i]);
        }
    }
    N->Color = COLOR_BLACK;
    T = T + 1;
    N->Finished = T;
}

void DFS(Graph* G)
{
    for (int i = 0; i < G->numberNodes; i++)
    {
        G->V[i]->Color = COLOR_WHITE;
        G->V[i]->Parent = NULL;
    }
    T = 0;
    for (int i = 0; i < G->numberNodes; i++)
    {
        if (G->V[i]->Color == COLOR_WHITE)
        {
            DFSVisit(G, G->V[i]);
        }
    }
}

void insertFirstList(List* L, Node* N)
{
    if (L->First != NULL)
    {
        N->Next = L->First;
    }
    L->First = N;
}

void DFSVisitTS(Graph* G, Node* N)
{
    T = T + 1;
    N->Discovered = T;
    N->Color = COLOR_GRAY;
    for (int i = 0; i < N->adjSize; i++)
    {
        if (N->Adj[i]->Color == COLOR_WHITE)
        {
            N->Adj[i]->Parent = N;
            DFSVisitTS(G, N->Adj[i]);
        }
    }
    N->Color = COLOR_BLACK;
    T = T + 1;
    N->Finished = T;
    insertFirstList(G->topologicalSort, N);
}

List* DFSTS(Graph* G)
{
    for (int i = 0; i < G->numberNodes; i++)
    {
        G->V[i]->Color = COLOR_WHITE;
        G->V[i]->Parent = NULL;
    }
    T = 0;
    for (int i = 0; i < G->numberNodes; i++)
    {
        if (G->V[i]->Color == COLOR_WHITE)
        {
            DFSVisitTS(G, G->V[i]);
        }
    }
    Node* F = G->topologicalSort->First->Next;
    while (F != NULL)
    {
        Node* FF = G->topologicalSort->First;
        for (int i = 0; i < F->adjSize; i++)
        {
            int W = 0;
            while (FF != F)
            {
                if (F->Adj[i] == FF)
                {
                    W = 1;
                }
                FF = FF->Next;
            }
            if (W == 1)
            {
                return NULL;
            }
        }
        F = F->Next;
    }
    return G->topologicalSort;
}

int Index = 0, numberComponents = 0;

Stack* createStack(int numberNodes)
{
    Stack* S = (Stack*)malloc(sizeof(Stack));
    S->Top = 0;
    S->V = (Node**)malloc(numberNodes * sizeof(Node*));
    return S;
}

void Push(Stack* S, Node* N)
{
    S->V[S->Top] = N;
    S->Top = S->Top + 1;
}

Node* Pop(Stack* S)
{
    S->Top = S->Top - 1;
    return S->V[S->Top];
}

void TarjanSC(Graph* G, Stack* S, Node* N)
{
    N->Index = Index;
    N->lowLink = Index;
    Index = Index + 1;
    Push(S, N);
    N->onStack = TRUE;
    for (int i = 0; i < N->adjSize; i++)
    {
        if (N->Adj[i]->Index == -1)
        {
            TarjanSC(G, S, N->Adj[i]);
            if (N->lowLink > N->Adj[i]->lowLink)
            {
                N->lowLink = N->Adj[i]->lowLink;
            }
        }
        else if (N->Adj[i]->onStack == TRUE)
        {
            if (N->lowLink > N->Adj[i]->Index)
            {
                N->lowLink = N->Adj[i]->Index;
            }
        }
    }
    Node* Z;
    if (N->lowLink == N->Index)
    {
        numberComponents = numberComponents + 1;
        do {
            Z = Pop(S);
            Z->onStack = FALSE;
            Z->Component = numberComponents;
        } while (Z != N);
    }
}

void Tarjan(Graph* G, Stack* S)
{
    for (int i = 0; i < G->numberNodes; i++)
    {
        G->V[i]->Index = -1;
        G->V[i]->lowLink = -1;
        G->V[i]->onStack = FALSE;
        G->V[i]->Component = 0;
    }
    for (int i = 0; i < G->numberNodes; i++)
    {
        if (G->V[i]->Index == -1)
        {
            TarjanSC(G, S, G->V[i]);
        }
    }
}

void demo()
{
    Graph* G1 = (Graph*)malloc(sizeof(Graph));
    G1->numberNodes = 10;
    G1->V = (Node**)malloc(G1->numberNodes * sizeof(Node*));
    for (int i = 0; i < G1->numberNodes; i++)
    {
        G1->V[i] = createNode(i, G1->numberNodes);
    }
    G1->V[0]->adjSize = 2;
    G1->V[0]->Adj[0] = G1->V[1];
    G1->V[0]->Adj[1] = G1->V[2];
    G1->V[1]->adjSize = 2;
    G1->V[1]->Adj[0] = G1->V[2];
    G1->V[1]->Adj[1] = G1->V[4];
    G1->V[3]->adjSize = 2;
    G1->V[3]->Adj[0] = G1->V[1];
    G1->V[3]->Adj[1] = G1->V[5];
    G1->V[4]->adjSize = 1;
    G1->V[4]->Adj[0] = G1->V[4];
    G1->V[5]->adjSize = 3;
    G1->V[5]->Adj[0] = G1->V[6];
    G1->V[5]->Adj[1] = G1->V[7];
    G1->V[5]->Adj[2] = G1->V[8];
    G1->V[7]->adjSize = 1;
    G1->V[7]->Adj[0] = G1->V[8];
    G1->V[8]->adjSize = 1;
    G1->V[8]->Adj[0] = G1->V[5];
    G1->V[9]->adjSize = 1;
    G1->V[9]->Adj[0] = G1->V[8];
    printf("DFS:\n");
    DFS(G1);
    for (int i = 0; i < G1->numberNodes; i++)
    {
        printf("Data: %d, Discovered: %d, Finished: %d, Color: %d, ", G1->V[i]->Data, G1->V[i]->Discovered, G1->V[i]->Finished, G1->V[i]->Color);
        if (G1->V[i]->Parent == NULL)
        {
            printf("Parent: NULL");
        }
        else
        {
            printf("Parent: %d", G1->V[i]->Parent->Data);
        }
        printf("\n");
    }
    printf("\n");
    Graph* G2 = (Graph*)malloc(sizeof(Graph));
    G2->numberNodes = 10;
    G2->topologicalSort = (List*)malloc(sizeof(List));
    G2->topologicalSort->First = NULL;
    G2->V = (Node**)malloc(G2->numberNodes * sizeof(Node*));
    for (int i = 0; i < G2->numberNodes; i++)
    {
        G2->V[i] = createNode(i, G2->numberNodes);
    }
    G2->V[0]->adjSize = 2;
    G2->V[0]->Adj[0] = G2->V[1];
    G2->V[0]->Adj[1] = G2->V[2];
    G2->V[1]->adjSize = 2;
    G2->V[1]->Adj[0] = G2->V[2];
    G2->V[1]->Adj[1] = G2->V[4];
    G2->V[3]->adjSize = 2;
    G2->V[3]->Adj[0] = G2->V[1];
    G2->V[3]->Adj[1] = G2->V[5];
    G2->V[4]->adjSize = 1;
    G2->V[4]->Adj[0] = G2->V[4];
    G2->V[5]->adjSize = 3;
    G2->V[5]->Adj[0] = G2->V[6];
    G2->V[5]->Adj[1] = G2->V[7];
    G2->V[5]->Adj[2] = G2->V[8];
    G2->V[7]->adjSize = 1;
    G2->V[7]->Adj[0] = G2->V[8];
    //G2->V[8]->adjSize = 1;
    //G2->V[8]->Adj[0] = G2->V[5];
    G2->V[9]->adjSize = 1;
    G2->V[9]->Adj[0] = G2->V[8];
    printf("Topological Sort:\n");
    G2->topologicalSort = DFSTS(G2);
    if (G2->topologicalSort != NULL)
    {
        Node* F = G2->topologicalSort->First;
        while (F != NULL)
        {
            printf("%d ", F->Data);
            F = F->Next;
        }
    }
    else
    {
        printf("NO! WE CAN'T!");
    }
    printf("\n");
    Graph* G3 = (Graph*)malloc(sizeof(Graph));
    G3->numberNodes = 10;
    G3->V = (Node**)malloc(G3->numberNodes * sizeof(Node*));
    for (int i = 0; i < G3->numberNodes; i++)
    {
        G3->V[i] = createNode(i, G3->numberNodes);
    }
    G3->V[0]->adjSize = 1;
    G3->V[0]->Adj[0] = G3->V[2];
    G3->V[1]->adjSize = 1;
    G3->V[1]->Adj[0] = G3->V[8];
    G3->V[2]->adjSize = 1;
    G3->V[2]->Adj[0] = G3->V[5];
    G3->V[3]->adjSize = 1;
    G3->V[3]->Adj[0] = G3->V[6];
    G3->V[4]->adjSize = 1;
    G3->V[4]->Adj[0] = G3->V[6];
    G3->V[5]->adjSize = 2;
    G3->V[5]->Adj[0] = G3->V[0];
    G3->V[5]->Adj[1] = G3->V[7];
    G3->V[7]->adjSize = 1;
    G3->V[7]->Adj[0] = G3->V[5];
    G3->V[8]->adjSize = 2;
    G3->V[8]->Adj[0] = G3->V[4];
    G3->V[8]->Adj[1] = G3->V[1];
    G3->V[9]->adjSize = 2;
    G3->V[9]->Adj[0] = G3->V[0];
    G3->V[9]->Adj[1] = G3->V[7];
    printf("\nTarjan:\n");
    Stack* S = createStack(G3->numberNodes);
    Tarjan(G3, S);
    for (int i = 1; i <= numberComponents; i++)
    {
        for (int j = 0; j < G3->numberNodes; j++)
        {
            if (G3->V[j]->Component == i)
            {
                printf("%d ", G3->V[j]->Data);
            }
        }
        printf("\n");
    }
}

void DFSVisitP(Graph* G, Node* N, Operation* OP)
{
    int operatii = 0;
    T = T + 1;
    operatii += 2;
    N->Discovered = T;
    N->Color = COLOR_GRAY;
    for (int i = 0; i < N->adjSize; i++)
    {
        operatii += 1;
        if (N->Adj[i]->Color == COLOR_WHITE)
        {
            operatii += 1;
            N->Adj[i]->Parent = N;
            DFSVisitP(G, N->Adj[i], OP);
        }
    }
    operatii += 1;
    N->Color = COLOR_BLACK;
    T = T + 1;
    operatii += 1;
    N->Finished = T;
    if (OP != NULL)
    {
        OP->count(operatii);
    }
}

void DFSP(Graph* G, Operation* OP)
{
    int operatii = 0;
    for (int i = 0; i < G->numberNodes; i++)
    {
        operatii += 2;
        G->V[i]->Color = COLOR_WHITE;
        G->V[i]->Parent = NULL;
    }
    T = 0;
    for (int i = 0; i < G->numberNodes; i++)
    {
        operatii += 1;
        if (G->V[i]->Color == COLOR_WHITE)
        {
            DFSVisitP(G, G->V[i], OP);
        }
    }
    if (OP != NULL)
    {
        OP->count(operatii);
    }
}

void generate_random_edges(Graph* G, int GRE)
{
    for (int j = 0; j < GRE; j++)
    {
        int a = rand() % G->numberNodes;
        int b;
        do
        {
            b = rand() % G->numberNodes;
        } while (a == b);
        int w = 0;
        while (w == 0)
        {
            w = 1;
            for (int z = 0; z < G->V[a]->adjSize; z++)
            {
                if (G->V[a]->Adj[z] == G->V[b])
                {
                    w = 0;
                }
            }
            if (w == 0)
            {
                b = rand() % G->numberNodes;
            }
        }
        G->V[a]->Adj[G->V[a]->adjSize] = G->V[b];
        G->V[a]->adjSize = G->V[a]->adjSize + 1;
    }
}

void perf()
{
    int n, i;
    for (n = 1000; n <= 4500; n += 100) {
        Operation OP = P.createOperation("DFS Edges", n);
        Graph graph;
        graph.numberNodes = 100;
        graph.V = (Node**)malloc(graph.numberNodes * sizeof(Node*));
        for (i = 0; i < graph.numberNodes; ++i)
        {
            graph.V[i] = (Node*)malloc(sizeof(Node));
            memset(graph.V[i], 0, sizeof(Node));
            graph.V[i]->Adj = (Node**)malloc(graph.numberNodes * sizeof(Node*));
            for (int j = 0; j < graph.numberNodes; j++)
            {
                graph.V[i]->Adj[j] = (Node*)malloc(sizeof(Node));
            }
        }
        generate_random_edges(&graph, n);
        DFSP(&graph, &OP);
    }
    for (n = 100; n <= 200; n += 10) {
        Operation OP = P.createOperation("DFS Vertices", n);
        Graph G;
        G.numberNodes = n;
        G.V = (Node**)malloc(G.numberNodes * sizeof(Node*));
        for (i = 0; i < G.numberNodes; ++i)
        {
            G.V[i] = (Node*)malloc(sizeof(Node));
            memset(G.V[i], 0, sizeof(Node));
            G.V[i]->Adj = (Node**)malloc(G.numberNodes * sizeof(Node*));
            for (int j = 0; j < G.numberNodes; j++)
            {
                G.V[i]->Adj[j] = (Node*)malloc(sizeof(Node));
            }
        }
        generate_random_edges(&G, 4500);
        DFSP(&G, &OP);
    }
    P.showReport();
}

int main()
{
    demo();
    //perf();
	return 0;
}