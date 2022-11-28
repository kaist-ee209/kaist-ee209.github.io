#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lexsyn.h"
#include "token.h"
#include "util.h"

/*--------------------------------------------------------------------*/

static int
createToken(DynArray_T oTokens, enum TokenType ttype, char *value) {
  struct Token *psToken;

  psToken = makeToken(ttype, value);
  if (psToken == NULL) {
    errorPrint("Cannot allocate memory", FPRINTF);
    return FALSE;
  }
  if (! DynArray_add(oTokens, psToken)) {
    errorPrint("Cannot allocate memory", FPRINTF);
    return FALSE;
  }
  return TRUE;
}

/*--------------------------------------------------------------------*/
void command_lexLine(const char *pcLine, DynArray_T cTokens)
{
  enum commandState {STATE_START, STATE_IN_WORD};
  enum commandState cState = STATE_START;

  int iLineIndex = 0;
  int iValueIndex = 0;
  char c;
  char acValue[MAX_LINE_SIZE];

  assert(pcLine != NULL);
  assert(cTokens != NULL);

  for (;;){
    if(iLineIndex == MAX_LINE_SIZE)
      return ;

    c = pcLine[iLineIndex++];

    switch(cState) {
      case STATE_START:
        if ((c == '\n') || (c == '\0'))
          return ;
        else if (isspace(c))
          cState = STATE_START;
        else {
          acValue[iValueIndex++] = c;
          cState = STATE_IN_WORD;
        }
        break;

      case STATE_IN_WORD:
        if ((c == '\n') || (c == '\0')) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(cTokens, TOKEN_WORD, acValue) == FALSE)
            return ;

          iValueIndex = 0;

          return;
        } else if (isspace(c)) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(cTokens, TOKEN_WORD, acValue) == FALSE)
            return ;

          iValueIndex = 0;

          cState = STATE_START;
        }
        else {
          acValue[iValueIndex++] = c;
          cState = STATE_IN_WORD;
        }
        break;
      default:
        assert(FALSE);
    }

  }
}

enum AliasResult
alias_lexLine(const char *pcLine, DynArray_T oTokens){
  enum aliasState {STATE_START, STATE_IN_WORD, STATE_IN_EQUAL};
  enum aliasState aState = STATE_START;

  int iLineIndex = 0;
  int iValueIndex = 0;
  char c;
  char acValue[MAX_LINE_SIZE];

  assert(pcLine != NULL);
  assert(oTokens != NULL);

  for (;;){
    if(iLineIndex == MAX_LINE_SIZE)
      return ALIAS_LONG;

    c = pcLine[iLineIndex++];

    switch(aState) {
      case STATE_START:
        if ((c == '\n') || (c == '\0'))
          return ALIAS_SUCCESS;
        else if(c == '=') {
          return ALIAS_QERROR;
        }
        else {
          acValue[iValueIndex++] = c;
          aState = STATE_IN_WORD;
        }
        break;

      case STATE_IN_WORD:
        if ((c == '\n') || (c == '\0')) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return ALIAS_QERROR;

          iValueIndex = 0;

          return ALIAS_SUCCESS;
        }
        else if (c == '=') {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return ALIAS_QERROR;

          iValueIndex = 0;

          aState = STATE_IN_EQUAL;
        }
        else {
          acValue[iValueIndex++] = c;
          aState = STATE_IN_WORD;
        }
        break;
      case STATE_IN_EQUAL:
        if ((c == '\n') || (c == '\0')) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return ALIAS_QERROR;

          iValueIndex = 0;

          return ALIAS_SUCCESS;
        }
        else {
          acValue[iValueIndex++] = c;
          aState = STATE_IN_EQUAL;
        }
        break;
      default:
        assert(FALSE);
    }

  }

}

