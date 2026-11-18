// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// Reads the analog joystick (VRx/VRy on A0/A1) and the push button (SW on
// D2) using the standard Arduino analogRead()/digitalRead() API, and turns
// the raw, noisy signal into clean, throttled game events:
//   - moveLeft / moveRight: repeat at a fixed rate while held
//   - softDrop: true for as long as the stick is held down
//   - rotate / hardDrop: fire exactly once per push (edge-triggered)

#pragma once
#include <stdint.h>

struct JoystickEvents {
    bool moveLeft;
    bool moveRight;
    bool softDrop;
    bool rotate;
    bool hardDrop;
};

// Configures pins and samples the stick's resting position for calibration.
// Call once from setup(), with the stick untouched.
void joystickInit();

// Samples the joystick/button and returns the debounced/throttled events
// for this instant. Call once per loop() iteration with the current millis().
JoystickEvents joystickPoll(unsigned long now);
