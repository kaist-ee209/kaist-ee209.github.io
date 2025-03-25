#include "mmr_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --- Updated BST node structures with AVL fields --- */

/* AVL node for ID-indexed tree */
typedef struct IDNode {
    char *id;
    int mmr;
    int height;             /* AVL height */
    struct IDNode *left, *right;
} IDNode;

/* AVL node for MMR-indexed tree */
typedef struct MMRNode {
    char *id;
    int mmr;
    int size;               /* subtree size (for kth element) */
    int height;             /* AVL height */
    struct MMRNode *left, *right;
} MMRNode;

/* The BST-based database holds two trees: one keyed by ID and one by MMR. */
typedef struct MMR_DB {
    IDNode *id_root;
    MMRNode *mmr_root;
} MMR_DB;

/* --- Helper functions for AVL balancing --- */

/* Returns maximum of two integers. */
static int max(int a, int b) {
    return (a > b) ? a : b;
}

/* ---------- For ID BST ---------- */
static int id_height(IDNode *node) {
    return node ? node->height : 0;
}

static void id_update_height(IDNode *node) {
    if (node)
        node->height = 1 + max(id_height(node->left), id_height(node->right));
}

static int id_get_balance(IDNode *node) {
    return node ? id_height(node->left) - id_height(node->right) : 0;
}

static IDNode *id_rotate_right(IDNode *y) {
    IDNode *x = y->left;
    IDNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    id_update_height(y);
    id_update_height(x);
    return x;
}

static IDNode *id_rotate_left(IDNode *x) {
    IDNode *y = x->right;
    IDNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    id_update_height(x);
    id_update_height(y);
    return y;
}

/* ---------- For MMR BST ---------- */
static int mmr_height(MMRNode *node) {
    return node ? node->height : 0;
}

static int mmr_size(MMRNode *node) {
    return node ? node->size : 0;
}

static void mmr_update_node(MMRNode *node) {
    if (node) {
        node->height = 1 + max(mmr_height(node->left), mmr_height(node->right));
        node->size = mmr_size(node->left) + mmr_size(node->right) + 1;
    }
}

static int mmr_get_balance(MMRNode *node) {
    return node ? mmr_height(node->left) - mmr_height(node->right) : 0;
}

static MMRNode *mmr_rotate_right(MMRNode *y) {
    MMRNode *x = y->left;
    MMRNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    mmr_update_node(y);
    mmr_update_node(x);
    return x;
}

static MMRNode *mmr_rotate_left(MMRNode *x) {
    MMRNode *y = x->right;
    MMRNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    mmr_update_node(x);
    mmr_update_node(y);
    return y;
}

/* --- DO NOT EDIT BELOW THIS LINE (interface functions) --- */

/* --- ID BST functions with AVL rebalancing --- */

/* Insert a node keyed by id in the AVL tree. */
static IDNode *id_insert(IDNode *root, const char *id, int mmr, int *status) {
    if (!root) {
        IDNode *node = malloc(sizeof(IDNode));
        if (!node) { *status = -1; return NULL; }
        node->id = strdup(id);
        node->mmr = mmr;
        node->left = node->right = NULL;
        node->height = 1;
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

    id_update_height(root);
    int balance = id_get_balance(root);
    /* Left Left Case */
    if (balance > 1 && strcmp(id, root->left->id) < 0)
        return id_rotate_right(root);
    /* Right Right Case */
    if (balance < -1 && strcmp(id, root->right->id) > 0)
        return id_rotate_left(root);
    /* Left Right Case */
    if (balance > 1 && strcmp(id, root->left->id) > 0) {
        root->left = id_rotate_left(root->left);
        return id_rotate_right(root);
    }
    /* Right Left Case */
    if (balance < -1 && strcmp(id, root->right->id) < 0) {
        root->right = id_rotate_right(root->right);
        return id_rotate_left(root);
    }
    return root;
}

/* Find a node in the ID AVL BST. */
static IDNode *id_find(IDNode *root, const char *id) {
    if (!root) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp == 0) return root;
    return cmp < 0 ? id_find(root->left, id) : id_find(root->right, id);
}

