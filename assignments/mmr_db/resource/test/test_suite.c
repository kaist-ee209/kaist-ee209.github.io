#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "test_suite.h"

/* Use a macro for convenience in test checks. */
static int totalScore = 0;
static int totalPossible = 0;

#define RUN_TEST(desc, condition)           \
    do {                                    \
        totalPossible++;                    \
        if (condition) {                    \
            totalScore++;                   \
            printf("[PASS] %s\n", desc);    \
        } else {                            \
            printf("[FAIL] %s\n", desc);    \
        }                                   \
    } while (0)

/* Example CountPlayers filters */
bool count_all(const char *id, int mmr) { (void)id; (void)mmr; return true; }
bool count_positive(const char *id, int mmr) { (void)id; return (mmr > 0); }

/* Helper for large arrays */
int* allocate_zeroed_int_array(int size) {
    int *arr = malloc(size * sizeof(int));
    if (!arr) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    memset(arr, 0, size * sizeof(int));
    return arr;
}

/* ------------------ TEST GROUPS ------------------ */
void run_create_destroy_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    bool created = (db != NULL);
    RUN_TEST("CreateDB: Create a new DB", created);
    iface->DestroyDB(&db);
    RUN_TEST("DestroyDB: Destroy a DB", db == NULL && created);
}

void run_registration_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    bool setup = db != NULL;

    int ret = iface->RegisterPlayer(NULL, "Alpha") + iface->RegisterPlayer(db, NULL);
    RUN_TEST("Registration: Register with invalid input", setup && ret == -2);
    setup =  setup && iface->RegisterPlayer(db, "Alpha") == 0;
    RUN_TEST("Registration: Register a new player", setup);
    bool pass = iface->RegisterPlayer(db, "Alpha") == 1 && setup;
    RUN_TEST("Registration: Duplicate registration", pass);
    
    ret = -1;
    int i;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        ret = iface->RegisterPlayer(db, name);
        if (ret!=0) break;
    }
    RUN_TEST("Registration: Register multiple players", ret == 0);
}

void run_unregistration_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();

    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    int ret = iface->UnregisterPlayer(NULL, "Alpha") + iface->UnregisterPlayer(db, NULL);
    bool pass = ret == -2 && setup;
    RUN_TEST("Unregister: Unregister with invalid input", pass);
    pass = iface->UnregisterPlayer(db, "Alpha") == 0 && setup;
    RUN_TEST("Unregister: Unregister an existing player", pass);
    pass = iface->UnregisterPlayer(db, "Alpha") == 1 && setup;
    RUN_TEST("Unregister: Unregister an unregistered player", pass);
    
    setup = setup && iface->RegisterPlayer(db, "Alpha") == 0;
    int unregistered[100] = {0}, i;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        setup = setup && iface->RegisterPlayer(db, name) == 0;
    }
    ret = 0;
    int temp_ret;
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            temp_ret = iface->UnregisterPlayer(db, name);
            if (temp_ret != 0) break;
            unregistered[i] = 1; ret++;
        }
    }
    temp_ret = 0;
    for (i = 0; i < 100; i++) {
        if (unregistered[i]) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            temp_ret += iface->UnregisterPlayer(db, name);
        }
    }
    RUN_TEST("Unregister: Unregister multiple players", ret == temp_ret && ret != 0);
    iface->DestroyDB(&db);
}

