#include "astar.h"

int powi(int basis, int exponent)
{
    int result = 1;

    for (int i = 0; i < exponent; i++)
        result *= basis;

    return result;
}

////////////////////////
// -- A* functions -- //
////////////////////////

// not for resetting
AStar* AStar_Init(Node*** initField, int initWidth, int initHeight)
{
    AStar* aStar = (AStar*) calloc(1, sizeof(AStar));

    aStar->field = initField;
    aStar->fWidth = initWidth;
    aStar->fHeight = initHeight;

    aStar->openHeap = NArr_Init(10);
    aStar->closedNArr = NArr_Init(10);

    return aStar;
}

void AStar_Reset(AStar* aStar)
{
    if (aStar == NULL) return;

    aStar->found = 0;
    aStar->stepcount = 0;

    NArr_Clear(aStar->openHeap);
    NArr_Clear(aStar->closedNArr);

    Heap_Add(aStar->openHeap, aStar->start);

    for (int ix = 0; ix < aStar->fWidth; ix++)
        for (int iy = 0; iy < aStar->fHeight; iy++)

            Node_Reset(aStar->field[ix][iy]);
}

void AStar_Destroy(AStar* aStar)
{
    if (aStar == NULL) return;
    NArr_Destroy(aStar->openHeap);
    NArr_Destroy(aStar->closedNArr);
    free(aStar);
}

NodeArray* AStar_Search(AStar* aStar, NodeArray* outPath)
{
    if (outPath == NULL) { errno = EINVAL; return (NodeArray*) ERR_OUT_ARR; }

    while (!aStar->found)
    {
        AStar_Step(aStar);
    }

    AStar_Backtrack(aStar, outPath);

    return outPath;
}

// returns number of neighbours found 
NodeArray* AStar_GetNeighbours(AStar* aStar, Node* node, NodeArray* out)
{
    if (aStar == NULL)  { errno = EINVAL; return (NodeArray*) ERR_ASTAR; }
    if (node == NULL)   { errno = EINVAL; return (NodeArray*) NULL; }

    for (int i = 0; i < 9; i++)
    {
        int x = node->x + (i % 3) - 1;
        int y = node->y + i / 3 - 1;

        if (x == node->x && y == node->y) continue;

        if (x < 0) continue;
        if (y < 0) continue;

        if (x >= aStar->fWidth) continue;
        if (y >= aStar->fHeight) continue;

        Node* currentNode = aStar->field[x][y];

        if (!currentNode->walkable) continue;

        if (NArr_Contains(aStar->closedNArr, currentNode)) continue;

        // check the two direct adjacent nodes so you dont cut corners
        if (x != node->x && y != node->y)
        {
            Node* xAdjacent = aStar->field[node->x][y];
            Node* yAdjacent = aStar->field[x][node->y];

            if (!(xAdjacent->walkable && yAdjacent->walkable)) continue;
        }

        NArr_Add(out, currentNode);
    }

    return out;
}

void AStar_Step(AStar* aStar)
{
    if (aStar == NULL) return;

    aStar->stepcount++;

    Node* node = Heap_Take(aStar->openHeap);
    Node* currentNeighbour;
    int old_F;

    NArr_Add(aStar->closedNArr, node);

    if (node == aStar->dest)
    {
        aStar->found = 1;
        return;
    }

    NodeArray* neighbours = NArr_Init(9);
    AStar_GetNeighbours(aStar, node, neighbours);

    for (int i = 0; i < neighbours->len; i++)
    {
        currentNeighbour = neighbours->elements[i];
        old_F = currentNeighbour->F;

        Node_CalF(currentNeighbour, node, aStar->dest);

        if (currentNeighbour->F != old_F)
            Heap_SortDown(aStar->openHeap);
        
        if (!NArr_Contains(aStar->openHeap, currentNeighbour))
            Heap_Add(aStar->openHeap, currentNeighbour);
    }
    NArr_Destroy(neighbours);
}

void AStar_Backtrack(AStar* aStar, NodeArray* path)
{
    for (Node* np = aStar->dest; np != aStar->start; np = np->parent)
        NArr_Add(path, np);

    NArr_Add(path, aStar->start);
    NArr_Reverse(path);
}

NodeArray* AStar_FindDirectPath(AStar* aStar, NodeArray* path, NodeArray* outDirectPath)
{
    return AStar_FindDirectPathAt(aStar, path, 0, -1, outDirectPath);
}

