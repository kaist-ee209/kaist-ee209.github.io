#ifndef MMR_DB_H
#define MMR_DB_H

#include <stdbool.h>

/* Type for the game ranking database. */
typedef struct MMR_DB *MMR_DB_T;

/* Function pointer type for filtering players. */
typedef bool (*MMR_FUNC_T)(const char *id, int mmr);

/* Database Management API */
MMR_DB_T CreateMMRDB(void);
void DestroyMMRDB(MMR_DB_T * db);
int RegisterPlayer(MMR_DB_T db, const char *id);
int UnregisterPlayer(MMR_DB_T db, const char *id);
int UpdatePlayerMMR(MMR_DB_T db, const char *id, int mmr_change);

/* MMR Query API */
int GetMMRByID(MMR_DB_T db, const char *id);
int CountPlayers(MMR_DB_T db, MMR_FUNC_T fp);

/* Rank Query API */
int GetRankByID(MMR_DB_T db, const char *id);
char *GetIDByRank(MMR_DB_T db, int rank);

#endif /* MMR_DB_H */