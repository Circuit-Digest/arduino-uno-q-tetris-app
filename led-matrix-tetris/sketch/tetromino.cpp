// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0

#include "tetromino.h"

struct TetrominoDef {
    uint8_t boxSize;
    Point cells[4];  // rotation-0 layout, in box-local coordinates
};

static const TetrominoDef kDefs[PIECE_COUNT] = {
    /* I */ {4, {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
    /* O */ {2, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    /* T */ {3, {{1, 0}, {0, 1}, {1, 1}, {2, 1}}},
    /* S */ {3, {{1, 0}, {2, 0}, {0, 1}, {1, 1}}},
    /* Z */ {3, {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
    /* J */ {3, {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
    /* L */ {3, {{2, 0}, {0, 1}, {1, 1}, {2, 1}}},
};

uint8_t tetrominoBoxSize(PieceType type) {
    return kDefs[type].boxSize;
}

// Rotates a single cell 90 degrees clockwise within an NxN box.
static Point rotateCellCW(Point p, uint8_t boxSize) {
    Point r;
    r.x = boxSize - 1 - p.y;
    r.y = p.x;
    return r;
}

void tetrominoGetCells(PieceType type, uint8_t rotation, Point outCells[4]) {
    const TetrominoDef &def = kDefs[type];
    for (uint8_t i = 0; i < 4; i++) {
        outCells[i] = def.cells[i];
    }
    rotation = rotation % 4;
    for (uint8_t r = 0; r < rotation; r++) {
        for (uint8_t i = 0; i < 4; i++) {
            outCells[i] = rotateCellCW(outCells[i], def.boxSize);
        }
    }
}
