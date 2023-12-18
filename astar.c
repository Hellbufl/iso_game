#include "astar.h"

////////////////////////
// -- A* functions -- //
////////////////////////

// not for resetting
AStar* astar_init(Node*** init_field, int init_width, int init_height)
{
    AStar* astar = (AStar*) calloc(1, sizeof(AStar));

    astar->field = init_field;
    astar->f_width = init_width;
    astar->f_height = init_height;

    astar->open_heap = narr_init(10);
    astar->closed_narr = narr_init(10);

    return astar;
}

void astar_reset(AStar* astar)
{
    if (astar == NULL) return;

    astar->found = 0;
    astar->stepcount = 0;

    narr_clear(astar->open_heap);
    narr_clear(astar->closed_narr);

    heap_add(astar->open_heap, astar->start);

    for (int ix = 0; ix < astar->f_width; ix++)
        for (int iy = 0; iy < astar->f_height; iy++)

            node_reset(astar->field[ix][iy]);
}

void astar_destroy(AStar* astar)
{
    if (astar == NULL) return;
    narr_destroy(astar->open_heap);
    narr_destroy(astar->closed_narr);
    free(astar);
}

// void astar_search(AStar* astar, NodeArray* outPath, NodeArray* outDirectPath)
// {
    // TODO: get some fucking consistency in this project

    // if (!astar->found)
    // {
    //     astar_step(astar);
    //     return;
    // }

    // astar_backtrack(astar, outPath);
    // // gstate->mode = 1;

    // if (outPath == NULL)
    // {
    //     perror("astar_search(): path == NULL");
    //     exit(1);
    // }

    // outDirectPath = astar_find_direct_path(astar, outPath, 0, -1);
// }

// returns number of neighbours found 
NodeArray* get_neighbours(AStar* astar, Node* node, NodeArray* nbs_narr)
{
    if (astar == NULL)
    {
        perror("get_neighbours(): AStar* astar == NULL");
        exit(1);
    }
    if (node == NULL)
    {
        perror("get_neighbours(): Node* node == NULL");
        exit(1);
    }

    for (int i = 0; i < 9; i++)
    {
        int x = node->x + (i % 3) - 1;
        int y = node->y + i / 3 - 1;

        if (x == node->x && y == node->y) continue;

        if (x < 0) continue;
        if (y < 0) continue;

        if (x >= astar->f_width) continue;
        if (y >= astar->f_height) continue;

        Node* current_node = astar->field[x][y];

        if (!current_node->walkable) continue;

        if (narr_contains(astar->closed_narr, current_node)) continue;

        // check the two direct adjacent nodes so you dont cut corners
        if (x != node->x && y != node->y)
        {
            Node* adj_1 = astar->field[node->x][y];
            Node* adj_2 = astar->field[x][node->y];

            if (!(adj_1->walkable && adj_2->walkable)) continue;
        }

        narr_add(nbs_narr, current_node);
    }

    return nbs_narr;
}

void astar_step(AStar* astar)
{
    if (astar == NULL) return;

    astar->stepcount++;

    Node* node = heap_take(astar->open_heap);
    Node* current_nb;
    int old_F;

    narr_add(astar->closed_narr, node);

    if (node == astar->dest)
    {
        astar->found = 1;
        return;
    }

    NodeArray* nbs_narr = narr_init(9);
    get_neighbours(astar, node, nbs_narr);

    for (int i = 0; i < nbs_narr->len; i++)
    {
        current_nb = nbs_narr->elements[i];
        old_F = current_nb->F;

        node_cal_F(current_nb, node, astar->dest);

        if (current_nb->F != old_F)
            heap_sortdown(astar->open_heap);
        
        if (!narr_contains(astar->open_heap, current_nb))
            heap_add(astar->open_heap, current_nb);
    }
    narr_destroy(nbs_narr);
}

void astar_backtrack(AStar* astar, NodeArray* path)
{
    for (Node* np = astar->dest; np != astar->start; np = np->parent)
        narr_add(path, np);

    narr_add(path, astar->start);
    narr_reverse(path);
}

