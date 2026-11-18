// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// The playfield grid, transposed to portrait orientation (13 rows x 8 columns).
// Mapped to the UNO Q LED matrix (8 rows x 13 columns physical) via display.cpp.

#pragma once
#include <stdint.h>
#include "tetromino.h"

static const int8_t BOARD_ROWS = 13;
static const int8_t BOARD_COLS = 8;

void boardReset();

// True when (x, y) is within the playfield and unoccupied - used for
// collision checks against both the walls/floor and previously locked cells.
bool boardCellIsFree(int8_t x, int8_t y);

// Value stored in the grid at (x, y): 0 if empty/out of bounds, otherwise
// the brightness (1-7) of the locked block.
uint8_t boardCellValue(int8_t x, int8_t y);

// Permanently writes `cells` (offset by originX/originY) into the grid.
void boardLockCells(const Point cells[4], int8_t originX, int8_t originY, uint8_t brightness);

// Scans the grid and returns how many rows are completely filled, marking
// them in outFullRows[BOARD_ROWS] (true = full). Does not modify the grid,
// so the caller can flash the board before actually removing the lines.
uint8_t boardFindFullLines(bool outFullRows[BOARD_ROWS]);

// Removes the rows flagged in fullRows, shifting everything above them down
// and clearing new empty rows at the top.
void boardRemoveLines(const bool fullRows[BOARD_ROWS]);
