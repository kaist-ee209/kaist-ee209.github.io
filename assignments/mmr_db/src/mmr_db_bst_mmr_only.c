#include "mmr_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --- New structure: only one tree keyed by MMR (with id as a field) --- */
typedef struct MMRNode {
    char *id;
    int mmr;
    int size;  /* size of subtree */
    struct MMRNode *left, *right;
} MMRNode;

typedef struct MMR_DB {
    MMRNode *mmr_root;
} MMR_DB;

/* --- MMR BST helper functions --- */

/* Create a new MMR node. */
static MMRNode *mmr_create_node(const char *id, int mmr) {
    MMRNode *node = malloc(sizeof(MMRNode));
    if (!node) return NULL;
    node->id = strdup(id);
    if (!node->id) { free(node); return NULL; }
    node->mmr = mmr;
    node->size = 1;
    node->left = node->right = NULL;
    return node;
}

/* Return the size of the subtree rooted at node. */
static int mmr_size(MMRNode *node) {
    return node ? node->size : 0;
}

/* Update the subtree size of a node. */
static MMRNode *mmr_update_size(MMRNode *node) {
    if (node)
        node->size = mmr_size(node->left) + mmr_size(node->right) + 1;
    return node;
}

/* 
   Insert a node into the MMR BST.
   Sorting: descending by mmr; if equal, sort by id lexicographically.
*/
static MMRNode *mmr_insert(MMRNode *root, const char *id, int mmr) {
    if (!root) return mmr_create_node(id, mmr);
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_insert(root->left, id, mmr);
    else
        root->right = mmr_insert(root->right, id, mmr);
    return mmr_update_size(root);
}

/* 
   Delete a node from the MMR BST.
   Note: mmr and id must match the node to be deleted.
*/
static MMRNode *mmr_delete(MMRNode *root, const char *id, int mmr) {
    if (!root) return NULL;
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_delete(root->left, id, mmr);
    else if (mmr < root->mmr || (mmr == root->mmr && strcmp(id, root->id) > 0))
        root->right = mmr_delete(root->right, id, mmr);
    else {
        if (!root->left) {
            MMRNode *temp = root->right;
            free(root->id);
            free(root);
            return temp;
        } else if (!root->right) {
            MMRNode *temp = root->left;
            free(root->id);
            free(root);
            return temp;
        } else {
            /* Find inorder successor (minimum in right subtree) */
            MMRNode *succ = root->right;
            while (succ->left)
                succ = succ->left;
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            root->right = mmr_delete(root->right, succ->id, succ->mmr);
        }
    }
    return mmr_update_size(root);
}

/* Destroy the MMR BST and free memory. */
static void mmr_destroy(MMRNode *root) {
    if (!root) return;
    mmr_destroy(root->left);
    mmr_destroy(root->right);
    free(root->id);
    free(root);
}

/* 
   Traverse the tree (any order) to find a node by ID.
   Since the BST is not keyed by ID, a full traversal is needed.
*/
static MMRNode *mmr_find(MMRNode *root, const char *id) {
    if (!root) return NULL;
    if (strcmp(root->id, id) == 0)
        return root;
    MMRNode *found = mmr_find(root->left, id);
    if (found) return found;
    return mmr_find(root->right, id);
}

/* 
   Update a player's mmr: remove the old record and insert a new one.
*/
static MMRNode *mmr_update(MMRNode *root, const char *id, int old_mmr, int new_mmr) {
    root = mmr_delete(root, id, old_mmr);
    root = mmr_insert(root, id, new_mmr);
    return root;
}

/* Return the kth node (1-based) in the MMR BST (sorted descending). */
static MMRNode *mmr_kth(MMRNode *root, int k) {
    if (!root) return NULL;
    int left_size = mmr_size(root->left);
    if (k == left_size + 1)
        return root;
    else if (k <= left_size)
        return mmr_kth(root->left, k);
    else
        return mmr_kth(root->right, k - left_size - 1);
}

/* 
   Traverse the MMR BST in order (descending order) to determine the rank of a given ID.
   *index keeps track of the position (rank) during the traversal.
*/
static int traverse_MMR(MMRNode *root, const char *id, int *index) {
    if (!root) return 0;
    int rank = traverse_MMR(root->left, id, index);
    if (rank > 0)
        return rank;
    (*index)++;
    if (strcmp(root->id, id) == 0)
        return *index;
    return traverse_MMR(root->right, id, index);
}

/* 
   Traverse the MMR tree and count nodes that satisfy the predicate function fp.
   The predicate should return a non-zero value if the node should be counted.
*/
static int traverse_mmr(MMRNode *node, MMR_FUNC_T fp) {
    if (!node) return 0;
    int count = fp(node->id, node->mmr) ? 1 : 0;
    count += traverse_mmr(node->left, fp);
    count += traverse_mmr(node->right, fp);
    return count;
}

/* --- BST API functions --- */

/* Create a new MMR database. */
MMR_DB_T CreateMMRDB(void) {
    MMR_DB *db = malloc(sizeof(MMR_DB));
    if (!db) return NULL;
    db->mmr_root = NULL;
    return (MMR_DB_T)db;
}

/* Destroy the MMR database and free all associated memory. */
void DestroyMMRDB(MMR_DB_T *db) {
    if (!db || !*db) return;
    MMR_DB *pdb = (MMR_DB *)(*db);
    mmr_destroy(pdb->mmr_root);
    free(pdb);
    *db = NULL;
}

/* Register a new player with initial MMR 0. */
int RegisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    if (mmr_find(pdb->mmr_root, id))
        return 1;  /* duplicate */
    pdb->mmr_root = mmr_insert(pdb->mmr_root, id, 0);
    return 0;
}

/* Unregister a player (delete from the database). */
int UnregisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    MMRNode *node = mmr_find(pdb->mmr_root, id);
    if (!node)
        return 1;
    int mmr = node->mmr;
    pdb->mmr_root = mmr_delete(pdb->mmr_root, id, mmr);
    return 0;
}

/* Update a player's MMR by mmr_change (cannot go below 0). */
int UpdatePlayerMMR(MMR_DB_T db, const char *id, int mmr_change) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    MMRNode *node = mmr_find(pdb->mmr_root, id);
    if (!node)
        return -1;
    int old_mmr = node->mmr;
    int new_mmr = old_mmr + mmr_change;
    if (new_mmr < 0)
        new_mmr = 0;
    pdb->mmr_root = mmr_update(pdb->mmr_root, id, old_mmr, new_mmr);
    return new_mmr;
}

/* Get a player's MMR by ID. Returns -1 if not found. */
int GetMMRByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    MMRNode *node = mmr_find(pdb->mmr_root, id);
    if (!node) return -1;
    return node->mmr;
}

/* Count players using an in-order traversal of the MMR BST that satisfy fp. */
int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp) {
    if (!db || !fp) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    return traverse_mmr(pdb->mmr_root, fp);
}

/* Get rank of player by ID (1-based rank, descending order by MMR). */
int GetRankByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    MMRNode *node = mmr_find(pdb->mmr_root, id);
    if (!node) return 0;
    int index = 0;
    return traverse_MMR(pdb->mmr_root, id, &index);
}

/* Get the player ID by rank.
   Caller must free the returned string. */
char *GetIDByRank(MMR_DB_T db, int rank) {
    if (!db || rank <= 0) return NULL;
    MMR_DB *pdb = (MMR_DB *)db;
    if (rank > (pdb->mmr_root ? pdb->mmr_root->size : 0))
        return NULL;
    MMRNode *node = mmr_kth(pdb->mmr_root, rank);
    if (!node) return NULL;
    return strdup(node->id);
}