NodeArray* astar_find_direct_path(AStar* astar, NodeArray* path, int first, int last)
{
    if (astar == NULL)
    {
        perror("astar_find_direct_path(): AStar* astar == NULL");
        exit(1);
    }
    if (path == NULL)
    {
        perror("astar_find_direct_path(): NodeArray* path == NULL");
        exit(1);
    }

    NodeArray* direct_path = narr_init(path->len);

    if (last < 0) last += path->len;

    Node* local_A = path->elements[first];
    Node* local_B = path->elements[last];

    narr_add(direct_path, local_A);
    narr_add(direct_path, local_B);

    int mid = (first + last) >> 1;
    Node* local_M = path->elements[mid];

    if (first == mid) return direct_path;
    if (astar_raycast(astar, local_A, local_B)) return direct_path;

    int path1_free = astar_raycast(astar, local_A, local_M);
    int path2_free = astar_raycast(astar, local_M, local_B);

    if (path1_free && path2_free)
    {
        narr_clear(direct_path);
        narr_add(direct_path, local_A);
        narr_add(direct_path, local_M);
        narr_add(direct_path, local_B);

        return direct_path;
    }

    if (path1_free)
    {
        NodeArray* path_2 = astar_find_direct_path(astar, path, mid, last);

        if (astar_raycast(astar, local_A, path_2->elements[1]))
            narr_append(direct_path, 1, path_2, 1);
        else
            narr_append(direct_path, 1, path_2, 0);

        narr_destroy(path_2);

        return direct_path;
    }

    if (path2_free)
    {
        NodeArray* path_1 = astar_find_direct_path(astar, path, first, mid);

        if (astar_raycast(astar, path_1->elements[path_1->len - 2], local_B))
            narr_append(path_1, -1, direct_path, 1);
        else
            narr_append(path_1, path_1->len, direct_path, 1);
        
        narr_destroy(direct_path);

        return path_1;
    }
    NodeArray* path_1 = astar_find_direct_path(astar, path, first, mid);
    NodeArray* path_2 = astar_find_direct_path(astar, path, mid, last);

    if (astar_raycast(astar, path_1->elements[path_1->len - 2], path_2->elements[1]))
        narr_append(path_1, -1, path_2, 1);
    else
        narr_append(path_1, path_1->len, path_2, 1);

    narr_destroy(direct_path);
    narr_destroy(path_2);

    return path_1;
}

int astar_raycast(AStar* astar, Node* start, Node* end)
{
    if (astar == NULL)
    {
        perror("astar_raycast(): AStar* astar == NULL");
        exit(1);
    }
    if (start == NULL)
    {
        perror("astar_raycast(): Node* start == NULL");
        exit(1);
    }
    if (end == NULL)
    {
        perror("astar_raycast(): Node* end == NULL");
        exit(1);
    }

    float vec[] = { (float) (end->x - start->x), (float) (end->y - start->y) };
    int step[] = { float_sign(vec[0]), float_sign(vec[1]) };

    // straight in y axis
    if (start->x == end->x)
    {
        for (int i = 0; i < abs(start->y - end->y); i++)
            if (!astar->field[start->x][start->y + step[1] * i]->walkable) return 0;
        
        return 1;
    }

    // straight in x axis
    if (start->y == end->y)
    {
        for (int i = 0; i < abs(start->x - end->x); i++)
            if (!astar->field[start->x + step[0] * i][start->y]->walkable) return 0;
        
        return 1;
    }

    float tan = vec[1] / vec[0];
    float cot = vec[0] / vec[1];

    Node* current_node = start;

    // initial intersection points
    float v_isect[] = { 0.5f * (float) step[0], 0.5f * (float) step[1] * tan };
    float h_isect[] = { 0.5f * (float) step[0] * cot, 0.5f * (float) step[1] };

    while (current_node != end)
    {
        narr_add(astar->closed_narr, current_node);

        float v_dist = sqrtf(v_isect[0] * v_isect[0] + v_isect[1] * v_isect[1]);
        float h_dist = sqrtf(h_isect[0] * h_isect[0] + h_isect[1] * h_isect[1]);

        // Node* node1 = astar->field[current_node->x + step[0]][current_node->y];
        // Node* node2 = astar->field[current_node->x][current_node->y + step[1]];
        
        // if (!node1->walkable) return 0;
        // if (!node2->walkable) return 0;

        if (v_dist < h_dist)
        {
            current_node = astar->field[current_node->x + step[0]][current_node->y];
            v_isect[0] += (float) step[0];
            v_isect[1] += (float) step[1] * tan;
        }
        else if (v_dist > h_dist)
        {
            current_node = astar->field[current_node->x][current_node->y + step[1]];
            h_isect[0] += (float) step[0] * cot;
            h_isect[1] += (float) step[1];
        }
        else
        {
            Node* node1 = astar->field[current_node->x + step[0]][current_node->y];
            Node* node2 = astar->field[current_node->x][current_node->y + step[1]];

            if (!node1->walkable) return 0;
            if (!node2->walkable) return 0;

            current_node = astar->field[current_node->x + step[0]][current_node->y + step[1]];
            v_isect[0] += step[0];
            v_isect[1] += step[1] * tan;
            h_isect[0] += step[0] * cot;
            h_isect[1] += step[1];
        }

        if (!current_node->walkable) return 0;
    }

    return 1;
}