void run_update_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();

    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    int ret = iface->UpdatePlayerMMR(NULL, "Alpha", 50) + iface->UpdatePlayerMMR(db, NULL, 50);
    bool pass = ret == -2 && setup;
    RUN_TEST("Update: Update with invalid input", pass);
    pass = iface->UpdatePlayerMMR(db, "Alpha", 50) == 50 && setup;
    RUN_TEST("Update: Update an existing player", pass);
    pass = iface->UpdatePlayerMMR(db, "Alpha", 50) == 100 && setup;
    RUN_TEST("Update: Update an existing player (2)", pass);
    pass = iface->UpdatePlayerMMR(db, "Alpha", -110) == 0 && setup;
    RUN_TEST("Update: Update with negative MMR", pass);
    setup = setup && iface->UnregisterPlayer(db, "Alpha") == 0;
    pass = iface->UpdatePlayerMMR(db, "Bravo", 20) == 20 && iface->UpdatePlayerMMR(db, "Alpha", 50) == -1 && setup;
    RUN_TEST("Update: Update after unregistering one", pass);
    setup = setup && iface->UnregisterPlayer(db, "Bravo") == 0;

    int updatedMMR[100] = {0}, i;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        setup = setup && iface->RegisterPlayer(db, name) == 0;  
    }
    ret = 0;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        int mmr = iface->UpdatePlayerMMR(db, name, i);
        if (mmr != i) { ret = -1; break; }
        updatedMMR[i] = mmr;
    }
    RUN_TEST("Update: Update multiple players (1)", setup && ret == 0);
    
    ret = 0;
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            int mmr = iface->UpdatePlayerMMR(db, name, i);
            if (mmr != 2*i) { ret = -1; break; }
            updatedMMR[i] = mmr;
        }
    }
    RUN_TEST("Update: Update multiple players (2)", setup && ret == 0);
    
    int unregistered[100] = {0};
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            setup = setup &&
                    iface->UnregisterPlayer(db, name) == 0;
            unregistered[i] = 1;
        }
    }
    ret = 0;
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            int mmr = iface->UpdatePlayerMMR(db, name, 0);
            if (unregistered[i]) { if (mmr != -1) { ret = -1; break; } }
            else { if (mmr != updatedMMR[i]) { ret = -1; break; } }
        }
    }
    RUN_TEST("Update: Update multiple players (3)", setup && ret == 0);
    
    iface->DestroyDB(&db);
}

void run_getmmr_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();

    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    int mmr = iface->GetMMRByID(NULL, "Alpha") + iface->GetMMRByID(db, NULL);
    RUN_TEST("GetMMR: Get MMR with invalid input", mmr == -2 && setup);
    
    bool pass = iface->GetMMRByID(db, "Alpha") == 0 && setup;
    RUN_TEST("GetMMR: Get MMR of an existing player", pass);
    
    setup = setup && iface->UpdatePlayerMMR(db, "Alpha", 10) == 10;
    pass = iface->GetMMRByID(db, "Alpha") == 10 && setup;
    RUN_TEST("GetMMR: Get MMR of an updated player", pass);
    
    setup = setup && iface->UpdatePlayerMMR(db, "Bravo", -10 ) == 0;
    pass = iface->GetMMRByID(db, "Bravo") == 0 && setup;
    RUN_TEST("GetMMR: Get MMR of a negatively updated player", pass);

    setup = setup && iface->UnregisterPlayer(db, "Alpha") == 0;
    pass = iface->GetMMRByID(db, "Alpha") == -1 && iface->GetMMRByID(db, "Bravo") == 0 && setup;
    RUN_TEST("GetMMR: Get MMR after unregistering one", pass);
    iface->UnregisterPlayer(db, "Bravo");

    int ret = 0, i;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        setup = setup && iface->RegisterPlayer(db, name) == 0;
        setup = setup && iface->UpdatePlayerMMR(db, name, i) == i;
    }
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        mmr = iface->GetMMRByID(db, name);
        if (mmr != i) { ret = -1; break; }
    }
    RUN_TEST("GetMMR: Get MMR of multiple players (1)", ret == 0 && setup);

    int unregistered[100] = {0}, index[100] = {0};
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            setup = setup && iface->UnregisterPlayer(db, name) == 0;
            unregistered[i] = 1;
        }
        index[i] = i;
    }
    ret = 0;
    for (i = 0; i < 100; i++) {
        int j = index[i];
        char name[32]; snprintf(name, sizeof(name), "Player%03d", j);
        mmr = iface->GetMMRByID(db, name);
        if (unregistered[j]) { if (mmr != -1) { ret = -1; break; } }
        else { if (mmr != j) { ret = -1; break; } }
    }
    RUN_TEST("GetMMR: Get MMR of multiple players (2)", ret == 0 && setup);
    iface->DestroyDB(&db);
}

