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

/* --- DO NOT EDIT ABOVE THIS LINE --- */

/* Helper: find a player record in a chain. */
static PlayerRecord *find_player(PlayerRecord *head, const char *id) {
    while (head) {
        if (strcmp(head->id, id) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

/* Create a new hash table–based database. */
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

/* Free all memory allocated for the hash table database. */
void DestroyMMRDB(MMR_DB_T * db) {
    if (!db || !*db) return;  // Ensure the input pointer and database are valid

    struct MMR_DB *pdb = (struct MMR_DB *)(*db);
    int i;
    for (i = 0; i < pdb->bucket_count; i++) {
        PlayerRecord *curr = pdb->buckets[i];
        while (curr) {
            PlayerRecord *tmp = curr;
            curr = curr->next;
            free(tmp->id);  // Free dynamically allocated player ID
            free(tmp);      // Free the player record
        }
    }
    free(pdb->buckets); // Free the array of bucket pointers
    free(pdb);          // Free the database structure itself

    *db = NULL; // Set the pointer to NULL to prevent dangling references
}

/* Register a new player with an initial MMR of 0. */
int RegisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    unsigned int hash = hash_func(id) % pdb->bucket_count;
    if (find_player(pdb->buckets[hash], id))
        return 1;  /* duplicate */
    PlayerRecord *new_rec = malloc(sizeof(PlayerRecord));
    if (!new_rec) return -1;
    new_rec->id = strdup(id);
    if (!new_rec->id) { free(new_rec); return -1; }
    new_rec->mmr = 0;
    new_rec->next = pdb->buckets[hash];
    pdb->buckets[hash] = new_rec;
    pdb->count++;
    return 0;
}

/* Unregister a player from the database. */
int UnregisterPlayer(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    unsigned int hash = hash_func(id) % pdb->bucket_count;
    PlayerRecord *curr = pdb->buckets[hash], *prev = NULL;
    while (curr) {
        if (strcmp(curr->id, id) == 0) {
            if (prev)
                prev->next = curr->next;
            else
                pdb->buckets[hash] = curr->next;
            free(curr->id);
            free(curr);
            pdb->count--;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return 1;  /* not found */
}

/* Update a player’s MMR by the specified change. */
int UpdatePlayerMMR(MMR_DB_T db, const char *id, int mmr_change) {
    if (!db || !id) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    unsigned int hash = hash_func(id) % pdb->bucket_count;
    PlayerRecord *rec = find_player(pdb->buckets[hash], id);
    if (!rec) return -1;
    int new_mmr = rec->mmr + mmr_change;
    if (new_mmr < 0)
        new_mmr = 0;
    rec->mmr = new_mmr;
    return new_mmr;
}

/* Retrieve the MMR of a specified player. */
int GetMMRByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    unsigned int hash = hash_func(id) % pdb->bucket_count;
    PlayerRecord *rec = find_player(pdb->buckets[hash], id);
    if (!rec) return -1;
    return rec->mmr;
}

/* Count players for which the callback function returns true. */
int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp) {
    if (!db || !fp) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    int count = 0, i;
    for (i = 0; i < pdb->bucket_count; i++) {
        PlayerRecord *curr = pdb->buckets[i];
        while (curr) {
            if (fp(curr->id, curr->mmr))
                count++;
            curr = curr->next;
        }
    }
    return count;
}

/* For ranking queries we build a sorted array of all players. */
static int compare_players(const void *a, const void *b) {
    PlayerRecord *pa = *(PlayerRecord **)a;
    PlayerRecord *pb = *(PlayerRecord **)b;
    if (pb->mmr != pa->mmr)
        return pb->mmr - pa->mmr;  /* descending order */
    return strcmp(pa->id, pb->id);
}

/* Get a player's rank (1-based) in descending order of MMR. */
int GetRankByID(MMR_DB_T db, const char *id) {
    if (!db || !id) return -1;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    if (pdb->count == 0)
        return -1;
    PlayerRecord **arr = malloc(pdb->count * sizeof(PlayerRecord *));
    if (!arr) return -1;
    int index = 0, i;
    for (i = 0; i < pdb->bucket_count; i++) {
        PlayerRecord *curr = pdb->buckets[i];
        while (curr) {
            arr[index++] = curr;
            curr = curr->next;
        }
    }
    qsort(arr, pdb->count, sizeof(PlayerRecord *), compare_players);
    int rank = 1;
    int found = 0;
    for (i = 0; i < pdb->count; i++) {
        if (strcmp(arr[i]->id, id) == 0) {
            found = 1;
            break;
        }
        rank++;
    }
    free(arr);
    return found ? rank : 0;
}

/* Get the player's ID by rank. Caller must free the returned string. */
char *GetIDByRank(MMR_DB_T db, int rank) {
    if (!db || rank <= 0) return NULL;
    struct MMR_DB *pdb = (struct MMR_DB *)db;
    if (rank > pdb->count) return NULL;
    PlayerRecord **arr = malloc(pdb->count * sizeof(PlayerRecord *));
    if (!arr) return NULL;
    int index = 0, i;
    for (i = 0; i < pdb->bucket_count; i++) {
        PlayerRecord *curr = pdb->buckets[i];
        while (curr) {
            arr[index++] = curr;
            curr = curr->next;
        }
    }
    qsort(arr, pdb->count, sizeof(PlayerRecord *), compare_players);
    char *result = strdup(arr[rank - 1]->id);
    free(arr);
    return result;
}