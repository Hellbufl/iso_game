#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "util.h"

#define TARGET_CAPACITY_FACTOR 2
#define MAX_CAPACITY_FACTOR 4

#define ERR_ASTAR -1
#define ERR_IN_ARR -2
#define ERR_OUT_ARR -3
#define ERR_INDEX -4

typedef struct Node
{
    int x;
    int y;

    int G;
    int H;
    int F;

    int walkable;
    
    struct Node* parent;

} Node;

typedef struct NodeArray
{
    struct Node** elements;
    int len;
    int cap;

} NodeArray;

typedef struct AStar
{
    Node*** field;
    int fWidth;
    int fHeight;

    Node* start;
    Node* dest;
    NodeArray* openHeap;
    NodeArray* closedNArr;

    int stepcount;
    int found;

} AStar;

// -- A* functions -- //
AStar*      AStar_Init(Node*** initField, int initWidth, int initHeight);
void        AStar_Reset(AStar* aStar);
void        AStar_Destroy(AStar* aStar);
NodeArray*  AStar_Search(AStar* aStar, NodeArray* outPath);
NodeArray*  AStar_GetNeighbours(AStar* aStar, Node* node, NodeArray* out);
void        AStar_Step(AStar* aStar);
void        AStar_Backtrack(AStar* aStar, NodeArray* path);
NodeArray*  AStar_FindDirectPath(AStar* aStar, NodeArray* path, NodeArray* outDirectPath);
NodeArray*  AStar_FindDirectPathAt(AStar* aStar, NodeArray* path, int first, int last, NodeArray* outDirectPath);
int         AStar_Raycast(AStar* aStar, Node* start, Node* end);

// -- Node functions -- //
Node*       Node_Init();
void        Node_Reset(Node* node);
int         Node_CalG(Node* node, Node* parent);
int         Node_CalH(Node* node, Node* dest);
void        Node_CalF(Node* node, Node* parent, Node* dest);
void        Node_Swap(Node** e1, Node** e2);

// -- NodeArray functions -- //
NodeArray*  NArr_Init(int capacity);
void        NArr_Expand(NodeArray* narr, int add_cap);
void        NArr_Balance(NodeArray* narr);
void        NArr_Destroy(NodeArray* narr);
void        NArr_Obliterate(NodeArray* narr);
void        NArr_Clear(NodeArray* narr);
void        NArr_Set(NodeArray* narr, int i, Node* node);
void        NArr_Add(NodeArray* narr, Node* node);
void        NArr_Remove(NodeArray* narr, Node* node);
int         NArr_Contains(NodeArray* narr, Node* node);
void        NArr_Append(NodeArray* narr_1, int i, NodeArray* narr_2, int k);
NodeArray*  NArr_Join(NodeArray* narr_1, int i, NodeArray* narr_2, int k);
void        NArr_Reverse(NodeArray* narr);
void        NArr_Print(NodeArray* narr);

// -- MinHeap functions -- //
void        Heap_SortUp(NodeArray* heap, int i);
void        Heap_SortDown(NodeArray* heap);
void        Heap_Add(NodeArray* heap, Node* node);
Node*       Heap_Take(NodeArray* heap);
int         Heap_Height(NodeArray* heap);
int         Heap_Compare(NodeArray* heap, int i, int k);
void        Heap_Show(NodeArray* heap);