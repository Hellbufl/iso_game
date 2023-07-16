#include "astar.h"

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

        Node* node1 = astar->field[current_node->x + step[0]][current_node->y];
        Node* node2 = astar->field[current_node->x][current_node->y + step[1]];
        
        // if (!node1->walkable) return 0;
        // if (!node2->walkable) return 0;

        if (v_dist < h_dist)
        {
            current_node = node1;
            v_isect[0] += (float) step[0];
            v_isect[1] += (float) step[1] * tan;
        }
        else if (v_dist > h_dist)
        {
            current_node = node2;
            h_isect[0] += (float) step[0] * cot;
            h_isect[1] += (float) step[1];
        }
        else
        {
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