void run_count_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    int cnt = iface->CountPlayers(NULL, count_all) + iface->CountPlayers(db, NULL);
    RUN_TEST("Count: Count with invalid input", cnt == -2 && setup);

    setup = setup && iface->UpdatePlayerMMR(db, "Alpha", 50) == 50 && iface->UpdatePlayerMMR(db, "Bravo", -10) == 0;
    bool pass = iface->CountPlayers(db, count_all) == 2 && setup;
    RUN_TEST("Count: Count all players in db", pass);
    pass = iface->CountPlayers(db, count_positive) == 1 && setup;
    RUN_TEST("Count: Count players with positive MMR", pass);
    setup = setup && iface->UnregisterPlayer(db, "Bravo") == 0;
    pass = iface->CountPlayers(db, count_all) == 1 && setup;
    RUN_TEST("Count: Count all players after unregistering one", pass);
    setup = setup && iface->UnregisterPlayer(db, "Alpha") == 0;

    int numPlayers = 0, posPlayers = 0, i;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        if (rand() % 2 == 0) {
            setup = setup && iface->RegisterPlayer(db, name) == 0;
            numPlayers++;
            if (rand() % 2 == 0) {
                setup = setup && iface->UpdatePlayerMMR(db, name, i + 1) == i + 1;
                posPlayers++;
            }
        }
    }
    pass = iface->CountPlayers(db, count_all) == numPlayers && setup;
    RUN_TEST("Count: Count all players in db (2)", pass);
    pass = iface->CountPlayers(db, count_positive) == posPlayers && setup;
    RUN_TEST("Count: Count players with positive MMR (2)", pass);
    
    int posUnreg = 0, ret;
    for (i = 0; i < 100; i++) {
        if (rand() % 2 == 0) {
            char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
            ret = iface->GetMMRByID(db, name);
            if (ret > 0) posUnreg++;
            iface->UnregisterPlayer(db, name);
        }
    }
    pass = iface->CountPlayers(db, count_positive) == posPlayers - posUnreg && setup;
    RUN_TEST("Count: Count players with positive MMR after unregistering some", pass);
    iface->DestroyDB(&db);
}

void run_rank_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    
    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    int rank = iface->GetRankByID(NULL, "Alpha") + iface->GetRankByID(db, NULL);
    RUN_TEST("GetRank: Get rank with invalid input", rank == -2 && setup);
    
    setup = setup && iface->UpdatePlayerMMR(db, "Alpha", 50) == 50 && iface->UpdatePlayerMMR(db, "Bravo", 10) == 10;
    bool pass = iface->GetRankByID(db, "Bravo") == 2 && setup;
    RUN_TEST("GetRank: Get rank of an existing player", pass);

    setup = setup && iface->UnregisterPlayer(db, "Alpha") == 0;
    pass = iface->GetRankByID(db, "Alpha") == 0 && setup && iface->GetRankByID(db, "Bravo") == 1;
    RUN_TEST("GetRank: Get rank after unregister", pass);
    setup = setup && iface->UnregisterPlayer(db, "Bravo") == 0;
    
    int ret = 0, i, j;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        setup = setup && iface->RegisterPlayer(db, name) == 0;
        setup = setup && iface->UpdatePlayerMMR(db, name, i) == i;
    }
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        rank = iface->GetRankByID(db, name);
        if (rank != 100 - i) { ret = -1; break; }
    }
    RUN_TEST("GetRank: Get rank of multiple players (1)", setup && ret == 0);

    int unregistered[100] = {0}, mmr_values[100] = {0};
    for (i = 0; i< 100; i++) {
        mmr_values[i] = i;
    }
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        if (rand() % 2 == 0) { 
            setup = setup && iface->UnregisterPlayer(db, name) == 0;
            unregistered[i] = 1;
        }
        else { 
            int delta = 200 - 2 * i;
            mmr_values[i] += delta;
            setup = setup && iface->UpdatePlayerMMR(db, name, delta) == mmr_values[i];
        }
    }
    int sorted_indices[100];
    for (i = 0; i < 100; i++) sorted_indices[i] = i;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 99 - i; j++) {
            if (mmr_values[sorted_indices[j]] < mmr_values[sorted_indices[j + 1]]) {
                int tmp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = tmp;
            }
        }
    }
    ret = 0;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", sorted_indices[i]);
        rank = iface->GetRankByID(db, name);
        if (unregistered[sorted_indices[i]]) { if (rank != 0) { ret = -1; break; } }
        else { if (rank != i + 1) { ret = -1; break; } }
    }
    RUN_TEST("GetRank: Get rank of multiple players (2)", setup && ret == 0);
    
    iface->DestroyDB(&db);
}

