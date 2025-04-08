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

/* --- Helper functions for ID-indexed BST --- */
static IDNode *id_insert(IDNode *root, const char *id, int mmr, int *status) {
    return NULL;
}
static IDNode *id_find(IDNode *root, const char *id) {
    return NULL;
}
static IDNode *id_delete(IDNode *root, const char *id, int *status) {
    return NULL;
}
static void id_destroy(IDNode *root) {
}

/* --- Helper functions for MMR-indexed BST --- */
static MMRNode *mmr_insert(MMRNode *root, const char *id, int mmr) {
    return NULL;
}
static MMRNode *mmr_delete(MMRNode *root, const char *id, int mmr) {
    return NULL;
}
static void mmr_destroy(MMRNode *root) {
}
static MMRNode *mmr_kth(MMRNode *root, int k) {
    return NULL;
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
}

int RegisterPlayer(MMR_DB_T db, const char *id) {
    return -1;
}

int UnregisterPlayer(MMR_DB_T db, const char *id) {
    return -1;
}

int UpdatePlayerMMR(MMR_DB_T db, const char *id, int mmr_change) {
    return -1;
}

int GetMMRByID(MMR_DB_T db, const char *id) {
    return -1;
}

int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp) {
    return -1;
}

int GetRankByID(MMR_DB_T db, const char *id) {
    return -1;
}

char *GetIDByRank(MMR_DB_T db, int rank) {
    return NULL;
}
