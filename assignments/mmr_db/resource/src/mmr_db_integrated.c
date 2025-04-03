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

MMR_DB_T CreateMMRDB(void) {
    return NULL;
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