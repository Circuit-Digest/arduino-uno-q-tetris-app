// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <string.h>
#include "display.h"
#include "board.h"

static Arduino_LED_Matrix matrix;

// The physical UNO Q LED matrix dimensions
static const int8_t MATRIX_ROWS = 8;
static const int8_t MATRIX_COLS = 13;
static const uint16_t FRAME_SIZE = MATRIX_ROWS * MATRIX_COLS; // 104 bytes

static const uint8_t BRIGHTNESS_LOCKED = 3;
static const uint8_t BRIGHTNESS_ACTIVE = 7;

// Maps board coordinates (x in [0..BOARD_COLS-1], y in [0..BOARD_ROWS-1])
// to the physical 8x13 matrix row-major frame buffer (104 bytes).
static inline void setPixel(uint8_t frame[104], int8_t x, int8_t y, uint8_t brightness) {
    if (x < 0 || x >= BOARD_COLS || y < 0 || y >= BOARD_ROWS) {
        return;
    }

    int8_t r = 0;
    int8_t c = 0;

#if DISPLAY_ORIENTATION == ORIENTATION_PORTRAIT_USB_TOP
    // Holding the board with USB-C connector at the top:
    // Board x (0..7) maps to matrix rows (7..0)
    // Board y (0..12) maps to matrix cols (0..12)
    r = (BOARD_COLS - 1) - x;
    c = y;
#elif DISPLAY_ORIENTATION == ORIENTATION_PORTRAIT_USB_BOTTOM
    // Holding the board with USB-C connector at the bottom:
    r = x;
    c = (BOARD_ROWS - 1) - y;
#elif DISPLAY_ORIENTATION == ORIENTATION_TRANSPOSE_INVERTED
    r = (BOARD_COLS - 1) - x;
    c = (BOARD_ROWS - 1) - y;
#else // ORIENTATION_TRANSPOSE_DIRECT
    r = x;
    c = y;
#endif

    if (r >= 0 && r < MATRIX_ROWS && c >= 0 && c < MATRIX_COLS) {
        frame[r * MATRIX_COLS + c] = brightness;
    }
}

void displayInit() {
    matrix.begin();
    matrix.setGrayscaleBits(3);  // 0-7 brightness levels
    matrix.clear();
}

void displayRenderBoard(const ActivePiece *activePiece) {
    uint8_t frame[FRAME_SIZE];
    memset(frame, 0, sizeof(frame));

    for (int8_t y = 0; y < BOARD_ROWS; y++) {
        for (int8_t x = 0; x < BOARD_COLS; x++) {
            uint8_t v = boardCellValue(x, y);
            if (v) {
                setPixel(frame, x, y, BRIGHTNESS_LOCKED);
            }
        }
    }

    if (activePiece != nullptr) {
        Point cells[4];
        pieceGetAbsoluteCells(*activePiece, cells);
        for (uint8_t i = 0; i < 4; i++) {
            setPixel(frame, cells[i].x, cells[i].y, BRIGHTNESS_ACTIVE);
        }
    }

    matrix.draw(frame);
}

// ---------------------------------------------------------------------
// Startup animation: top-to-bottom wipe across the board.
// ---------------------------------------------------------------------
static unsigned long startupStartTime = 0;
static const unsigned long STARTUP_DURATION_MS = 900;

void displayStartupStart(unsigned long now) {
    startupStartTime = now;
}

bool displayStartupUpdate(unsigned long now) {
    unsigned long elapsed = now - startupStartTime;
    uint32_t litRows = (uint32_t)elapsed * BOARD_ROWS / STARTUP_DURATION_MS;
    if (litRows > (uint32_t)BOARD_ROWS) litRows = BOARD_ROWS;

    uint8_t frame[FRAME_SIZE];
    memset(frame, 0, sizeof(frame));
    for (uint32_t y = 0; y < litRows; y++) {
        for (int8_t x = 0; x < BOARD_COLS; x++) {
            setPixel(frame, x, (int8_t)y, 5);
        }
    }
    matrix.draw(frame);

    return elapsed >= STARTUP_DURATION_MS;
}

// ---------------------------------------------------------------------
// Line-clear flash: flashes only the cleared lines while keeping the
// rest of the board visible.
// ---------------------------------------------------------------------
static unsigned long flashStartTime = 0;
static const unsigned long FLASH_DURATION_MS = 420;
static const unsigned long FLASH_PERIOD_MS = 105;  // on/off half-period
static bool lineClearRows[BOARD_ROWS];

void displayLineClearStart(unsigned long now, const bool fullRows[BOARD_ROWS]) {
    flashStartTime = now;
    if (fullRows != nullptr) {
        memcpy(lineClearRows, fullRows, sizeof(lineClearRows));
    } else {
        memset(lineClearRows, 0, sizeof(lineClearRows));
    }
}

bool displayLineClearUpdate(unsigned long now) {
    unsigned long elapsed = now - flashStartTime;
    bool flashBright = ((elapsed / FLASH_PERIOD_MS) % 2) == 0;

    uint8_t frame[FRAME_SIZE];
    memset(frame, 0, sizeof(frame));

    for (int8_t y = 0; y < BOARD_ROWS; y++) {
        bool isClearing = lineClearRows[y];
        for (int8_t x = 0; x < BOARD_COLS; x++) {
            if (isClearing) {
                if (flashBright) {
                    setPixel(frame, x, y, 7);
                }
            } else {
                uint8_t v = boardCellValue(x, y);
                if (v) {
                    setPixel(frame, x, y, BRIGHTNESS_LOCKED);
                }
            }
        }
    }

    matrix.draw(frame);
    return elapsed >= FLASH_DURATION_MS;
}

// ---------------------------------------------------------------------
// Game-over animation: fill the board top-to-bottom, then hold.
// ---------------------------------------------------------------------
static unsigned long gameOverStartTime = 0;
static const unsigned long GAME_OVER_DURATION_MS = 700;

void displayGameOverStart(unsigned long now) {
    gameOverStartTime = now;
}

bool displayGameOverUpdate(unsigned long now) {
    unsigned long elapsed = now - gameOverStartTime;
    uint32_t litRows = (uint32_t)elapsed * BOARD_ROWS / GAME_OVER_DURATION_MS;
    if (litRows > (uint32_t)BOARD_ROWS) litRows = BOARD_ROWS;

    uint8_t frame[FRAME_SIZE];
    memset(frame, 0, sizeof(frame));
    for (uint32_t y = 0; y < litRows; y++) {
        for (int8_t x = 0; x < BOARD_COLS; x++) {
            setPixel(frame, x, (int8_t)y, 2);
        }
    }
    matrix.draw(frame);

    return elapsed >= GAME_OVER_DURATION_MS;
}
