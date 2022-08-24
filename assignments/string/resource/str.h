#ifndef _STR_H_
#define _STR_H_
#include <limits.h> /* for LONG_MIN, LONG_MAX */
#include <unistd.h> /* for typedef of size_t */

/* Part 1 */
size_t StrGetLength(const char* pcSrc);
char *StrCopy(char *pcDest, const char* pcSrc);
int StrCompare(const char* pcS1, const char* pcS2);
char *StrFindChr(const char *pcHaystack, int c);
char *StrFindStr(const char* pcHaystack, const char *pcNeedle);
char *StrConcat(char *pcDest, const char* pcSrc);
long int StrToLong(const char *nptr, char **endptr, int base);
int StrCaseCompare(const char* pcS1, const char* pcS2);

#endif /* _STR_H_ */