void run_getid_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    
    bool setup = (db!=NULL) && iface->RegisterPlayer(db, "Alpha") == 0 && iface->RegisterPlayer(db, "Bravo") == 0;
    char *id = iface->GetIDByRank(NULL, 1);
    if (!id) { 
        id = iface->GetIDByRank(db, 0); 
        if (!id) { 
            id = iface->GetIDByRank(db, -1); 
            } }
    RUN_TEST("GetID: Get ID with invalid input", id == NULL && setup); free(id);
    
    setup = setup && iface->UpdatePlayerMMR(db, "Alpha", 50) == 50 && iface->UpdatePlayerMMR(db, "Bravo", 10) == 10;
    id = iface->GetIDByRank(db, 2);
    bool pass = id && strcmp(id, "Bravo") == 0;
    RUN_TEST("GetID: Get ID of an existing rank", setup && pass); free(id);
    
    setup = setup && iface->UnregisterPlayer(db, "Alpha") == 0;
    id = iface->GetIDByRank(db, 1);
    pass = id && strcmp(id, "Bravo") == 0;
    RUN_TEST("GetID: Get ID of a rank after unregister", setup && pass); free(id);
    
    setup = setup && iface->UnregisterPlayer(db, "Bravo") == 0;
    int ret = 0, i, j;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        setup = setup && iface->RegisterPlayer(db, name) == 0;
        setup = setup && iface->UpdatePlayerMMR(db, name, i) == i;
    }
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        id = iface->GetIDByRank(db, 100 - i);
        if (!id) { ret = -1; break; }
        if (strcmp(id, name) != 0) { ret = -1; break; }
        free(id);
    }
    RUN_TEST("GetID: Get ID of multiple ranks (1)", setup && ret == 0);
    
    ret = 0;
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        int rank = iface->GetRankByID(db, name);
        id = iface->GetIDByRank(db, rank);
        if (!id) { ret = -1; break; }
        if (strcmp(id, name) != 0) { ret = -1; break; }
        free(id);
    }
    RUN_TEST("GetID: Get ID of multiple ranks (2)", setup && ret == 0);
    
    ret = 0;
    int unregistered[100] = {0}, mmr_values[100] = {0};
    for (i = 0; i< 100; i++) {
        mmr_values[i] = i;
    }
    for (i = 0; i < 100; i++) {
        char name[32]; snprintf(name, sizeof(name), "Player%03d", i);
        if (rand() % 2 == 0) {
            setup = setup && iface->UnregisterPlayer(db, name) == 0; 
            unregistered[i] = 1; }
        else {
            int delta = 200 - 2 * i;
            mmr_values[i] += delta;
            setup = setup && iface->UpdatePlayerMMR(db, name, delta) == mmr_values[i]; 
        }
    }
    int sorted_indices[100]; for (i = 0; i < 100; i++) sorted_indices[i] = i;
    for (i = 0; i < 99; i++) {
        for (j = 0; j < 99 - i; j++) {
            if (mmr_values[sorted_indices[j]] < mmr_values[sorted_indices[j + 1]]) {
                int tmp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < 100; i++) {
        id = iface->GetIDByRank(db, i + 1);
        if (unregistered[sorted_indices[i]] == 0) {
            char expected[32]; snprintf(expected, sizeof(expected), "Player%03d", sorted_indices[i]);
            if (!id) { ret = -1; break; }
            if (strcmp(id, expected) != 0) { ret = -1; break; }
        } else {
            if (id) { ret = -1; break; }
        }
    }
    RUN_TEST("GetID: Get ID of multiple ranks (3)", setup && ret == 0);
    iface->DestroyDB(&db);
}

void run_scalability_tests(const MMR_Interface *iface) {
    MMR_DB_T db = iface->CreateDB();
    const int N = 3333; int pass = 1, ret = -1, i, j; char playerName[32];
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", i);
        ret = iface->RegisterPlayer(db, playerName);
        if (ret != 0) { pass = 0; break; }
    }
    RUN_TEST("Scalability: Register 3333 players", pass);
    
    ret = -1; pass = 1;
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", i);
        ret = iface->UnregisterPlayer(db, playerName);
        if (ret != 0) { pass = 0; break; }
    }
    RUN_TEST("Scalability: Unregister 3333 players", pass);
    
    ret = -1; pass = 1;
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", i);
        ret = iface->RegisterPlayer(db, playerName);
        if (ret != 0) { pass = 0; break; }
    }
    ret = -1; pass = 1;
    int *mmr_values = allocate_zeroed_int_array(N);
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", i);
        mmr_values[i] = 3*N - 2*i;
        ret = iface->UpdatePlayerMMR(db, playerName, mmr_values[i]);
        if (ret != mmr_values[i]) { pass = 0; break; }
    }
    RUN_TEST("Scalability: Update MMR of 3333 players", pass);
    
    pass = 1;
    pass = (iface->CountPlayers(db, count_all) == N);
    RUN_TEST("Scalability: Count all players", pass);

    pass = 1;
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", i);
        int mmr = iface->GetMMRByID(db, playerName);
        if (mmr != mmr_values[i]) { pass = 0; break; }
    }
    RUN_TEST("Scalability: Get MMR of 3333 players", pass);

    pass = 1;
    int sorted_indices[N];
    for (i = 0; i < N; i++) sorted_indices[i] = i;
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - i - 1; j++) {
            if (mmr_values[sorted_indices[j]] < mmr_values[sorted_indices[j + 1]]) {
                int tmp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = tmp;
            }
        }
    }
    pass = 1;
    for (i = 0; i < N; i++) {
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", sorted_indices[i]);
        int rank = iface->GetRankByID(db, playerName);
        if (rank != i + 1) { pass = 0; break; }
    }
    RUN_TEST("Scalability: Get rank of 3333 players", pass);
    pass = 1;
    for (i = 0; i < N; i++) {
        char *id = iface->GetIDByRank(db, i + 1);
        if (id == NULL) { pass = 0; break; }
        snprintf(playerName, sizeof(playerName), "BigTestPlayer%04d", sorted_indices[i]);
        if (strcmp(id, playerName) != 0) { pass = 0; }
        free(id);
    }
    RUN_TEST("Scalability: Get ID of 3333 players", pass);

    free(mmr_values);
    iface->DestroyDB(&db);
}

