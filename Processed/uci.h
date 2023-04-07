/*
Carlsim 2.0 Chess Engine
Copyright (C) 2023 Sam Peterson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
void uci_perfTest(void);

#endif
