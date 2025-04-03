#include "mmr_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* BST node for ID-indexed tree */
typedef struct IDNode {
    char *id;
    int mmr;
    struct IDNode *left, *right;
} IDNode;

/* The BST-based database holds only the ID-indexed tree. */
typedef struct MMR_DB {
    IDNode *id_root;
} MMR_DB;

/* --- BST functions keyed by id --- */

/* Insert a node keyed by id. */
static IDNode *id_insert(IDNode *root, const char *id, int mmr, int *status) {
    if (!root) {
        IDNode *node = malloc(sizeof(IDNode));
        if (!node) { *status = -1; return NULL; }
        node->id = strdup(id);
        if (!node->id) { free(node); *status = -1; return NULL; }
        node->mmr = mmr;
        node->left = node->right = NULL;
        return node;
    }
    int cmp = strcmp(id, root->id);
    if (cmp == 0) {
        *status = 1; /* duplicate */
        return root;
    } else if (cmp < 0)
        root->left = id_insert(root->left, id, mmr, status);
    else
        root->right = id_insert(root->right, id, mmr, status);
    return root;
}

/* Find a node in the ID BST. */
static IDNode *id_find(IDNode *root, const char *id) {
    if (!root) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp == 0) return root;
    return cmp < 0 ? id_find(root->left, id) : id_find(root->right, id);
}

/* Delete a node from the ID BST. */
static IDNode *id_delete(IDNode *root, const char *id, int *status) {
    if (!root) { *status = 1; return NULL; }
    int cmp = strcmp(id, root->id);
    if (cmp < 0)
        root->left = id_delete(root->left, id, status);
    else if (cmp > 0)
        root->right = id_delete(root->right, id, status);
    else {
        if (!root->left) {
            IDNode *temp = root->right;
            free(root->id);
            free(root);
            *status = 0;
            return temp;
        } else if (!root->right) {
            IDNode *temp = root->left;
            free(root->id);
            free(root);
            *status = 0;
            return temp;
        } else {
            /* Find inorder successor */
            IDNode *succParent = root;
            IDNode *succ = root->right;
            while (succ->left) {
                succParent = succ;
                succ = succ->left;
            }
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            if (succParent != root)
                succParent->left = id_delete(succParent->left, succ->id, status);
            else
                succParent->right = id_delete(succParent->right, succ->id, status);
        }
    }
    return root;
}

/* Destroy the ID BST. */
static void id_destroy(IDNode *root) {
    if (!root) return;
    id_destroy(root->left);
    id_destroy(root->right);
    free(root->id);
    free(root);
}

/* Traverse the ID BST and apply function fp to each node.
   The function fp should return nonzero for nodes that count toward a total.
   Returns the total count. */
int traverse(IDNode *node, MMR_FUNC_T fp) {
    if (!node) return 0;
    int count = fp(node->id, node->mmr) ? 1 : 0;
    count += traverse(node->left, fp);
    count += traverse(node->right, fp);
    return count;
}

/* Count players using an in-order traversal of the ID BST. */
int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp) {
    if (!db || !fp) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    int count = traverse(pdb->id_root, fp);
    return count;
}

/* --- API functions --- */

MMR_DB_T CreateMMRDB(void) {
    MMR_DB *db = malloc(sizeof(MMR_DB));
    if (!db) return NULL;
    db->id_root = NULL;
    return (MMR_DB_T)db;
}

void DestroyMMRDB(MMR_DB_T *db) {
    if (!db || !*db) return;
    MMR_DB *pdb = (MMR_DB *)(*db);
    id_destroy(pdb->id_root);
    free(pdb);
    *db = NULL;
}

int RegisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    if (id_find(pdb->id_root, id))
        return 1;  /* duplicate */
    int status = 0;
    pdb->id_root = id_insert(pdb->id_root, id, 0, &status);
    return status == 0 ? 0 : -1;
}

int UnregisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node)
        return 1;  /* not found */
    int status = 0;
    pdb->id_root = id_delete(pdb->id_root, id, &status);
    return 0;
}

int UpdatePlayerMMR(MMR_DB_T db, const char *id, int mmr_change) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node) return -1;
    int new_mmr = node->mmr + mmr_change;
    if (new_mmr < 0)
        new_mmr = 0;
    node->mmr = new_mmr;
    return new_mmr;
}

int GetMMRByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node) return -1;
    return node->mmr;
}

/* --- Rank Query functions --- */

/* Helper function for traversal */

int traverse_2(IDNode *node, IDNode *target, int (*fp)(const char *, int, IDNode *)) {
    if (!node) return 0;
    int count = fp(node->id, node->mmr, target) ? 1 : 0;
    count += traverse_2(node->left, target, fp);
    count += traverse_2(node->right, target, fp);
    return count;
}

int helper(const char *other_id, int other_mmr, IDNode *target) {
    if (other_mmr > target->mmr ||(other_mmr == target->mmr && strcmp(other_id, target->id) < 0)) return 1;
    return 0;
}

/* 
   GetRankByID:
   Computes a player’s rank based on descending mmr order.
   Rank is defined as one plus the number of players with either:
     - a strictly higher mmr, or
     - the same mmr but with an id that is lexicographically smaller.
   This function traverses the entire ID BST.
*/
int GetRankByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *target = id_find(pdb->id_root, id);
    if (!target) return 0; // not found

    int count = 0;
    count = traverse_2(pdb->id_root, target, helper);
    return count + 1;
}

/* 
   Helper: Count the number of nodes in the BST.
*/
static int count_nodes(IDNode *root) {
    if (!root) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

/* 
   Helper: Store pointers to each node in the BST into an array.
*/
static void store_nodes(IDNode *root, IDNode **arr, int *index) {
    if (!root) return;
    store_nodes(root->left, arr, index);
    arr[(*index)++] = root;
    store_nodes(root->right, arr, index);
}

/* Comparator for qsort: sorts by descending mmr; if equal, ascending id. */
static int cmp_nodes(const void *a, const void *b) {
    const IDNode *nodeA = *(const IDNode **)a;
    const IDNode *nodeB = *(const IDNode **)b;
    if (nodeA->mmr != nodeB->mmr)
        return nodeB->mmr - nodeA->mmr; /* descending order */
    return strcmp(nodeA->id, nodeB->id); /* ascending order for equal mmr */
}

/* 
   GetIDByRank:
   Returns the id of the player with the given rank (1-based)
   according to descending mmr order (and id tiebreaker).
   This function traverses the ID BST, stores nodes in an array,
   sorts the array, and then retrieves the kth element.
   Caller must free the returned string.
*/
char *GetIDByRank(MMR_DB_T db, int rank) {
    if (!db || rank <= 0) return NULL;
    MMR_DB *pdb = (MMR_DB *)db;
    int total = count_nodes(pdb->id_root);
    if (rank > total)
        return NULL;
    
    /* Allocate an array to hold pointers to all nodes. */
    IDNode **nodes = malloc(total * sizeof(IDNode *));
    if (!nodes) return NULL;
    int index = 0;
    store_nodes(pdb->id_root, nodes, &index);
    
    /* Sort the array by descending mmr and then ascending id. */
    qsort(nodes, total, sizeof(IDNode *), cmp_nodes);
    
    char *result = strdup(nodes[rank - 1]->id);
    free(nodes);
    return result;
}