void run_all_tests(const MMR_Interface *iface) {
    totalScore = totalPossible = 0;
    printf("========================================\nRunning tests for [%s] Implementation:\n\n", iface->implName);
    run_create_destroy_tests(iface);
    run_registration_tests(iface);
    run_unregistration_tests(iface);
    run_update_tests(iface);
    run_getmmr_tests(iface);
    run_count_tests(iface);
    run_rank_tests(iface);
    run_getid_tests(iface);
    run_scalability_tests(iface);
    printf("\nImplementation [%s] result: %d out of %d\n", iface->implName, totalScore, totalPossible);
    printf("========================================\n");
}

/* ------------------ PERFORMANCE MEASUREMENT ------------------ */
long measure_update_time(const MMR_Interface *iface, int N) {
    MMR_DB_T db = iface->CreateDB();
    char **allIDs = malloc(N * sizeof(char*));
    if (!allIDs) { fprintf(stderr, "Out of memory.\n"); exit(1); }
    bool correctness = true;
    int i;
    for (i = 0; i < N; i++) {
        allIDs[i] = malloc(32);
        snprintf(allIDs[i], 32, "PerfTest%07d", i);
        correctness = correctness && iface->RegisterPlayer(db, allIDs[i]) == 0;
        iface->RegisterPlayer(db, allIDs[i]);
    }
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        correctness = correctness && iface->UpdatePlayerMMR(db, allIDs[i], i) == i;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long secDiff = end.tv_sec - start.tv_sec;
    long nsecDiff = end.tv_nsec - start.tv_nsec;
    long totalMicroseconds = secDiff * 1000000L + nsecDiff / 1000L;

    for (i=0; i<N; i++){
        correctness = correctness && iface->GetMMRByID(db, allIDs[i]) == i;
    }
    for (i = 0; i < N; i++) free(allIDs[i]);
    free(allIDs);
    iface->DestroyDB(&db);
    return correctness ? totalMicroseconds : -1;
}

