// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// The currently falling tetromino: its type/rotation/position, plus the
// movement, rotation and collision-checked transforms applied to it.

#pragma once
#include "tetromino.h"

struct ActivePiece {
    PieceType type;
    uint8_t rotation;
    int8_t x;  // board-space column of the piece's bounding box top-left
    int8_t y;  // board-space row of the piece's bounding box top-left
};

// Places a new piece of `type` centered at the top of the board.
void pieceSpawn(ActivePiece &piece, PieceType type);

// Writes the piece's 4 occupied cells in absolute board coordinates.
void pieceGetAbsoluteCells(const ActivePiece &piece, Point outCells[4]);

// True if the piece's current position/rotation overlaps a wall, the floor,
// or a locked block.
bool pieceCollides(const ActivePiece &piece);

// Attempts to shift the piece by (dx, dy). Returns true and applies the
// move if the destination is free; otherwise leaves the piece untouched.
bool pieceMove(ActivePiece &piece, int8_t dx, int8_t dy);

// Attempts to rotate the piece 90 degrees clockwise, trying a couple of
// small horizontal kicks if the straight rotation would collide. Returns
// true if a valid rotated position was found and applied.
bool pieceRotate(ActivePiece &piece);

// True if the piece could move down one more row right now (used by the
// gravity/lock logic to decide when to lock the piece in place).
bool pieceCanDrop(const ActivePiece &piece);