NodeArray* AStar_FindDirectPathAt(AStar* aStar, NodeArray* path, int first, int last, NodeArray* outDirectPath)
{
    if (aStar == NULL)                              { errno = EINVAL; return (NodeArray*) ERR_ASTAR; }
    if (path == NULL)                               { errno = EINVAL; return (NodeArray*) ERR_IN_ARR; }
    if (outDirectPath == NULL)                      { errno = EINVAL; return (NodeArray*) ERR_OUT_ARR; }
    if (first >= path->len || last >= path->len)    { errno = EINVAL; return (NodeArray*) ERR_INDEX; }

    while (first < 0) first += path->len;
    while (last < 0) last += path->len;

    Node* local_A = path->elements[first];
    Node* local_B = path->elements[last];

    NArr_Add(outDirectPath, local_A);
    NArr_Add(outDirectPath, local_B);

    int mid = (first + last) >> 1;
    Node* local_M = path->elements[mid];

    if (first == mid) return outDirectPath;
    if (AStar_Raycast(aStar, local_A, local_B)) return outDirectPath;

    int path1Free = AStar_Raycast(aStar, local_A, local_M);
    int path2Free = AStar_Raycast(aStar, local_M, local_B);

    if (path1Free && path2Free)
    {
        NArr_Clear(outDirectPath);
        NArr_Add(outDirectPath, local_A);
        NArr_Add(outDirectPath, local_M);
        NArr_Add(outDirectPath, local_B);

        return outDirectPath;
    }

    if (path1Free)
    {
        NodeArray* path2 = NArr_Init(path->len);
        
        AStar_FindDirectPathAt(aStar, path, mid, last, path2);

        if (AStar_Raycast(aStar, local_A, path2->elements[1]))
            NArr_Append(outDirectPath, 1, path2, 1);
        else
            NArr_Append(outDirectPath, 1, path2, 0);

        NArr_Destroy(path2);

        return outDirectPath;
    }

    if (path2Free)
    {
        NodeArray* path1 = NArr_Init(path->len);
        
        AStar_FindDirectPathAt(aStar, path, first, mid, path1);

        if (AStar_Raycast(aStar, path1->elements[path1->len - 2], local_B))
            NArr_Append(path1, -1, outDirectPath, 1);
        else
            NArr_Append(path1, path1->len, outDirectPath, 1);
        
        NArr_Clear(outDirectPath);
        NArr_Append(outDirectPath, 0, path1, 0);

        NArr_Destroy(path1);

        return outDirectPath;
    }

    NodeArray* path1 = NArr_Init(path->len);
    NodeArray* path2 = NArr_Init(path->len);

    AStar_FindDirectPathAt(aStar, path, first, mid, path1);
    AStar_FindDirectPathAt(aStar, path, mid, last, path2);

    if (AStar_Raycast(aStar, path1->elements[path1->len - 2], path2->elements[1]))
        NArr_Append(path1, -1, path2, 1);
    else
        NArr_Append(path1, path1->len, path2, 1);

    NArr_Clear(outDirectPath);
    NArr_Append(outDirectPath, 0, path1, 0);

    NArr_Destroy(path1);
    NArr_Destroy(path2);

    return outDirectPath;
}

int AStar_Raycast(AStar* aStar, Node* start, Node* end)
{
    if (aStar == NULL)  { errno = EINVAL; return ERR_ASTAR; }
    if (start == NULL)  { errno = EINVAL; return 0; }
    if (end == NULL)    { errno = EINVAL; return 0; }

    float vec[] = { (float) (end->x - start->x), (float) (end->y - start->y) };
    int step[] = { SIGNOF(vec[0]), SIGNOF(vec[1]) };

    // straight in y axis
    if (start->x == end->x)
    {
        for (int i = 0; i < abs(start->y - end->y); i++)
            if (!aStar->field[start->x][start->y + step[1] * i]->walkable) return 0;
        
        return 1;
    }

    // straight in x axis
    if (start->y == end->y)
    {
        for (int i = 0; i < abs(start->x - end->x); i++)
            if (!aStar->field[start->x + step[0] * i][start->y]->walkable) return 0;
        
        return 1;
    }

    float tan = vec[1] / vec[0];
    float cot = vec[0] / vec[1];

    Node* currentNode = start;

    // initial intersection points
    float v_isect[] = { 0.5f * (float) step[0], 0.5f * (float) step[1] * tan };
    float h_isect[] = { 0.5f * (float) step[0] * cot, 0.5f * (float) step[1] };

    while (currentNode != end)
    {
        float v_dist = sqrtf(v_isect[0] * v_isect[0] + v_isect[1] * v_isect[1]);
        float h_dist = sqrtf(h_isect[0] * h_isect[0] + h_isect[1] * h_isect[1]);
        
        // if (!node1->walkable) return 0;
        // if (!node2->walkable) return 0;

        if (v_dist < h_dist)
        {
            currentNode = aStar->field[currentNode->x + step[0]][currentNode->y];
            v_isect[0] += (float) step[0];
            v_isect[1] += (float) step[1] * tan;
        }
        else if (v_dist > h_dist)
        {
            currentNode = aStar->field[currentNode->x][currentNode->y + step[1]];
            h_isect[0] += (float) step[0] * cot;
            h_isect[1] += (float) step[1];
        }
        else
        {
            Node* node1 = aStar->field[currentNode->x + step[0]][currentNode->y];
            Node* node2 = aStar->field[currentNode->x][currentNode->y + step[1]];

            if (!node1->walkable) return 0;
            if (!node2->walkable) return 0;

            currentNode = aStar->field[currentNode->x + step[0]][currentNode->y + step[1]];
            v_isect[0] += step[0];
            v_isect[1] += step[1] * tan;
            h_isect[0] += step[0] * cot;
            h_isect[1] += step[1];
        }

        if (!currentNode->walkable) return 0;
    }

    return 1;
}

