#include "2-3_tree.h"
#include <stdlib.h>
#include <stdio.h>

void del_tree(node** tree) {
    if (!(*tree)) {
        return;
    }
    int i;
    for (i = 0; i < (*tree)->size; i++) {
        del_tree(&(*tree)->sons[i]);
    }
    free(*tree);
    *tree = NULL;
}

node* search(node* tree, type k) {
    if (!tree) return NULL;
    int found = 0;
    for (int i = 0; i < tree->size; i++) {
        if (tree->keys[i] == k) {
            found = 1;
            break;
        }
    }
    if (found) return tree;
    else if (k < tree->keys[0]) return search(tree->sons[0], k);
    else if ((tree->size == 1) || (tree->size == 2) && (k < tree->keys[1])) return search(tree->sons[1], k);
    else if (tree->size == 2) return search(tree->sons[2], k);
    return NULL;
}

int is_leaf(node* tree) {
    return (tree->sons[0] == NULL) && (tree->sons[1] == NULL) && (tree->sons[2] == NULL);
}

void swap(type* a, type* b) {
    type tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort(node* tree) {
    if (tree->size == 1) {
        return;
    }
    if (tree->size == 2) {
        if (tree->keys[0] > tree->keys[1]) {
            swap(&tree->keys[0], &tree->keys[1]);
        }
    }
    else {
        if (tree->keys[0] > tree->keys[1]) {
            swap(&tree->keys[0], &tree->keys[1]);
        }
        if (tree->keys[0] > tree->keys[2]) {
            swap(&tree->keys[0], &tree->keys[2]);
        }
        if (tree->keys[1] > tree->keys[2]) {
            swap(&tree->keys[1], &tree->keys[2]);
        }
    }
}

void insert_to_node(node* tree, type k) {
    tree->keys[tree->size++] = k;
    sort(tree);
}

node* split(node* tree) {
    node* x = (node*)malloc(sizeof(node));
    node* y = (node*)malloc(sizeof(node));
    if (!x || !y) {
        fprintf(stderr, "Can't allocate memory!");
        exit(1);
    }
    x->size = 1;
    x->keys[0] = tree->keys[0];
    x->sons[0] = tree->sons[0];
    x->sons[1] = tree->sons[1];
    x->sons[2] = NULL;
    x->sons[3] = NULL;
    x->parent = tree->parent;
    y->size = 1;
    y->keys[0] = tree->keys[2];
    y->sons[0] = tree->sons[2];
    y->sons[1] = tree->sons[3];
    y->sons[2] = NULL;
    y->sons[3] = NULL;
    y->parent = tree->parent;
    if (x->sons[0]) {
        x->sons[0]->parent = x;
    }
    if (x->sons[1]) {
        x->sons[1]->parent = x;
    }
    if (y->sons[0]) {
        y->sons[0]->parent = y;
    }
    if (y->sons[1]) {
        y->sons[1]->parent = y;
    }
    if (tree->parent) {
        insert_to_node(tree->parent, tree->keys[1]);
        if (tree->parent->sons[0] == tree) {
            tree->parent->sons[0] = NULL;
        }
        else if (tree->parent->sons[1] == tree) {
            tree->parent->sons[1] = NULL;
        }
        else if (tree->parent->sons[2] == tree) {
            tree->parent->sons[2] = NULL;
        }
        if (tree->parent->sons[0] == NULL) {
            tree->parent->sons[3] = tree->parent->sons[2];
            tree->parent->sons[2] = tree->parent->sons[1];
            tree->parent->sons[1] = y;
            tree->parent->sons[0] = x;
        }
        else if (tree->parent->sons[1] == NULL) {
            tree->parent->sons[3] = tree->parent->sons[2];
            tree->parent->sons[2] = y;
            tree->parent->sons[1] = x;
        }
        else {
            tree->parent->sons[3] = y;
            tree->parent->sons[2] = x;
        }
        node* tmp = tree->parent;
        free(tree);
        return tmp;
    }
    else {
        x->parent = tree;
        y->parent = tree;
        tree->keys[0] = tree->keys[1];
        tree->size = 1;
        tree->sons[0] = x;
        tree->sons[1] = y;
        tree->sons[2] = NULL;
        tree->sons[3] = NULL;
        return tree;
    }

}

node* insert(node* tree, type k) {
    if (!tree) {
        tree = (node*)malloc(sizeof(node));
        if (!tree) {
            fprintf(stderr, "Can't allocate memory!");
            exit(1);
        }
        tree->size = 1;
        tree->keys[0] = k;
        tree->parent = NULL;
        int i;
        for (i = 0; i < 4; i++) {
            tree->sons[i] = NULL;
        }
        return tree;
    }
    if (is_leaf(tree)) {
        insert_to_node(tree, k);
    }
    else if (k < tree->keys[0]) {
        insert(tree->sons[0], k);
    }
    else if ((tree->size == 1) || ((tree->size == 2) && (k < tree->keys[1]))) {
        insert(tree->sons[1], k);
    }
    else {
        insert(tree->sons[2], k);
    }
    if (tree->size <= 2) {
        return tree;
    }
    return split(tree);
}

node* search_min(node* p) {
    if (!p) return p;
    if (!(p->sons[0])) return p;
    else return search_min(p->sons[0]);
}

void remove_from_node(node* p, type k) {
    if (p->size >= 1 && p->keys[0] == k) {
        p->keys[0] = p->keys[1];
        p->keys[1] = p->keys[2];
        p->size--;
    }
    else if (p->size == 2 && p->keys[1] == k) {
        p->keys[1] = p->keys[2];
        p->size--;
    }
}

node* redistribute(node* leaf) {
    node* parent = leaf->parent;
    node* sons[3] = { parent->sons[0],parent->sons[1], parent->sons[2] };
    if ((parent->size == 2) && (sons[0]->size < 2) && (sons[1]->size < 2) && (sons[2]->size < 2)) {
        if (sons[0] == leaf) {
            parent->sons[0] = parent->sons[1];
            parent->sons[1] = parent->sons[2];
            parent->sons[2] = NULL;
            insert_to_node(parent->sons[0], parent->keys[0]);
            parent->sons[0]->sons[2] = parent->sons[0]->sons[1];
            parent->sons[0]->sons[1] = parent->sons[0]->sons[0];
            if (leaf->sons[0] != NULL) parent->sons[0]->sons[0] = leaf->sons[0];
            else if (leaf->sons[1] != NULL) parent->sons[0]->sons[0] = leaf->sons[1];
            if (parent->sons[0]->sons[0] != NULL) parent->sons[0]->sons[0]->parent = parent->sons[0];
            remove_from_node(parent, parent->keys[0]);
            free(sons[0]);
        }
        else if (sons[1] == leaf) {
            insert_to_node(sons[0], parent->keys[0]);
            remove_from_node(parent, parent->keys[0]);
            if (leaf->sons[0] != NULL) sons[0]->sons[2] = leaf->sons[0];
            else if (leaf->sons[1] != NULL) sons[0]->sons[2] = leaf->sons[1];

            if (sons[0]->sons[2] != NULL) sons[0]->sons[2]->parent = sons[0];

            parent->sons[1] = parent->sons[2];
            parent->sons[2] = NULL;

            free(sons[1]);
        }
        else if (sons[2] == leaf) {
            insert_to_node(sons[1], parent->keys[1]);
            parent->sons[2] = NULL;
            remove_from_node(parent, parent->keys[1]);
            if (leaf->sons[0] != NULL) sons[1]->sons[2] = leaf->sons[0];
            else if (leaf->sons[1] != NULL) sons[1]->sons[2] = leaf->sons[1];

            if (sons[1]->sons[2] != NULL)  sons[1]->sons[2]->parent = sons[1];

            free(sons[2]);
        }
    }
    else if ((parent->size == 2) && ((sons[0]->size == 2) || (sons[1]->size == 2) || (sons[2]->size == 2))) {
        if (sons[2] == leaf) {
            if (leaf->sons[0] != NULL) {
                leaf->sons[1] = leaf->sons[0];
                leaf->sons[0] = NULL;
            }

            insert_to_node(leaf, parent->keys[1]);
            if (sons[1]->size == 2) {
                parent->keys[1] = sons[1]->keys[1];
                remove_from_node(sons[1], sons[1]->keys[1]);
                leaf->sons[0] = sons[1]->sons[2];
                sons[1]->sons[2] = NULL;
                if (leaf->sons[0] != NULL) leaf->sons[0]->parent = leaf;
            }
            else if (sons[0]->size == 2) {
                parent->keys[1] = sons[1]->keys[0];
                leaf->sons[0] = sons[1]->sons[1];
                sons[1]->sons[1] = sons[1]->sons[0];
                if (leaf->sons[0] != NULL) leaf->sons[0]->parent = leaf;

                sons[1]->keys[0] = parent->keys[0];
                parent->keys[0] = sons[0]->keys[1];
                remove_from_node(sons[0], sons[0]->keys[1]);
                sons[1]->sons[0] = sons[0]->sons[2];
                if (sons[1]->sons[0] != NULL) sons[1]->sons[0]->parent = sons[1];
                sons[0]->sons[2] = NULL;
            }
        }
        else if (sons[1] == leaf) {
            if (sons[2]->size == 2) {
                if (leaf->sons[0] == NULL) {
                    leaf->sons[0] = leaf->sons[1];
                    leaf->sons[1] = NULL;
                }
                insert_to_node(sons[1], parent->keys[1]);
                parent->keys[1] = sons[2]->keys[0];
                remove_from_node(sons[2], sons[2]->keys[0]);
                sons[1]->sons[1] = sons[2]->sons[0];
                if (sons[1]->sons[1] != NULL) sons[1]->sons[1]->parent = sons[1];
                sons[2]->sons[0] = sons[2]->sons[1];
                sons[2]->sons[1] = sons[2]->sons[2];
                sons[2]->sons[2] = NULL;
            }
            else if (sons[0]->size == 2) {
                if (leaf->sons[1] == NULL) {
                    leaf->sons[1] = leaf->sons[0];
                    leaf->sons[0] = NULL;
                }
                insert_to_node(sons[1], parent->keys[0]);
                parent->keys[0] = sons[0]->keys[1];
                remove_from_node(sons[0], sons[0]->keys[1]);
                sons[1]->sons[0] = sons[0]->sons[2];
                if (sons[1]->sons[0] != NULL) sons[1]->sons[0]->parent = sons[1];
                sons[0]->sons[2] = NULL;
            }
        }
        else if (sons[0] == leaf) {
            if (leaf->sons[0] == NULL) {
                leaf->sons[0] = leaf->sons[1];
                leaf->sons[1] = NULL;
            }
            insert_to_node(sons[0], parent->keys[0]);
            if (sons[1]->size == 2) {
                parent->keys[0] = sons[1]->keys[0];
                remove_from_node(sons[1], sons[1]->keys[0]);
                sons[0]->sons[1] = sons[1]->sons[0];
                if (sons[0]->sons[1] != NULL) sons[0]->sons[1]->parent = sons[0];
                sons[1]->sons[0] = sons[1]->sons[1];
                sons[1]->sons[1] = sons[1]->sons[2];
                sons[1]->sons[2] = NULL;
            }
            else if (sons[2]->size == 2) {
                parent->keys[0] = sons[1]->keys[0];
                sons[1]->keys[0] = parent->keys[1];
                parent->keys[1] = sons[2]->keys[0];
                remove_from_node(sons[2], sons[2]->keys[0]);
                sons[0]->sons[1] = sons[1]->sons[0];
                if (sons[0]->sons[1] != NULL) sons[0]->sons[1]->parent = sons[0];
                sons[1]->sons[0] = sons[1]->sons[1];
                sons[1]->sons[1] = sons[2]->sons[0];
                if (sons[1]->sons[1] != NULL) sons[1]->sons[1]->parent = sons[1];
                sons[2]->sons[0] = sons[2]->sons[1];
                sons[2]->sons[1] = sons[2]->sons[2];
                sons[2]->sons[2] = NULL;
            }
        }
    }
    else if (parent->size == 1) {

        if (sons[0] == leaf && sons[1]->size == 2) {
            parent->keys[0] = sons[1]->keys[0];
            remove_from_node(sons[1], sons[1]->keys[0]);

            if (leaf->sons[0] == NULL) leaf->sons[0] = leaf->sons[1];

            leaf->sons[1] = sons[1]->sons[0];
            sons[1]->sons[0] = sons[1]->sons[1];
            sons[1]->sons[1] = sons[1]->sons[2];
            sons[1]->sons[2] = NULL;
            if (leaf->sons[1] != NULL) leaf->sons[1]->parent = leaf;
        }
        else if (sons[1] == leaf && sons[0]->size == 2) {
            parent->keys[0] = sons[0]->keys[1];
            remove_from_node(sons[0], sons[0]->keys[1]);

            if (leaf->sons[1] == NULL) leaf->sons[1] = leaf->sons[0];

            leaf->sons[0] = sons[0]->sons[2];
            sons[0]->sons[2] = NULL;
            if (leaf->sons[0] != NULL) leaf->sons[0]->parent = leaf;
        }
    }
    return parent;
}

node* merge(node* leaf) {
    node* parent = leaf->parent;
    if (parent->sons[0] == leaf) {
        insert_to_node(parent->sons[1], parent->keys[0]);
        parent->sons[1]->sons[2] = parent->sons[1]->sons[1];
        parent->sons[1]->sons[1] = parent->sons[1]->sons[0];
        if (leaf->sons[0] != NULL) parent->sons[1]->sons[0] = leaf->sons[0];
        else if (leaf->sons[1] != NULL) parent->sons[1]->sons[0] = leaf->sons[1];
        if (parent->sons[1]->sons[0] != NULL) parent->sons[1]->sons[0]->parent = parent->sons[1];
        remove_from_node(parent, parent->keys[0]);
        free(parent->sons[0]);
        parent->sons[0] = NULL;
    }
    else if (parent->sons[1] == leaf) {
        insert_to_node(parent->sons[0], parent->keys[0]);
        if (leaf->sons[0] != NULL) parent->sons[0]->sons[2] = leaf->sons[0];
        else if (leaf->sons[1] != NULL) parent->sons[0]->sons[2] = leaf->sons[1];
        if (parent->sons[0]->sons[2] != NULL) parent->sons[0]->sons[2]->parent = parent->sons[0];
        remove_from_node(parent, parent->keys[0]);
        free(parent->sons[1]);
        parent->sons[1] = NULL;
    }
    if (parent->parent == NULL) {
        node* tmp = NULL;
        if (parent->sons[0] != NULL) tmp = parent->sons[0];
        else tmp = parent->sons[1];
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

node* fix(node* leaf) {
    if (leaf->size == 0 && leaf->parent == NULL) {
        free(leaf);
        return NULL;
    }
    if (leaf->size != 0) {
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }
    node* parent = leaf->parent;
    if (parent->sons[0]->size == 2 || parent->sons[1]->size == 2 || parent->size == 2) leaf = redistribute(leaf);
    else if (parent->size == 2 && parent->sons[2]->size == 2) leaf = redistribute(leaf);
    else leaf = merge(leaf);
    return fix(leaf);
}

node* del_key(node* p, type k) {
    node* item = search(p, k);
    if (!item) return p;
    node* min = NULL;
    if (item->keys[0] == k) min = search_min(item->sons[1]);
    else min = search_min(item->sons[2]);
    if (min) {
        type* z = (k == item->keys[0] ? &(item->keys[0]) : &(item->keys[1]));
        swap(z, &min->keys[0]);
        item = min;
    }
    remove_from_node(item, k);
    return fix(item);
}