// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// LED Matrix Tetris
//
// A fully self-contained game on the MCU side: the joystick (VRx/VRy/SW)
// and the onboard LED matrix are both driven directly by the
// microcontroller, so no Python/Bridge involvement is needed (see
// python/main.py, which just keeps the app alive). All game modules are
// tied together in game.cpp/game.h; see that file for the state machine.

#include "game.h"

void setup() {
    gameInit();
}

void loop() {
    gameUpdate(millis());
}
