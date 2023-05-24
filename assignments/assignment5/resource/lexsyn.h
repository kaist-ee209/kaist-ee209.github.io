#ifndef _LEXSYN_H_
#define _LEXSYN_H_

#include "dynarray.h"

enum {MAX_LINE_SIZE = 1024};
enum {MAX_ARGS_CNT = 64};

enum LexResult {LEX_SUCCESS, LEX_QERROR, LEX_NOMEM, LEX_LONG};
enum AliasResult {ALIAS_SUCCESS, ALIAS_LONG, ALIAS_QERROR};
enum SyntaxResult {
  SYN_SUCCESS,
  SYN_FAIL_NOCMD,
  SYN_FAIL_MULTREDIN, 
  SYN_FAIL_NODESTIN,
  SYN_FAIL_MULTREDOUT,
  SYN_FAIL_NODESTOUT,
  SYN_FAIL_INVALIDBG,
};

void command_lexLine(const char * pcLine, DynArray_T ctokens);
enum AliasResult alias_lexLine(const char *pcLine, DynArray_T oTokens);
enum LexResult lexLine_quote(const char *pcLine, DynArray_T oTokens);
enum LexResult lexLine(const char *pcLine, DynArray_T oTokens);
enum SyntaxResult syntaxCheck(DynArray_T oTokens);

#endif /* _LEXSYN_H_ */