//////////////////////////
// -- Node functions -- //
//////////////////////////

Node* Node_Init()
{
    Node* newNode = (Node*) calloc(1, sizeof(Node));
    newNode->walkable = 1;
    newNode->parent = newNode;
    return newNode;
}

void Node_Reset(Node* node)
{
    node->G = 0;
    node->H = 0;
    node->F = 0;
    node->parent = node;
}

// calculate G, H and F values

int Node_CalG(Node* node, Node* parent)
{
    if (node->x == parent->x || node->y == parent->y)
        return parent->G + 10;
    
    return parent->G + 14;
}

int Node_CalH(Node* node, Node* dest)
{
    int delta_x = abs(dest->x - node->x);
    int delta_y = abs(dest->y - node->y);

    if (delta_x == delta_y)
        return 14 * delta_x;
    
    return 14 * MINOF(delta_x, delta_y) + 10 * abs(delta_y - delta_x);
}

void Node_CalF(Node* node, Node* parent, Node* dest)
{
    int new_G = Node_CalG(node, parent);

    if (new_G < node->G || node->G == 0)
    {
        node->G = new_G;
        node->parent = parent;
    }

    node->H = Node_CalH(node, dest);
    node->F = node->G + node->H;
}

void Node_Swap(Node** e1, Node** e2)
{
    Node* tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
}

///////////////////////////////
// -- NodeArray functions -- //
///////////////////////////////

NodeArray* NArr_Init(int capacity)
{
    NodeArray* narr = (NodeArray*) calloc(1, sizeof(NodeArray));

    if (narr == NULL) { return NULL; }

    narr->elements = (Node**) calloc(capacity, sizeof(Node*));
    narr->len = 0;
    narr->cap = capacity;

    return narr;
}

void NArr_Expand(NodeArray* narr, int add_cap)
{
    if (narr == NULL) return;

    if (add_cap <= 0) add_cap = narr->len;

    Node** new_elements = (Node**) calloc(narr->cap + add_cap, sizeof(Node*));
    narr->cap += add_cap;

    for (int i = 0; i < narr->len; i++)
        new_elements[i] = narr->elements[i];
    
    free(narr->elements);

    narr->elements = new_elements;
}

void NArr_Balance(NodeArray* narr)
{
    if (narr == NULL) return;
    if (narr->len == 0) return;
    if (narr->cap > narr->len && narr->cap < narr->len * MAX_CAPACITY_FACTOR) return;

    narr->cap = narr->len * TARGET_CAPACITY_FACTOR;
    Node** new_elements = (Node**) calloc(narr->cap, sizeof(Node*));

    for (int i = 0; i < narr->len; i++)
        new_elements[i] = narr->elements[i];
    
    free(narr->elements);

    narr->elements = new_elements;
}

// does NOT free the nodes
void NArr_Destroy(NodeArray* narr)
{
    if (narr == NULL) return;

    free(narr->elements);
    free(narr);
}

// does free the nodes
void NArr_Obliterate(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        free(narr->elements[i]);

    NArr_Destroy(narr);
}

// does NOT free the nodes
void NArr_Clear(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        narr->elements[i] = NULL;
    
    narr->len = 0;
}

void NArr_Set(NodeArray* narr, int i, Node* node)
{
    if (i < 0) return;
    if (i >= narr->len) return;

    narr->elements[i] = node;
}

void NArr_Add(NodeArray* narr, Node* node)
{
    if (narr->len >= narr->cap)
        NArr_Balance(narr);

    narr->elements[narr->len] = node;
    narr->len += 1;
}

// does NOT free the node
void NArr_Remove(NodeArray* narr, Node* node)
{
    for (int i = 0; i < narr->len; i++)

        if (narr->elements[i] == node)
            narr->elements[i] = NULL;
    
    NArr_Balance(narr);
}

int NArr_Contains(NodeArray* narr, Node* node)
{
    if (narr == NULL) return 0;

    for (int i = 0; i < narr->len; i++)
    {
        if (narr->elements[i] == node) return 1;
    }
    return 0;
}

