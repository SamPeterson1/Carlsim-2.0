#ifndef UCI_H

#define UCI_H

#include "Board.h"
#include "StrUtil.h"
#include "Fen.h"
#include "Movegen.h"
#include "Move.h"
#include "Perft.h"
#include <stdlib.h>

int uci_exec(char *cmd);

#endif