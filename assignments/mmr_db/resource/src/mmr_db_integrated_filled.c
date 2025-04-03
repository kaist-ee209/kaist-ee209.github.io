#include "mmr_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_BUCKETS 1024

/* djb2 hash function */
static unsigned int hash_func(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return (unsigned int)hash;
}

/* --- DO NOT EDIT ABOVE THIS LINE --- */

/* Hash table node for fast ID lookup */
typedef struct PlayerRecord {
    char *id;
    int mmr;
    struct PlayerRecord *next;
} PlayerRecord;

/* MMR BST node for ranking queries.
   We sort in descending order by mmr; if equal, lexicographic order of id. */
typedef struct MMRNode {
    char *id;
    int mmr;
    int size;           /* number of nodes in the subtree */
    struct MMRNode *left, *right;
} MMRNode;

/* Integrated database structure holding both hash table and BST */
typedef struct MMR_DB {
    /* Hash table fields */
    PlayerRecord **buckets;
    int bucket_count;
    int count;
    /* MMR BST root (for ranking queries) */
    MMRNode *mmr_root;
} MMR_DB;

/* Find a player record in a chain */
static PlayerRecord *find_player(PlayerRecord *head, const char *id) {
    while (head) {
        if (strcmp(head->id, id) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

/* ---------------------------
   Helper Functions: MMR BST
   --------------------------- */

/* Create a new MMR node */
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

/* Get size of subtree rooted at node */
static int mmr_size(MMRNode *node) {
    return node ? node->size : 0;
}

/* Update the size field of the node */
static MMRNode *mmr_update_size(MMRNode *node) {
    if (node)
        node->size = mmr_size(node->left) + mmr_size(node->right) + 1;
    return node;
}

/* Insert into MMR BST.
   Sorting: descending order by mmr; if equal, use lexicographic order of id */
static MMRNode *mmr_insert(MMRNode *root, const char *id, int mmr) {
    if (!root)
        return mmr_create_node(id, mmr);
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_insert(root->left, id, mmr);
    else
        root->right = mmr_insert(root->right, id, mmr);
    return mmr_update_size(root);
}

/* Find the minimum (by BST order) node in a subtree */
static MMRNode *mmr_find_min(MMRNode *root) {
    while (root && root->left)
        root = root->left;
    return root;
}

/* Delete from MMR BST.
   Returns the new tree root. */
static MMRNode *mmr_delete(MMRNode *root, const char *id, int mmr) {
    if (!root) return NULL;
    if (mmr > root->mmr || (mmr == root->mmr && strcmp(id, root->id) < 0))
        root->left = mmr_delete(root->left, id, mmr);
    else if (mmr < root->mmr || (mmr == root->mmr && strcmp(id, root->id) > 0))
        root->right = mmr_delete(root->right, id, mmr);
    else {
        // Node found. Delete it.
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
            // Replace with inorder successor from right subtree.
            MMRNode *succ = mmr_find_min(root->right);
            free(root->id);
            root->id = strdup(succ->id);
            root->mmr = succ->mmr;
            root->right = mmr_delete(root->right, succ->id, succ->mmr);
        }
    }
    return mmr_update_size(root);
}

/* Update a player's mmr in the BST: remove old record and insert new one */
static MMRNode *mmr_update(MMRNode *root, const char *id, int old_mmr, int new_mmr) {
    root = mmr_delete(root, id, old_mmr);
    root = mmr_insert(root, id, new_mmr);
    return root;
}

/* Return the kth node (1-based) in the BST (sorted descending) */
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

/* Recursively free the MMR BST */
static void mmr_destroy(MMRNode *root) {
    if (!root) return;
    mmr_destroy(root->left);
    mmr_destroy(root->right);
    free(root->id);
    free(root);
}

/* ---------------------------
   API Functions
   --------------------------- */

/* Create a new integrated database */
MMR_DB_T CreateMMRDB(void) {
    MMR_DB *db = malloc(sizeof(MMR_DB));
    if (!db) return NULL;
    db->bucket_count = INITIAL_BUCKETS;
    db->count = 0;
    db->buckets = calloc(db->bucket_count, sizeof(PlayerRecord *));
    if (!db->buckets) {
        free(db);
        return NULL;
    }
    db->mmr_root = NULL;
    return (MMR_DB_T)db;
}

/* Destroy the integrated database */
void DestroyMMRDB(MMR_DB_T *db_ptr) {
    if (!db_ptr || !*db_ptr)
        return;
    MMR_DB *db = (MMR_DB *)(*db_ptr);
    // Free hash table records
    int i;
    for (i = 0; i < db->bucket_count; i++) {
        PlayerRecord *curr = db->buckets[i];
        while (curr) {
            PlayerRecord *tmp = curr;
            curr = curr->next;
            free(tmp->id);
            free(tmp);
        }
    }
    free(db->buckets);
    // Free BST
    mmr_destroy(db->mmr_root);
    free(db);
    *db_ptr = NULL;
}

/* Register a new player (initial MMR 0) */
int RegisterPlayer(MMR_DB_T db_ptr, const char *id) {
    if (!db_ptr || !id)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    unsigned int hash = hash_func(id) % db->bucket_count;
    if (find_player(db->buckets[hash], id))
        return 1;  /* duplicate */

    // Create and insert into hash table
    PlayerRecord *new_rec = malloc(sizeof(PlayerRecord));
    if (!new_rec)
        return -1;
    new_rec->id = strdup(id);
    if (!new_rec->id) {
        free(new_rec);
        return -1;
    }
    new_rec->mmr = 0;
    new_rec->next = db->buckets[hash];
    db->buckets[hash] = new_rec;
    db->count++;

    // Insert into the MMR BST
    db->mmr_root = mmr_insert(db->mmr_root, id, 0);
    return 0;
}

/* Unregister a player */
int UnregisterPlayer(MMR_DB_T db_ptr, const char *id) {
    if (!db_ptr || !id)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    unsigned int hash = hash_func(id) % db->bucket_count;
    PlayerRecord *curr = db->buckets[hash], *prev = NULL;
    while (curr) {
        if (strcmp(curr->id, id) == 0)
            break;
        prev = curr;
        curr = curr->next;
    }
    if (!curr)
        return 1;  /* not found */

    // Remove from hash table
    if (prev)
        prev->next = curr->next;
    else
        db->buckets[hash] = curr->next;
    int player_mmr = curr->mmr;
    free(curr->id);
    free(curr);
    db->count--;

    // Remove from BST
    db->mmr_root = mmr_delete(db->mmr_root, id, player_mmr);
    return 0;
}

/* Update a player's MMR by mmr_change.
   Returns the new MMR or -1 if error.
   Also updates the BST accordingly.
 */
int UpdatePlayerMMR(MMR_DB_T db_ptr, const char *id, int mmr_change) {
    if (!db_ptr || !id)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    unsigned int hash = hash_func(id) % db->bucket_count;
    PlayerRecord *rec = find_player(db->buckets[hash], id);
    if (!rec)
        return -1;
    int old_mmr = rec->mmr;
    int new_mmr = old_mmr + mmr_change;
    if (new_mmr < 0)
        new_mmr = 0;
    rec->mmr = new_mmr;
    // Update BST: remove old value and insert updated record.
    db->mmr_root = mmr_update(db->mmr_root, id, old_mmr, new_mmr);
    return new_mmr;
}

/* Retrieve a player's MMR by ID */
int GetMMRByID(MMR_DB_T db_ptr, const char *id) {
    if (!db_ptr || !id)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    unsigned int hash = hash_func(id) % db->bucket_count;
    PlayerRecord *rec = find_player(db->buckets[hash], id);
    if (!rec)
        return -1;
    return rec->mmr;
}

/* Count players for which the callback function returns true.
   The callback fp is defined as: int fp(const char *id, int mmr)
 */
int CountPlayers(MMR_DB_T db_ptr, MMR_FUNC_T fp) {
    if (!db_ptr || !fp)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    int count = 0, i;
    for (i = 0; i < db->bucket_count; i++) {
        PlayerRecord *curr = db->buckets[i];
        while (curr) {
            if (fp(curr->id, curr->mmr))
                count++;
            curr = curr->next;
        }
    }
    return count;
}

/* Helper for traversing the MMR BST in order to compute rank.
   Traversal is done in descending order. */
static int traverse_MMR(MMRNode *root, const char *id, int *index) {
    if (!root)
        return 0;
    int rank = traverse_MMR(root->left, id, index);
    if (rank > 0)
        return rank;
    (*index)++;
    if (strcmp(root->id, id) == 0)
        return *index;
    return traverse_MMR(root->right, id, index);
}

/* Get a player's rank (1-based) in descending order of MMR */
int GetRankByID(MMR_DB_T db_ptr, const char *id) {
    if (!db_ptr || !id)
        return -1;
    MMR_DB *db = (MMR_DB *)db_ptr;
    // If player does not exist, return 0
    unsigned int hash = hash_func(id) % db->bucket_count;
    if (!find_player(db->buckets[hash], id))
        return 0;
    int index = 0;
    return traverse_MMR(db->mmr_root, id, &index);
}

/* Get the player's ID by rank.
   Caller must free the returned string.
 */
char *GetIDByRank(MMR_DB_T db_ptr, int rank) {
    if (!db_ptr || rank <= 0)
        return NULL;
    MMR_DB *db = (MMR_DB *)db_ptr;
    if (rank > mmr_size(db->mmr_root))
        return NULL;
    MMRNode *node = mmr_kth(db->mmr_root, rank);
    if (!node)
        return NULL;
    return strdup(node->id);
}