// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// Renders the game onto the onboard 8x13 LED matrix (Arduino_LED_Matrix,
// matrix.draw() with a row-major uint8_t[104] grayscale frame buffer - see
// the "LED Matrix Frame" core example) and plays the non-blocking startup,
// line-clear-flash and game-over effects. Every *Update() function is meant
// to be called once per loop() with the current millis() and returns true
// once its effect has finished, so the joystick keeps being polled and
// nothing here ever calls delay().

#pragma once
#include "piece.h"
#include "board.h"

// Display orientation settings for the 8x13 physical LED matrix:
#define ORIENTATION_PORTRAIT_USB_TOP     0  // Portrait mode, USB connector at top
#define ORIENTATION_PORTRAIT_USB_BOTTOM  1  // Portrait mode, USB connector at bottom
#define ORIENTATION_TRANSPOSE_DIRECT     2  // Direct matrix mapping: row=x, col=y
#define ORIENTATION_TRANSPOSE_INVERTED   3  // Inverted direct mapping

// Select orientation (default: ORIENTATION_PORTRAIT_USB_TOP)
#ifndef DISPLAY_ORIENTATION
#define DISPLAY_ORIENTATION ORIENTATION_PORTRAIT_USB_TOP
#endif

void displayInit();

// Draws the locked board plus the currently falling piece (pass nullptr to
// draw the board alone, e.g. mid line-clear-flash).
void displayRenderBoard(const ActivePiece *activePiece);

void displayStartupStart(unsigned long now);
bool displayStartupUpdate(unsigned long now);

void displayLineClearStart(unsigned long now, const bool fullRows[BOARD_ROWS] = nullptr);
bool displayLineClearUpdate(unsigned long now);

void displayGameOverStart(unsigned long now);
bool displayGameOverUpdate(unsigned long now);