enum LexResult
lexLine_quote(const char *pcLine, DynArray_T oTokens) {

  /* lexLine() uses a DFA approach.  It "reads" its characters from
     pcLine. The difference from 'lexLine' is about STATE_IN_QUOTE
     here, we consider ' ' as a token like double quote
FIXME: The reason why we use duplicated-like function is simple 'lexLine' lesical analysis guide-line in EE209
shows ' as a normal character, not quotes.
So, we need to handle this as a normal character in general lesical analysis.
*/

  enum LexState {STATE_START, STATE_IN_NUMBER, STATE_IN_WORD, STATE_IN_DQUOTE, STATE_IN_QUOTE};

  enum LexState eState = STATE_START;

  int iLineIndex = 0;
  int iValueIndex = 0;
  char c;
  char acValue[MAX_LINE_SIZE];

  assert(pcLine != NULL);
  assert(oTokens != NULL);

  for (;;) {
    if (iLineIndex == MAX_LINE_SIZE)
      return LEX_LONG;
    /* "Read" the next character from pcLine. */
    c = pcLine[iLineIndex++];

    switch (eState) {
      case STATE_START:
        if ((c == '\n') || (c == '\0'))
          return LEX_SUCCESS;
        else if (isspace(c))
          eState = STATE_START;
        else if (c == '\"') {
          eState = STATE_IN_DQUOTE;
        }
        else if (c == '\'') {
          eState = STATE_IN_QUOTE;
        }
        else {
          acValue[iValueIndex++] = c;
          eState = STATE_IN_WORD;
        }
        break;
      case STATE_IN_QUOTE:
        if (c == '\'')
          eState = STATE_IN_WORD;
        else if ((c == '\n') || (c == '\0'))
          return LEX_QERROR;
        else
          acValue[iValueIndex++] = c;
        break;
      case STATE_IN_WORD:
        if ((c == '\n') || (c == '\0')) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          return LEX_SUCCESS;
        } else if (isspace(c)) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          eState = STATE_START;
        }
        else if (c == '\"') {
          eState = STATE_IN_DQUOTE;
        }
        else if( c== '\'' ){
          eState = STATE_IN_QUOTE;
        }
        else {
          acValue[iValueIndex++] = c;
          eState = STATE_IN_WORD;
        }
        break;
      case STATE_IN_DQUOTE:
        if (c == '\"')
          eState = STATE_IN_WORD;
        else if ((c == '\n') || (c == '\0'))
          return LEX_QERROR;
        else
          acValue[iValueIndex++] = c;

        break;
      default:
        assert(FALSE);
    }
  }
}


