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

#ifndef TESTS_H

#define TESTS_H

#include "assert.h"

#define USE_ASSERTS

#ifdef USE_ASSERTS
#define ASSERT(expr, msg) if (!(expr)) printf("\e[1;31mAssert failed at %s %d: %s\e[0m\n", __FILE__, __LINE__, msg);
#else
#define ASSERT(expr, msg);
#endif
#endif