// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0

#include "piece.h"
#include "board.h"

void pieceSpawn(ActivePiece &piece, PieceType type) {
    piece.type = type;
    piece.rotation = 0;
    uint8_t size = tetrominoBoxSize(type);
    piece.x = (BOARD_COLS - size) / 2;
    piece.y = 0;
}

void pieceGetAbsoluteCells(const ActivePiece &piece, Point outCells[4]) {
    tetrominoGetCells(piece.type, piece.rotation, outCells);
    for (uint8_t i = 0; i < 4; i++) {
        outCells[i].x += piece.x;
        outCells[i].y += piece.y;
    }
}

static bool cellsAreFree(const Point cells[4]) {
    for (uint8_t i = 0; i < 4; i++) {
        if (!boardCellIsFree(cells[i].x, cells[i].y)) {
            return false;
        }
    }
    return true;
}

bool pieceCollides(const ActivePiece &piece) {
    Point cells[4];
    pieceGetAbsoluteCells(piece, cells);
    return !cellsAreFree(cells);
}

bool pieceMove(ActivePiece &piece, int8_t dx, int8_t dy) {
    ActivePiece attempt = piece;
    attempt.x += dx;
    attempt.y += dy;
    if (pieceCollides(attempt)) {
        return false;
    }
    piece = attempt;
    return true;
}

bool pieceRotate(ActivePiece &piece) {
    ActivePiece attempt = piece;
    attempt.rotation = (attempt.rotation + 1) % 4;

    // Try straight rotation first, then horizontal kicks (wall kicks)
    // and vertical kicks (floor kicks) so pieces rotating next to walls
    // or near the floor don't get rejected.
    struct Kick { int8_t dx; int8_t dy; };
    static const Kick kKicks[] = {
        { 0,  0},
        {-1,  0}, { 1,  0},
        { 0, -1},
        {-1, -1}, { 1, -1},
        {-2,  0}, { 2,  0}
    };
    for (const Kick &kick : kKicks) {
        ActivePiece kicked = attempt;
        kicked.x += kick.dx;
        kicked.y += kick.dy;
        if (!pieceCollides(kicked)) {
            piece = kicked;
            return true;
        }
    }
    return false;
}

bool pieceCanDrop(const ActivePiece &piece) {
    ActivePiece attempt = piece;
    attempt.y += 1;
    return !pieceCollides(attempt);
}
