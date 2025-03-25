#include <stdio.h>
#include <string.h>
#include "test_suite.h"
#include "../src/mmr_db.h" 

/*
 * main() simply:
 * 1) Asks for implementation name (ht, bst, integrated)
 * 2) Sets up the MMR_Interface function pointers
 * 3) Runs the tests & prints performance stats
 */

int main(void) {
    char implName[20];
    printf("Enter the implementation name (ht, bst, integrated): ");
    scanf("%19s", implName);

    /* Build up the MMR_Interface with pointers to the actual MMR DB functions. */
    MMR_Interface Iface = {
        .CreateDB       = CreateMMRDB,
        .DestroyDB      = DestroyMMRDB,
        .RegisterPlayer = RegisterPlayer,
        .UnregisterPlayer = UnregisterPlayer,
        .UpdatePlayerMMR = UpdatePlayerMMR,
        .GetMMRByID     = GetMMRByID,
        .CountPlayers   = CountPlayers,
        .GetRankByID    = GetRankByID,
        .GetIDByRank    = GetIDByRank,
        .implName       = ""
    };

    if (strcmp(implName, "ht") == 0) {
        Iface.implName = "Hash Table";
    } else if (strcmp(implName, "bst") == 0) {
        Iface.implName = "Binary Search Tree";
    } else if (strcmp(implName, "integrated") == 0) {
        Iface.implName = "Integrated";
    } else {
        printf("Invalid implementation name.\n");
        return 1;
    }

    /* Run functional tests */
    run_all_tests(&Iface);

    /* Implementation-specific performance tests */
    if (strcmp(implName, "ht") == 0) {
        long UpdateTimeUS = measure_update_time(&Iface, 10000);
        if(UpdateTimeUS > 0) {printf("Update Time: %ld us\n", UpdateTimeUS);}
        printf("Blackbox Performance Test: HT expansion Implementation (+5 points): %s\n",
               (UpdateTimeUS <= 1000) && (UpdateTimeUS > 0) ? "PASS" : "FAIL");
    }
    else if (strcmp(implName, "bst") == 0) {
        long GetMMRTimeUS = measure_query_time(&Iface, 1000, true, true, query_getmmr);
        if(GetMMRTimeUS > 0) {printf("Get MMR Time: %ld us\n", GetMMRTimeUS);}
        printf("Blackbox Performance Test: ID indexed BST Implementation (-5 points): %s\n",
               (GetMMRTimeUS <= 400) && (GetMMRTimeUS > 0) ? "PASS" : "FAIL");

        long GetIDByRankTimeUS = measure_query_time(&Iface, 1000, true, true, query_getIDbyrank);
        if(GetIDByRankTimeUS > 0) {printf("Get ID by Rank Time: %ld us\n", GetIDByRankTimeUS);}
        printf("Blackbox Performance Test: MMR indexed BST Implementation (-5 points): %s\n",
               (GetIDByRankTimeUS <= 400) && (GetIDByRankTimeUS > 0) ? "PASS" : "FAIL");

        long GetIDByRankTimeUS2 = measure_query_time(&Iface, 1000, true, false, query_getIDbyrank);
        if(GetIDByRankTimeUS2 > 0) {printf("Get ID by Rank Time (worst case): %ld us\n", GetIDByRankTimeUS2);}
        double ratio = (double)GetIDByRankTimeUS2 / GetIDByRankTimeUS;
        bool pass = (ratio < 2) && (GetIDByRankTimeUS <= 400) && (GetMMRTimeUS <= 400);
        pass = pass && (GetIDByRankTimeUS2 > 0) && (GetIDByRankTimeUS > 0) && (GetMMRTimeUS > 0);
        printf("Blackbox Performance Test: Self-balancing BST Implementation (+5 points): %s\n",
               pass ? "PASS" : "FAIL");
    }
    else if (strcmp(implName, "integrated") == 0) {
        long GetMMRTimeUS = measure_query_time(&Iface, 5000, true, true, query_getmmr);
        if(GetMMRTimeUS > 0) {printf("Get MMR Time: %ld us\n", GetMMRTimeUS);}
        printf("Blackbox Performance Test: GetMMRbyID efficiency: %s\n",
               (GetMMRTimeUS <= 1400) && (GetMMRTimeUS > 0) ? "PASS" : "FAIL");

        long CountTimeUS = measure_query_time(&Iface, 5000, true, true, query_count);
        if(CountTimeUS > 0) {printf("Count Time: %ld us\n", CountTimeUS);}
        printf("Blackbox Performance Test: CountPlayers efficiency: %s\n",
               (CountTimeUS <= 520000) && (CountTimeUS > 0) ? "PASS" : "FAIL");

        long GetRankTimeUS = measure_query_time(&Iface, 1000, true, true, query_getrank);
        if(GetRankTimeUS > 0) {printf("Get Rank Time: %ld us\n", GetRankTimeUS);}
        printf("Blackbox Performance Test: GetRankByID efficiency: %s\n",
               (GetRankTimeUS > 0) && (GetRankTimeUS <= 20000) ? "PASS" : "FAIL");

        long GetIDByRankTimeUS = measure_query_time(&Iface, 1000, true, true, query_getIDbyrank);
        if(GetIDByRankTimeUS > 0) {printf("Get ID by Rank Time: %ld us\n", GetIDByRankTimeUS);}
        printf("Blackbox Performance Test: GetIDByRank efficiency: %s\n",
               (GetIDByRankTimeUS <= 400) && (GetIDByRankTimeUS > 0) ? "PASS" : "FAIL");
    }

    return 0;
}