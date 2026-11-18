// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// Tetromino definitions: the 7 standard pieces (I, O, T, S, Z, J, L), each
// described as 4 cells inside a square bounding box. Rotated states are
// computed on the fly (see tetromino.cpp) instead of being hand-tabulated,
// so there is a single source of truth per piece.

#pragma once
#include <stdint.h>

enum PieceType : uint8_t {
    PIECE_I = 0,
    PIECE_O,
    PIECE_T,
    PIECE_S,
    PIECE_Z,
    PIECE_J,
    PIECE_L,
    PIECE_COUNT
};

struct Point {
    int8_t x;
    int8_t y;
};

// Side length of the square bounding box a piece rotates within (2, 3 or 4).
uint8_t tetrominoBoxSize(PieceType type);

// Writes the 4 occupied cells of `type` at rotation state `rotation`
// (0-3, 90 degrees clockwise each step) into outCells, in box-local
// coordinates (0..boxSize-1).
void tetrominoGetCells(PieceType type, uint8_t rotation, Point outCells[4]);
