#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __node {                   
    int value;
    struct __node *next;
} node_t;

/* 優雅的code
void remove_list_node(List *list, Node *target)
{
    // A pointer pointing to a Node pointer.
    // We first point it to the head of the list.
    // The "indirect" pointer points to the *address*
    // of the thing we'll update.

    Node **indirect = &list->head;
    // Walk the list, looking for the thing that 
    // points to the node we want to remove.
    while (*indirect != target)
        indirect = &(*indirect)->next;
    *indirect = target->next;
}
*/
static inline node_t* list_make_node_t(node_t *list, int value) {
    node_t *node = malloc(sizeof(node_t));
    node->value = value;
    node->next = 0;
    if (list)
    {
        node->next = list->next;
        list->next = node;
    } else
	list = node;
    return list;
}

static inline void list_add_node_t(node_t **list, node_t *node_t) {
    node_t->next = *list;   // Add to head of the list(by dereferencing the list pointer we get the address of the list head).
    *list = node_t;         // Change list head to node_t.
}

static inline void list_concat(node_t **left, node_t *right) {
    // Using left as a pointer to a pointer to current node_t.
    // So left stores the address of the pointer to current node_t.
    // Dereferencing it we get access to the current node_t.
    while (*left)
        left = &(*left)->next;
    *left = right;
}

void quicksort(node_t **list)
{
    if (!*list)
        return;

    node_t *pivot = *list;
    int value = pivot->value;
    node_t *p = pivot->next;
    pivot->next = NULL;

    node_t *left = NULL, *right = NULL;
    while (p) {
        node_t *n = p;
        p = p->next;
        list_add_node_t(n->value > value ? &right : &left, n);
    }

    quicksort(&left);
    quicksort(&right);

    node_t *result = NULL;
    list_concat(&result, left);
    list_concat(&result, pivot);
    list_concat(&result, right);
    *list = result;
}

static bool list_is_ordered(node_t *list) {
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

static void list_display(node_t *list) {
    printf("%s IN ORDER : ", list_is_ordered(list) ? "   " : "NOT");
    while (list) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

static void list_free(node_t **list) {
    while (*list) {
        node_t *curr = *list; //Make a pointer point to head item.
	//free current item
	*list = curr->next;   //Make head pointer of list point to the next one of current.
        free(curr);
    }
}

int main(int argc, char **argv) {
    int count = 20;

    node_t *list = NULL;
    while (count--)
        list = list_make_node_t(list, random() % 1024);

    list_display(list);
    quicksort(&list);
    list_display(list);

    if (!list_is_ordered(list))
        return EXIT_FAILURE;

    list_free(&list);
    return EXIT_SUCCESS;
}        