void NArr_Append(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    if (i > narr_1->len) { return; }
    if (k >= narr_2->len) { return; }

    while (i < 0) i += narr_1->len;
    while (k < 0) k += narr_2->len;

    NArr_Expand(narr_1, narr_2->len);
    narr_1->len = i + narr_2->len - k;

    while (i < narr_1->cap && k < narr_2->len)
    {
        narr_1->elements[i] = narr_2->elements[k];
        i++;
        k++;
    }
}

NodeArray* NArr_Join(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    NodeArray* new_narr = NArr_Init(narr_1->cap + narr_2->cap);

    while (i < 0) i += narr_1->len;
    while (k < 0) k += narr_2->len;

    int new_len_1 = i % narr_1->len;
    int new_len_2 = narr_2->len - k % narr_2->len;

    for (i = 0; i < new_len_1; i++)
        new_narr->elements[i] = narr_1->elements[i];

    for (i = 0; i < new_len_2; i++)
        new_narr->elements[new_len_1 + i] = narr_2->elements[k + i];
    
    return new_narr;
}

void NArr_Reverse(NodeArray* narr)
{
    Node* tmp;
    
    for (int i = 0; i < narr->len >> 1; i++)
    {
        tmp = narr->elements[i];
        narr->elements[i] = narr->elements[narr->len - i - 1];
        narr->elements[narr->len - i - 1] = tmp;
    }
}

void NArr_Print(NodeArray* narr)
{
    printf("[ ");

    for (int i = 0; i < narr->len; i++)
        printf("(%d, %d) ", narr->elements[i]->x, narr->elements[i]->y);

    printf("]");

    for (int i = narr->len; i < narr->cap; i++)
        printf(" +");

    printf("\n");
}

/////////////////////////////
// -- MinHeap functions -- //
/////////////////////////////

void Heap_SortUp(NodeArray* heap, int i)
{
    if (i <= 0) return;

    // parent index
    int ip = (i - 1) / 2;

    if (!Heap_Compare(heap, i, ip)) return;

    Node_Swap(&heap->elements[i], &heap->elements[ip]);
    Heap_SortUp(heap, ip);
}

void Heap_SortDown(NodeArray* heap)
{
    int height = Heap_Height(heap);
    int n = heap->len - powi(2, height - 1) + 1;

    for (int e = height - 1; e > 0; e--)
    {
        int start = powi(2, e) - 1;
        
        for (int k = start; k < start + n; k++)
        {
            if (k % 2 == 0) continue;

            int ip = (k - 1) / 2;
            int ir = 2 * ip + 2;

            if (Heap_Compare(heap, k, ir))
            {
                if (Heap_Compare(heap, k, ip))
                    Node_Swap(&heap->elements[k], &heap->elements[ip]);
                
                continue;
            }

            if (Heap_Compare(heap, ir, ip))
                Node_Swap(&heap->elements[ir], &heap->elements[ip]);
        }

        n = powi(2, e - 1);
    }
}

void Heap_Add(NodeArray* heap, Node* node)
{
    if (heap->len == heap->cap)
        NArr_Balance(heap);

    heap->elements[heap->len] = node;
    heap->len += 1;

    Heap_SortUp(heap, heap->len - 1);
}

Node* Heap_Take(NodeArray* heap)
{
    if (heap->len == 0) return NULL;

    Node* first = heap->elements[0];

    for (int i = 0; i < heap->len - 1; i++)
        heap->elements[i] = heap->elements[i+1];
    
    heap->elements[heap->len - 1] = NULL;
    heap->len -= 1;

    NArr_Balance(heap);

    return first;
}

int Heap_Height(NodeArray* heap)
{
    if (heap->len == 0) return 0;

    // log2(x) = logy(x) / logy(2)
    return (int) (log((double) heap->len) / log(2.0)) + 1;
}

int Heap_Compare(NodeArray* heap, int i, int k)
{
    if (i >= heap->len) return 0;
    if (k >= heap->len) return 1;
    if (heap->elements[i]->F > heap->elements[k]->F) return 0;
    if (heap->elements[i]->F < heap->elements[k]->F) return 1;
    if (heap->elements[i]->H > heap->elements[k]->H) return 0;
    return 1;
}

void Heap_Show(NodeArray* heap)
{
    int height = Heap_Height(heap);

    for (int layer = 0; layer < height; layer++)
    {
        int max_n = powi(2, layer);
        int spacing = powi(2, height - layer);

        for (int i = 0; i < max_n; i++)
        {
            int arr_i = i + powi(2, layer) - 1;
            
            if (arr_i >= heap->len) continue;

            for (int k = 0; k < spacing; k++)
                printf(" ");
            
            printf("%d", heap->elements[arr_i]->F);
        }

        printf("\n");
    }
}