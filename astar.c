#include "astar.h"
// #include "node.h"
// #include "minheap.h"
// #include "util.h"

// not for resetting
AStar* ass_init(Node*** init_field, int init_width, int init_height, Node* init_start, Node* init_dest)
{
    AStar* ass = (AStar*) calloc(1, sizeof(AStar));

    ass->field = init_field;
    ass->f_width = init_width;
    ass->f_height = init_height;
    ass->start = init_start;
    ass->dest = init_dest;

    ass->open_heap = narr_init(NULL, 10);
    heap_add(ass->open_heap, ass->start);

    ass->closed_narr = narr_init(NULL, 10);

    return ass;
}

void ass_reset(AStar* ass)
{
    AStar* new_ass = ass_init(ass->field, ass->f_width, ass->f_height, ass->start, ass->dest);
    *ass = *new_ass;
    free(new_ass);
}

// returns number of neighbours found 
NodeArray* get_neighbours(AStar* ass, Node* node)
{
    NodeArray* nbs_narr = narr_init(NULL, 9);

    for (int i = 0; i < 9; i++)
    {
        int x = node->x + (i % 3) - 1;
        int y = node->y + i / 3 - 1;

        if (x == y == 0) continue;

        if (x < 0) continue;
        if (y < 0) continue;

        if (x >= ass->f_width) continue;
        if (y >= ass->f_height) continue;

        Node* current_node = ass->field[x][y];

        if (!current_node->walkable) continue;

        if (narr_contains(ass->closed_narr, current_node)) continue;

        // check the two direct adjacent nodes so you dont cut corners
        if (x != node->x && y != node->y)
        {
            Node* adj_1 = ass->field[node->x][y];
            Node* adj_2 = ass->field[x][node->y];

            if (!(adj_1->walkable && adj_2->walkable)) continue;
        }

        narr_add(nbs_narr, current_node);
    }

    return nbs_narr;
}

void ass_step(AStar* ass)
{
    ass->stepcount++;

    Node* node = heap_take(ass->open_heap);
    Node* current_nb;
    int old_F;

    narr_add(ass->closed_narr, node);

    if (node == ass->dest)
    {
        ass->found = 1;
        return;
    }

    NodeArray* nbs_narr = get_neighbours(ass, node);

    for (int i = 0; i < nbs_narr->len; i++)
    {
        current_nb = nbs_narr->elements[i];
        old_F = current_nb->F;

        node_cal_F(current_nb, node, ass->dest);

        if (current_nb->F != old_F)
            heap_sortdown(ass->open_heap);
        
        if (!narr_contains(ass->open_heap, current_nb))
            heap_add(ass->open_heap, current_nb);
    }
}

NodeArray* ass_find_direct_path(AStar* ass, NodeArray* path, int first, int last)
{
    NodeArray* direct_path = narr_init(NULL, path->len);

    if (last < 0) last = path->len - 1;

    Node* local_A = path->elements[first];
    Node* local_B = path->elements[last];

    narr_add(direct_path, local_A);
    narr_add(direct_path, local_B);

    int mid = (first + last) / 2;
    Node* local_M = path->elements[mid];

    if (first == mid)
    {
        narr_add(direct_path, local_A);
        narr_add(direct_path, local_B);
        return direct_path;
    }

    int path1_free = ass_raycast(ass, local_M, local_A);
    int path2_free = ass_raycast(ass, local_M, local_B);

    if (path1_free && path2_free)
    {
        narr_add(direct_path, local_A);
        narr_add(direct_path, local_M);
        narr_add(direct_path, local_B);
        return direct_path;
    }

    if (path1_free)
    {
        path = ass_find_direct_path(ass, path, mid, last);

        if (ass_raycast(ass, local_A, path->elements[1]))
            narr_append(direct_path, -1, path, 1);
        else
            narr_append(direct_path, 2, path, 1);

        narr_destroy(path);
        return direct_path;
    }

    if (path2_free)
    {
        path = ass_find_direct_path(ass, path, first, mid);

        if (ass_raycast(ass, local_B, path->elements[path->len - 2]))
            narr_append(path, -1, direct_path, 1);
        else
            narr_append(path, path->len, direct_path, 1);

        narr_destroy(direct_path);
        return direct_path;
    }

    NodeArray* path_1 = ass_find_direct_path(ass, path, first, mid);
    NodeArray* path_2 = ass_find_direct_path(ass, path, mid, last);

    if (ass_raycast(ass, path_1->elements[path_1->len - 2], path_2->elements[1]))
        path = narr_append(path_1, -1, path_2, 1);
    else
        path = narr_append(path_1, path_1->len, path_2, 1);

    narr_destroy(path_1);
    narr_destroy(path_2);
    return path;
}

int ass_raycast(AStar* ass, Node* start, Node* end)
{
    float vec[] = { (float) (end->x - start->x), (float) (end->y - start->y) };
    int step = { dsign(vec[0]), dsign(vec[1]) };

    // straight in y axis
    if (start->x == end->x)
    {
        for (int i = 0; i < iabs(start->y - end->y); i++)
            if (!ass->field[start->x][start->y + step[1] * i]->walkable) return 0;
        
        return 1;
    }

    // straight in x axis
    if (start->y == end->y)
    {
        for (int i = 0; i < iabs(start->x - end->x); i++)
            if (!ass->field[start->x + step[0] * i][start->y]->walkable) return 0;
        
        return 1;
    }

    float tan = vec[1] / vec[0];
    float cot = vec[0] / vec[1];

    current_node = start;

    // initial intersection points
    float v_isect[] = { 0.5f * (float) step[0], 0.5f * (float) step[1] * tan };
    float h_isect[] = { 0.5f * (float) step[0], 0.5f * (float) step[1] * tan };

    while (current_node != end)
    {
        // inverse distance cause fast inverse sqare root is fast
        float v_iid = fast_isqrt(v_isect[0] * v_isect[0] + v_isect[1] * v_isect[1]);
        float h_iid = fast_isqrt(h_isect[0] * h_isect[0] + h_isect[1] * h_isect[1]);

        Node* node1 = ass->field[current_node->x + step[0]][current_node->y];
        Node* node2 = ass->field[current_node->x][current_node->y + step[1]];

        if (v_iid > h_iid)
        {
            current_node = node1;
            v_isect[0] += (float) step[0];
            v_isect[1] += (float) step[1] * tan;
        }
        else if (v_iid < h_iid)
        {
            current_node = node2;
            h_isect[0] += (float) step[0] * cot;
            h_isect[1] += (float) step[1];
        }
        else
        {
            if (!node1->walkable) return 0;
            if (!node2->walkable) return 0;

            current_node = ass->field[current_node->x + step[0]][current_node->y + step[1]];
            v_isect[0] += step[0];
            v_isect[1] += step[1] * tan;
            h_isect[0] += step[0] * cot;
            h_isect[1] += step[1];
        }

        if (!current_node->walkable) return 0;
    }

    return 1;
}