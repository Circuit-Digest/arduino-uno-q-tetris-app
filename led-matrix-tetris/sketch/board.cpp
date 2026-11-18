// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0

#include "board.h"
#include <string.h>

static uint8_t grid[BOARD_ROWS][BOARD_COLS];

void boardReset() {
    memset(grid, 0, sizeof(grid));
}

bool boardCellIsFree(int8_t x, int8_t y) {
    if (x < 0 || x >= BOARD_COLS || y < 0 || y >= BOARD_ROWS) {
        return false;
    }
    return grid[y][x] == 0;
}

uint8_t boardCellValue(int8_t x, int8_t y) {
    if (x < 0 || x >= BOARD_COLS || y < 0 || y >= BOARD_ROWS) {
        return 0;
    }
    return grid[y][x];
}

void boardLockCells(const Point cells[4], int8_t originX, int8_t originY, uint8_t brightness) {
    for (uint8_t i = 0; i < 4; i++) {
        int8_t x = originX + cells[i].x;
        int8_t y = originY + cells[i].y;
        if (x >= 0 && x < BOARD_COLS && y >= 0 && y < BOARD_ROWS) {
            grid[y][x] = brightness;
        }
    }
}

uint8_t boardFindFullLines(bool outFullRows[BOARD_ROWS]) {
    uint8_t count = 0;
    for (int8_t y = 0; y < BOARD_ROWS; y++) {
        bool full = true;
        for (int8_t x = 0; x < BOARD_COLS; x++) {
            if (grid[y][x] == 0) {
                full = false;
                break;
            }
        }
        outFullRows[y] = full;
        if (full) count++;
    }
    return count;
}

void boardRemoveLines(const bool fullRows[BOARD_ROWS]) {
    int8_t writeRow = BOARD_ROWS - 1;
    for (int8_t readRow = BOARD_ROWS - 1; readRow >= 0; readRow--) {
        if (fullRows[readRow]) {
            continue;  // drop this row
        }
        if (writeRow != readRow) {
            memcpy(grid[writeRow], grid[readRow], sizeof(grid[0]));
        }
        writeRow--;
    }
    // Any remaining rows above the compacted content become empty.
    for (int8_t y = writeRow; y >= 0; y--) {
        memset(grid[y], 0, sizeof(grid[0]));
    }
}