//////////////////////////
// -- Node functions -- //
//////////////////////////

Node* node_init()
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    new_node->walkable = 1;
    new_node->parent = new_node;
    return new_node;
}

void node_reset(Node* node)
{
    node->G = 0;
    node->H = 0;
    node->F = 0;
    node->parent = node;
}

// calculate G, H and F values

int node_cal_G(Node* node, Node* parent)
{
    if (node->x == parent->x || node->y == parent->y)
        return parent->G + 10;
    
    return parent->G + 14;
}

int node_cal_H(Node* node, Node* dest)
{
    int delta_x = abs(dest->x - node->x);
    int delta_y = abs(dest->y - node->y);

    if (delta_x == delta_y)
        return 14 * delta_x;
    
    return 14 * MINOF(delta_x, delta_y) + 10 * abs(delta_y - delta_x);
}

void node_cal_F(Node* node, Node* parent, Node* dest)
{
    int new_G = node_cal_G(node, parent);

    if (new_G < node->G || node->G == 0)
    {
        node->G = new_G;
        node->parent = parent;
    }

    node->H = node_cal_H(node, dest);
    node->F = node->G + node->H;
}

void node_swap(Node** e1, Node** e2)
{
    Node* tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
}

///////////////////////////////
// -- NodeArray functions -- //
///////////////////////////////

NodeArray* narr_init(int capacity)
{
    NodeArray* narr = (NodeArray*) calloc(1, sizeof(NodeArray));

    if (narr == NULL)
    {
        perror("Failed to allocate memory for NodeArray");
        exit(1);
    }

    narr->elements = (Node**) calloc(capacity, sizeof(Node*));
    narr->len = 0;
    narr->cap = capacity;

    return narr;
}

void narr_expand(NodeArray* narr, int add_cap)
{
    if (narr == NULL) return;
    // if (add_cap <= 0) return;

    if (add_cap <= 0)
        add_cap = narr->len;

    Node** new_elements = (Node**) calloc(narr->cap + add_cap, sizeof(Node*));
    narr->cap += add_cap;

    for (int i = 0; i < narr->len; i++)
        new_elements[i] = narr->elements[i];
    
    free(narr->elements);

    narr->elements = new_elements;
}

void narr_balance(NodeArray* narr)
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
void narr_destroy(NodeArray* narr)
{
    if (narr == NULL) return;

    free(narr->elements);
    free(narr);
}

// does free the nodes
void narr_obliterate(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        free(narr->elements[i]);

    narr_destroy(narr);
}

// does NOT free the nodes
void narr_clear(NodeArray* narr)
{
    if (narr == NULL) return;

    for (int i = 0; i < narr->len; i++)
        narr->elements[i] = NULL;
    
    narr->len = 0;
}

void narr_set(NodeArray* narr, int i, Node* node)
{
    if (i < 0) return;
    if (i >= narr->len) return;

    narr->elements[i] = node;
}

void narr_add(NodeArray* narr, Node* node)
{
    if (narr->len >= narr->cap)
        narr_balance(narr);

    narr->elements[narr->len] = node;
    narr->len += 1;
}

// does NOT free the node
void narr_remove(NodeArray* narr, Node* node)
{
    for (int i = 0; i < narr->len; i++)

        if (narr->elements[i] == node)
            narr->elements[i] = NULL;
    
    narr_balance(narr);
}