/* Delete a node from the ID AVL BST. */
static IDNode *id_delete(IDNode *root, const char *id, int *status) {
    if (!root) { *status = 1; return NULL; }
    int cmp = strcmp(id, root->id);
    if (cmp < 0)
        root->left = id_delete(root->left, id, status);
    else if (cmp > 0)
        root->right = id_delete(root->right, id, status);
    else {
        if (!root->left || !root->right) {
            IDNode *temp = root->left ? root->left : root->right;
            if (!temp) {  // No child case
                free(root->id);
                free(root);
                *status = 0;
                return NULL;
            } else {
                /* One child case: copy the child into root */
                IDNode *temp2 = temp;
                free(root->id);
                *root = *temp2;
                free(temp2);
            }
        } else {
            /* Two children: find inorder successor */
            IDNode *succ = root->right;
            while (succ->left)
                succ = succ->left;
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            root->right = id_delete(root->right, succ->id, status);
        }
    }
    if (!root) return root;
    id_update_height(root);
    int balance = id_get_balance(root);
    /* Left Left Case */
    if (balance > 1 && id_get_balance(root->left) >= 0)
        return id_rotate_right(root);
    /* Left Right Case */
    if (balance > 1 && id_get_balance(root->left) < 0) {
        root->left = id_rotate_left(root->left);
        return id_rotate_right(root);
    }
    /* Right Right Case */
    if (balance < -1 && id_get_balance(root->right) <= 0)
        return id_rotate_left(root);
    /* Right Left Case */
    if (balance < -1 && id_get_balance(root->right) > 0) {
        root->right = id_rotate_right(root->right);
        return id_rotate_left(root);
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

/* --- MMR BST functions with AVL rebalancing --- */

/* Create a new MMR node (initializing height as well). */
static MMRNode *mmr_create_node(const char *id, int mmr) {
    MMRNode *node = malloc(sizeof(MMRNode));
    if (!node) return NULL;
    node->id = strdup(id);
    if (!node->id) { free(node); return NULL; }
    node->mmr = mmr;
    node->size = 1;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

static MMRNode *mmr_insert(MMRNode *root, const char *id, int mmr) {
    if (!root)
        return mmr_create_node(id, mmr);
    /* Sorting: descending by mmr; if equal, lexicographically by id */
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_insert(root->left, id, mmr);
    else
        root->right = mmr_insert(root->right, id, mmr);

    mmr_update_node(root);
    int balance = mmr_get_balance(root);
    /* Left Left Case */
    if (balance > 1) {
        if (mmr > root->left->mmr ||
           (mmr == root->left->mmr && strcmp(id, root->left->id) < 0))
            return mmr_rotate_right(root);
        else {
            root->left = mmr_rotate_left(root->left);
            return mmr_rotate_right(root);
        }
    }
    /* Right Right Case */
    if (balance < -1) {
        if (mmr < root->right->mmr ||
           (mmr == root->right->mmr && strcmp(id, root->right->id) >= 0))
            return mmr_rotate_left(root);
        else {
            root->right = mmr_rotate_right(root->right);
            return mmr_rotate_left(root);
        }
    }
    return root;
}

static MMRNode *mmr_delete(MMRNode *root, const char *id, int mmr) {
    if (!root) return NULL;
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_delete(root->left, id, mmr);
    else if (mmr < root->mmr || (mmr == root->mmr && strcmp(id, root->id) > 0))
        root->right = mmr_delete(root->right, id, mmr);
    else {
        if (!root->left || !root->right) {
            MMRNode *temp = root->left ? root->left : root->right;
            if (!temp) {
                free(root->id);
                free(root);
                return NULL;
            } else {
                MMRNode *temp2 = temp;
                free(root->id);
                *root = *temp2;
                free(temp2);
            }
        } else {
            MMRNode *succ = root->right;
            while (succ->left)
                succ = succ->left;
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            root->right = mmr_delete(root->right, succ->id, succ->mmr);
        }
    }
    if (!root) return root;
    mmr_update_node(root);
    int balance = mmr_get_balance(root);
    /* Left Left Case */
    if (balance > 1) {
        if (mmr_get_balance(root->left) >= 0)
            return mmr_rotate_right(root);
        else {
            root->left = mmr_rotate_left(root->left);
            return mmr_rotate_right(root);
        }
    }
    /* Right Right Case */
    if (balance < -1) {
        if (mmr_get_balance(root->right) <= 0)
            return mmr_rotate_left(root);
        else {
            root->right = mmr_rotate_right(root->right);
            return mmr_rotate_left(root);
        }
    }
    return root;
}

static void mmr_destroy(MMRNode *root) {
    if (!root) return;
    mmr_destroy(root->left);
    mmr_destroy(root->right);
    free(root->id);
    free(root);
}

/* Update a player's mmr in the MMR AVL BST: remove the old record and insert the new one. */
static MMRNode *mmr_update(MMRNode *root, const char *id, int old_mmr, int new_mmr) {
    root = mmr_delete(root, id, old_mmr);
    root = mmr_insert(root, id, new_mmr);
    return root;
}

/* Return the kth node (1-based) in the MMR AVL BST (sorted descending). */
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
    if (!db || !*db) return;
    MMR_DB *pdb = (MMR_DB *)(*db);
    id_destroy(pdb->id_root);
    mmr_destroy(pdb->mmr_root);
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
    int old_mmr = node->mmr;
    node->mmr = new_mmr;
    pdb->mmr_root = mmr_update(pdb->mmr_root, id, old_mmr, new_mmr);
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

int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp) {
    if (!db || !fp) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    int count = traverse(pdb->id_root, fp);
    return count;
}

int traverse_MMR(MMRNode *root, const char *id, int *index) {
    if (!root) return 0;
    int rank = traverse_MMR(root->left, id, index);
    if (rank > 0) return rank;
    (*index)++;
    if (strcmp(root->id, id) == 0)
        return *index;
    return traverse_MMR(root->right, id, index);
}

int GetRankByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    MMR_DB *pdb = (MMR_DB *)db;
    IDNode *node = id_find(pdb->id_root, id);
    if (!node) return 0;
    int index = 0;
    return traverse_MMR(pdb->mmr_root, id, &index);
}

char *GetIDByRank(MMR_DB_T db, int rank) {
    if (!db || rank <= 0) return NULL;
    MMR_DB *pdb = (MMR_DB *)db;
    if (rank > (pdb->mmr_root ? pdb->mmr_root->size : 0))
        return NULL;
    MMRNode *node = mmr_kth(pdb->mmr_root, rank);
    if (!node) return NULL;
    return strdup(node->id);
}