/* QueryOp callbacks for measure_query_time */
void query_getmmr(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i) {
    iface->GetMMRByID(db, allIDs[i]);}
void query_count(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i) {
    iface->CountPlayers(db, count_all);}
void query_getrank(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i) {
    iface->GetRankByID(db, allIDs[i]);}
void query_getIDbyrank(const MMR_Interface *iface, MMR_DB_T db, char **allIDs, int i) {
    iface->GetIDByRank(db, i + 1);}

/* measure_query_time */
long measure_query_time(const MMR_Interface *iface, int N, bool id_shuffled, bool mmr_shuffled, QueryOp op) {
    MMR_DB_T db = iface->CreateDB();

    char **allIDs = malloc(N * sizeof(char*));
    int *id_values = malloc(N * sizeof(int));
    if (!id_values || !allIDs) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }
    bool correctness = true;
    int i, j;
    for (i = 0; i < N; i++) {
        id_values[i] = i + 1;
    }
    if (id_shuffled) {
        for (i = 0; i < N - 1; i++) {
            int j = i + rand() % (N - i);
            int tmp = id_values[i];
            id_values[i] = id_values[j];
            id_values[j] = tmp;
        }
    }
    for (i = 0; i < N; i++) {
        allIDs[i] = malloc(32);
        snprintf(allIDs[i], 32, "PerfTest%07d", id_values[i]);
        correctness = correctness && iface->RegisterPlayer(db, allIDs[i]) == 0;
    }

    int *mmr_values = malloc(N * sizeof(int));
    if (!mmr_values) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }
    for (i = 0; i < N; i++) {
        mmr_values[i] = i + 1;
    }
    if (mmr_shuffled) {
        for (i = 0; i < N - 1; i++) {
            int j = i + rand() % (N - i);
            int tmp = mmr_values[i];
            mmr_values[i] = mmr_values[j];
            mmr_values[j] = tmp;
        }
    }
    for (i = 0; i < N; i++) {
        correctness = correctness && iface->UpdatePlayerMMR(db, allIDs[i], mmr_values[i]) == mmr_values[i];
        correctness = correctness && iface->GetMMRByID(db, allIDs[i]) == mmr_values[i];
    }
    
    int sorted_indices[N];
    for (i = 0; i < N; i++) sorted_indices[i] = i;
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - i - 1; j++) {
            if (mmr_values[sorted_indices[j]] < mmr_values[sorted_indices[j + 1]]) {
                int tmp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = tmp;
            }
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        op(iface, db, allIDs, i);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Correctness check
    for (i = 0; i < N; i++){
        char *expectedID = malloc(32);
        snprintf(expectedID, 32, "PerfTest%07d", id_values[sorted_indices[i]]);
        if(op == query_getIDbyrank){
            char *id = iface->GetIDByRank(db, i + 1);
            correctness = correctness && strcmp(id, expectedID) == 0;
        }
        else if(op == query_getrank){
            correctness = correctness && iface->GetRankByID(db, expectedID) == i + 1;}
        else if(op == query_count){
            correctness = correctness && iface->CountPlayers(db, count_all) == N;}
        else if(op == query_getmmr){
            correctness = correctness && iface->GetMMRByID(db, allIDs[i]) == mmr_values[i];}
    }

    long secDiff = end.tv_sec - start.tv_sec;
    long nsecDiff = end.tv_nsec - start.tv_nsec;
    long totalMicroseconds = secDiff * 1000000L + nsecDiff / 1000L;

    for (i = 0; i < N; i++) {
        free(allIDs[i]);
    }
    free(allIDs);
    free(mmr_values);
    iface->DestroyDB(&db);

    return correctness ? totalMicroseconds : -1;
}