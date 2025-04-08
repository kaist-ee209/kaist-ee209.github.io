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

/* BST node for MMR-indexed tree */
typedef struct MMRNode {
    char *id;
    int mmr;
    int size; /* size of subtree */
    struct MMRNode *left, *right;
} MMRNode;

/* The BST-based database holds two trees: one keyed by ID and one by MMR. */
typedef struct MMR_DB {
    IDNode *id_root;
    MMRNode *mmr_root;
} MMR_DB;

/* --- DO NOT EDIT ABOVE THIS LINE --- */

/* Insert a node keyed by id. */
static IDNode *id_insert(IDNode *root, const char *id, int mmr, int *status) {
    if (!root) {
        IDNode *node = malloc(sizeof(IDNode));
        if (!node) { *status = -1; return NULL; }
        node->id = strdup(id);
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

static void id_destroy(IDNode *root) {
    if (!root) return;
    id_destroy(root->left);
    id_destroy(root->right);
    free(root->id);
    free(root);
}

/* --- MMR BST functions --- */

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

static int mmr_size(MMRNode *node) {
    return node ? node->size : 0;
}

static MMRNode *mmr_update_size(MMRNode *node) {
    if (node)
        node->size = mmr_size(node->left) + mmr_size(node->right) + 1;
    return node;
}

/* Insert a node in the MMR BST.
   Sorting: descending order by mmr; if equal, use id lexicographically. */
static MMRNode *mmr_insert(MMRNode *root, const char *id, int mmr) {
    if (!root) return mmr_create_node(id, mmr);
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_insert(root->left, id, mmr);
    else
        root->right = mmr_insert(root->right, id, mmr);
    return mmr_update_size(root);
}

/* Delete a node from the MMR BST. */
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
            MMRNode *succ = root->right;
            while (succ->left) succ = succ->left;
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            root->right = mmr_delete(root->right, succ->id, succ->mmr);
        }
    }
    return mmr_update_size(root);
}

static void mmr_destroy(MMRNode *root) {
    if (!root) return;
    mmr_destroy(root->left);
    mmr_destroy(root->right);
    free(root->id);
    free(root);
}

/* Update a player's mmr in the MMR BST: remove the old record and insert the new one. */
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

/* --- BST API functions --- */

MMR_DB_T CreateMMRDB(void) {
    MMR_DB *db = malloc(sizeof(MMR_DB));
    if (!db) return NULL;
    db->id_root = NULL;
    db->mmr_root = NULL;
    return (MMR_DB_T)db;
}

void DestroyMMRDB(MMR_DB_T * db) {
    if (!db || !*db) return; // Ensure both db and *db are valid

    MMR_DB *pdb = (MMR_DB *)(*db);
    id_destroy(pdb->id_root);   // Free the ID-based BST
    mmr_destroy(pdb->mmr_root); // Free the MMR-based BST

    free(pdb);  // Free the main database structure
    *db = NULL; // Prevent dangling pointer
}

int RegisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    if (id_find(pdb->id_root, id))
        return 1;  /* duplicate */
    int status = 0;
    pdb->id_root = id_insert(pdb->id_root, id, 0, &status);
    if (status == -1) return -1;
    pdb->mmr_root = mmr_insert(pdb->mmr_root, id, 0);
    return 0;
}

int UnregisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node)
        return 1;
    int status = 0;
    int mmr = node->mmr;
    pdb->id_root = id_delete(pdb->id_root, id, &status);
    pdb->mmr_root = mmr_delete(pdb->mmr_root, id, mmr);
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
    pdb->mmr_root = mmr_update(pdb->mmr_root, id, node->mmr - mmr_change, new_mmr);
    return new_mmr;
}

int GetMMRByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node) return -1;
    return node->mmr;
}

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

int traverse_MMR(MMRNode *root, const char *id, int *index) {
    if (!root) return 0;

    int rank = 0;

    // Traverse left subtree
    rank = traverse_MMR(root->left, id, index);
    if (rank > 0) return rank; // If found in left subtree, return immediately

    // Process current node
    (*index)++; // Increment index to track position in sorted order
    if (strcmp(root->id, id) == 0)
        return *index; // Return rank if found

    // Traverse right subtree
    return traverse_MMR(root->right, id, index);
}

/* Get rank by traversing the MMR BST in order. */
int GetRankByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node) return 0;
    int index = 0; // Initialize rank tracker
    return traverse_MMR(pdb->mmr_root, id, &index);
}

/* Get the player ID by rank. Caller must free the returned string. */
char *GetIDByRank(MMR_DB_T db, int rank) {
    if (!db || rank <= 0) return NULL;
    MMR_DB *pdb = (MMR_DB *)db;
    if (rank > (pdb->mmr_root ? pdb->mmr_root->size : 0))
        return NULL;
    MMRNode *node = mmr_kth(pdb->mmr_root, rank);
    if (!node) return NULL;
    return strdup(node->id);
}
