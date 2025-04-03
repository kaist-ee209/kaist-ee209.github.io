#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include "../src/mmr_db.h"

/*
 * MMR_Interface structure holding pointers to the implementation being tested.
 */
typedef struct {
    MMR_DB_T (*CreateDB)(void);
    void (*DestroyDB)(MMR_DB_T*);
    int  (*RegisterPlayer)(MMR_DB_T, const char*);
    int  (*UnregisterPlayer)(MMR_DB_T, const char*);
    int  (*UpdatePlayerMMR)(MMR_DB_T, const char*, int);
    int  (*GetMMRByID)(MMR_DB_T, const char*);
    int  (*CountPlayers)(MMR_DB_T, bool (*)(const char*, int));
    int  (*GetRankByID)(MMR_DB_T, const char*);
    char* (*GetIDByRank)(MMR_DB_T, int);
    const char *implName;
} MMR_Interface;

/* Runs the entire suite of tests (create/destroy, registration, etc.). */
void run_all_tests(const MMR_Interface *iface);

/* Measures how long it takes to UpdatePlayerMMR() N times. */
long measure_update_time(const MMR_Interface *iface, int N);

/* 
 * For measure_query_time, we define a function pointer type to handle any
 * "per-iteration" query operation: GetMMRByID, CountPlayers, etc.
 */
typedef void (*QueryOp)(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i);

/*
 * Measures time to perform N queries (given by QueryOp).
 *  - id_shuffled: shuffle the ID insertion order
 *  - mmr_shuffled: shuffle the MMR update order
 */
long measure_query_time(const MMR_Interface *iface, int N, bool id_shuffled, bool mmr_shuffled, QueryOp op);

/* Concrete callbacks used by measure_query_time. */
void query_getmmr(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i);
void query_count(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i);
void query_getrank(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i);
void query_getIDbyrank(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i);

#endif /* TEST_SUITE_H */