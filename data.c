#include <stdlib.h>
#include <string.h>
#include "data.h"


/* Add a directed edge to the graph. */
void graph_add_edge(Graph* g, char from, char to) {
    /* Find the vertices in the graph's vertex list. */
    Vertex* from_vertex = NULL;
    Vertex* to_vertex = NULL;
    for (size_t i = 0; i < g->n; i++) {
        if (g->vertices[i].val == from) {
            from_vertex = &g->vertices[i];
        }
        if (g->vertices[i].val == to) {
            to_vertex = &g->vertices[i];
        }
    }
    if (from_vertex == NULL || to_vertex == NULL) return;
    /* Make sure the edge doesn't actually exist. */
    VertexList* ptr = from_vertex->neighbors;
    while (ptr != NULL) {
        if (ptr->v == to_vertex) {
            return;
        }
        ptr = ptr->next;
    }
    /* Construct the new entry in the vertex's edge list. */
    VertexList* new_ptr = malloc(sizeof *new_ptr);
    if (new_ptr) {
        new_ptr->v = to_vertex;
        new_ptr->next = from_vertex->neighbors;
        from_vertex->neighbors = new_ptr;
    }
}


Graph* graph_from_string(enum GraphType typ, const char* vertices, const char* edges) {
    Graph* ret = malloc(sizeof *ret);
    if (ret) {
        ret->n = strlen(vertices);
        ret->vertices = malloc(ret->n * sizeof *ret->vertices);
        if (ret->vertices) {
            /* Add the vertices. */
            for (size_t i = 0; i < ret->n; i++) {
                ret->vertices[i].val = vertices[i];
                ret->vertices[i].neighbors = NULL;
            }
            /* Add the edges. */
            size_t i = 0;
            size_t n = strlen(edges);
            while (i < n) {
                graph_add_edge(ret, edges[i], edges[i+1]);
                if (typ == UNDIRECTED) {
                    graph_add_edge(ret, edges[i+1], edges[i]);
                }
                i += 3;
            }
        }
    }
    return ret;
}


void vertex_list_free(VertexList* p) {
    if (p == NULL) return;
    vertex_list_free(p->next);
    free(p);
}


void graph_free(Graph* g) {
    if (g == NULL) return;
    for (size_t i = 0; i < g->n; i++) {
        vertex_list_free(g->vertices[i].neighbors);
    }
    free(g->vertices);
    free(g);
}
