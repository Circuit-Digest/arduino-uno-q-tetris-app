// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// Top-level game state machine: startup animation -> playing -> (line-clear
// flash <-> playing) -> game-over animation -> wait for restart. Owns
// scoring and the gravity/drop-speed ramp, and ties the joystick, board,
// piece and display modules together. Everything is timed off millis()
// passed in from sketch.ino's loop(), so nothing here blocks.

#pragma once

void gameInit();
void gameUpdate(unsigned long now);