int narr_contains(NodeArray* narr, Node* node)
{
    if (narr == NULL) return 0;

    for (int i = 0; i < narr->len; i++)
    {
        if (narr->elements[i] == node) return 1;
    }
    return 0;
}

void narr_append(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    if (i > narr_1->len) printf("[error] narr_append(): index i out of range\n");
    if (k >= narr_2->len) printf("[error] narr_append(): index k out of range\n");

    while (i < 0) i += narr_1->len;
    while (k < 0) k += narr_2->len;

    narr_expand(narr_1, narr_2->len);
    narr_1->len = i + narr_2->len - k;

    while (i < narr_1->cap && k < narr_2->len)
    {
        narr_1->elements[i] = narr_2->elements[k];
        i++;
        k++;
    }
}

NodeArray* narr_join(NodeArray* narr_1, int i, NodeArray* narr_2, int k)
{
    NodeArray* new_narr = narr_init(narr_1->cap + narr_2->cap);

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

void narr_reverse(NodeArray* narr)
{
    Node* tmp;
    
    for (int i = 0; i < narr->len >> 1; i++)
    {
        tmp = narr->elements[i];
        narr->elements[i] = narr->elements[narr->len - i - 1];
        narr->elements[narr->len - i - 1] = tmp;
    }
}

void narr_print(NodeArray* narr)
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

void heap_sortup(NodeArray* heap, int i)
{
    if (i <= 0) return;

    // parent index
    int ip = (i - 1) / 2;

    if (!less_expensive(heap, i, ip)) return;

    node_swap(&heap->elements[i], &heap->elements[ip]);
    heap_sortup(heap, ip);
}

void heap_sortdown(NodeArray* heap)
{
    int height = heap_height(heap);
    int n = heap->len - int_pow(2, height - 1) + 1;

    for (int e = height - 1; e > 0; e--)
    {
        int start = int_pow(2, e) - 1;
        
        for (int k = start; k < start + n; k++)
        {
            if (k % 2 == 0) continue;

            int ip = (k - 1) / 2;
            int ir = 2 * ip + 2;

            if (less_expensive(heap, k, ir))
            {
                if (less_expensive(heap, k, ip))
                    node_swap(&heap->elements[k], &heap->elements[ip]);
                
                continue;
            }

            if (less_expensive(heap, ir, ip))
                node_swap(&heap->elements[ir], &heap->elements[ip]);
        }

        n = int_pow(2, e - 1);
    }
}

void heap_add(NodeArray* heap, Node* node)
{
    if (heap->len == heap->cap)
        narr_balance(heap);

    heap->elements[heap->len] = node;
    heap->len += 1;

    heap_sortup(heap, heap->len - 1);
}

Node* heap_take(NodeArray* heap)
{
    if (heap->len == 0) return NULL;

    Node* first = heap->elements[0];

    for (int i = 0; i < heap->len - 1; i++)
        heap->elements[i] = heap->elements[i+1];
    
    heap->elements[heap->len - 1] = NULL;
    heap->len -= 1;

    narr_balance(heap);

    return first;
}

int heap_height(NodeArray* heap)
{
    if (heap->len == 0) return 0;

    // log2(x) = logy(x) / logy(2)
    return (int) (log((double) heap->len) / log(2.0)) + 1;
}

int less_expensive(NodeArray* heap, int i, int k)
{
    if (i >= heap->len) return 0;
    if (k >= heap->len) return 1;
    if (heap->elements[i]->F > heap->elements[k]->F) return 0;
    if (heap->elements[i]->F < heap->elements[k]->F) return 1;
    if (heap->elements[i]->H > heap->elements[k]->H) return 0;
    return 1;
}

void heap_show(NodeArray* heap)
{
    int height = heap_height(heap);

    for (int layer = 0; layer < height; layer++)
    {
        int max_n = int_pow(2, layer);
        int spacing = int_pow(2, height - layer);

        for (int i = 0; i < max_n; i++)
        {
            int arr_i = i + int_pow(2, layer) - 1;
            
            if (arr_i >= heap->len) continue;

            for (int k = 0; k < spacing; k++)
                printf(" ");
            
            printf("%d", heap->elements[arr_i]->F);
        }

        printf("\n");
    }
}