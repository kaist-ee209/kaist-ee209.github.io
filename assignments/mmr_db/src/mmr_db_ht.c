#include "mmr_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_BUCKETS 1024

/* Structure for a player record in the hash table. */
typedef struct PlayerRecord {
    char *id;
    int mmr;
    struct PlayerRecord *next;
} PlayerRecord;

/* The hash table database structure. */
struct MMR_DB {
    PlayerRecord **buckets;
    int bucket_count;
    int count;
};

/* djb2 hash function */
static unsigned int hash_func(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return (unsigned int)hash;
}

/* Helper: find a player record in a chain. */
static PlayerRecord *find_player(PlayerRecord *head, const char *id) {
    while (head) {
        if (strcmp(head->id, id) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

MMR_DB_T CreateMMRDB(void) {
    struct MMR_DB *db = malloc(sizeof(struct MMR_DB));
    if (!db) return NULL;
    db->bucket_count = INITIAL_BUCKETS;
    db->count = 0;
    db->buckets = calloc(db->bucket_count, sizeof(PlayerRecord *));
    if (!db->buckets) {
        free(db);
        return NULL;
    }
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