enum LexResult
lexLine(const char *pcLine, DynArray_T oTokens) {

  /* lexLine() uses a DFA approach.  It "reads" its characters from
     pcLine. */

  enum LexState {STATE_START, STATE_IN_NUMBER, STATE_IN_WORD, STATE_IN_DQUOTE, STATE_IN_QUOTE};

  enum LexState eState = STATE_START;

  int iLineIndex = 0;
  int iValueIndex = 0;
  char c;
  char acValue[MAX_LINE_SIZE];

  assert(pcLine != NULL);
  assert(oTokens != NULL);

  for (;;) {
    if (iLineIndex == MAX_LINE_SIZE)
      return LEX_LONG;
    /* "Read" the next character from pcLine. */
    c = pcLine[iLineIndex++];

    switch (eState) {
      case STATE_START:
        if ((c == '\n') || (c == '\0'))
          return LEX_SUCCESS;
        else if (isspace(c))
          eState = STATE_START;
        else if (c == '|') {
          /* Create a PIPE token. */
          if (createToken(oTokens, TOKEN_PIPE, NULL) == FALSE)
            return LEX_NOMEM;

          eState = STATE_START;
        }
        else if (c == '&') {
          // Create a Background command token.
          if (createToken(oTokens, TOKEN_BG, NULL) == FALSE)
            return LEX_NOMEM;

          eState = STATE_START;
        }
        else if (c == '>') {
          /* Create a REDOUT token. */
          if (createToken(oTokens, TOKEN_REDOUT, NULL) == FALSE)
            return LEX_NOMEM;

          eState = STATE_START;
        } else if (c == '<') {
          /* Create a PIPE token. */
          if (createToken(oTokens, TOKEN_REDIN, NULL) == FALSE)
            return LEX_NOMEM;

          eState = STATE_START;
        } else if (c == '\"') {
          eState = STATE_IN_DQUOTE;
        }

        else if (c == '\'') {
          eState = STATE_IN_QUOTE;
        }

        else {
          acValue[iValueIndex++] = c;
          eState = STATE_IN_WORD;
        }
        break;

      case STATE_IN_WORD:
        if ((c == '\n') || (c == '\0')) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          return LEX_SUCCESS;
        } else if (isspace(c)) {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          eState = STATE_START;
        } else if (c == '|') {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          /* Create a PIPE token. */
          if (createToken(oTokens, TOKEN_PIPE, NULL) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          eState = STATE_START;
        } else if (c == '>') {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          /* Create a REDOUT token. */

          if (createToken(oTokens, TOKEN_REDOUT, NULL) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          eState = STATE_START;
        } else if (c == '<') {
          /* Create a WORD token. */
          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;

          /* Create a REDIN token. */
          if (createToken(oTokens, TOKEN_REDIN, NULL) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;

          eState = STATE_START;
        }
        else if (c == '&') {
          // Create a WORD token

          acValue[iValueIndex] = '\0';
          if (createToken(oTokens, TOKEN_WORD, acValue) == FALSE)
            return LEX_NOMEM;


          // Create a Background command token.
          if (createToken(oTokens, TOKEN_BG, NULL) == FALSE)
            return LEX_NOMEM;

          iValueIndex = 0;
          eState = STATE_START;
        }
        else if (c == '\"') {
          eState = STATE_IN_DQUOTE;
        }
        else if (c == '\'') {
          eState = STATE_IN_QUOTE;
        }
        else {
          acValue[iValueIndex++] = c;
          eState = STATE_IN_WORD;
        }
        break;
      case STATE_IN_DQUOTE:
        if (c == '\"')
          eState = STATE_IN_WORD;
        else if ((c == '\n') || (c == '\0'))
          return LEX_QERROR;
        else
          acValue[iValueIndex++] = c;

        break;

      case STATE_IN_QUOTE:
        if (c == '\'')
          eState = STATE_IN_WORD;
        else if ((c == '\n') || (c == '\0'))
          return LEX_QERROR;
        else
          acValue[iValueIndex++] = c;
        break;
      default:
        assert(FALSE);
    }
  }
}

enum SyntaxResult
syntaxCheck(DynArray_T oTokens) {
  int i;
  enum SyntaxResult ret = SYN_SUCCESS;
  int riexist = FALSE, roexist = FALSE, pexist = FALSE;
  struct Token *t, *t1;

  assert(oTokens);

  for (i = 0; i < DynArray_getLength(oTokens); i++) {
    t = DynArray_get(oTokens, i);
    if (i == 0) {
      if (t->eType != TOKEN_WORD) {
        /* Missing command name */
        ret = SYN_FAIL_NOCMD;
        break;
      }
    } else {
      if (t->eType == TOKEN_PIPE) {
        /* No redout in previous tokens and no consecutive pipe in following tokens */
        if (roexist == TRUE) {
          /* Multiple redirection error */
          ret = SYN_FAIL_MULTREDOUT;
          break;
        } else {
          if (i == DynArray_getLength(oTokens)-1) {
            /* Redirection without destination */
            ret = SYN_FAIL_NOCMD;
            break;
          }
          else {
            t1 = DynArray_get(oTokens, i+1);
            if (t1->eType != TOKEN_WORD) {
              /* Redirection without destination */
              ret = SYN_FAIL_NOCMD;
              break;
            }
          }
          pexist = TRUE;
        }
      }
      else if (t->eType == TOKEN_BG) {
        if (i != DynArray_getLength(oTokens) - 1) {
          ret = SYN_FAIL_INVALIDBG;
          break;
        }
      }
      else if (t->eType == TOKEN_REDIN) {
        /* No pipe in previous tokens and no redin in following tokens */
        if ((pexist == TRUE) || (riexist == TRUE)) {
          /* Multiple redirection error */
          ret = SYN_FAIL_MULTREDIN;
          break;
        } else {
          if (i == DynArray_getLength(oTokens)-1) {
            /* Redirection without destination */
            ret = SYN_FAIL_NODESTIN;
            break;
          }
          else {
            t1 = DynArray_get(oTokens, i+1);
            if (t1->eType != TOKEN_WORD) {
              /* Redirection without destination */
              ret = SYN_FAIL_NODESTIN;
              break;
            }
          }
          riexist = TRUE;
        }
      } else if (t->eType == TOKEN_REDOUT) {
        /* No redout in following tokens */
        if (roexist == TRUE) {
          /* Multiple redirection error */
          ret = SYN_FAIL_MULTREDOUT;
          break;
        } else {
          if (i == DynArray_getLength(oTokens)-1) {
            /* Redirection without destination */
            ret = SYN_FAIL_NODESTOUT;
            break;
          }
          else {
            t1 = DynArray_get(oTokens, i+1);
            if (t1->eType != TOKEN_WORD) {
              /* Redirection without destination */
              ret = SYN_FAIL_NODESTOUT;
              break;
            }
          }
          roexist = TRUE;
        }
      }
    }
  }

  return ret;
}

