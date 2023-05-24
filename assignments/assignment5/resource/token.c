#include <stdlib.h>
#include <string.h>
#include "token.h"

/*--------------------------------------------------------------------*/

void
freeToken(void *pvItem, void *pvExtra) {

  /* Free token pvItem.  pvExtra is unused. */

  struct Token *psToken = (struct Token*)pvItem;

  if (psToken->pcValue != NULL)
    free(psToken->pcValue);

  free(psToken);
}

/*--------------------------------------------------------------------*/

struct Token *
makeToken(enum TokenType eTokenType,
    char *pcValue) {

  /* Create and return a Token whose type is eTokenType and whose
     value consists of string pcValue.  Return NULL if insufficient
     memory is available.  The caller owns the Token. */

  struct Token *psToken;

  psToken = (struct Token*)malloc(sizeof(struct Token));
  if (psToken == NULL)
    return NULL;

  psToken->eType = eTokenType;

  if (pcValue != NULL) {
    psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
    if (psToken->pcValue == NULL) {
      free(psToken);
      return NULL;
    }

    strcpy(psToken->pcValue, pcValue);
  } else psToken->pcValue = NULL;

  return psToken;
}
