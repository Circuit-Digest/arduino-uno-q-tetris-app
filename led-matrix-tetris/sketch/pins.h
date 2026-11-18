// SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
//
// SPDX-License-Identifier: MPL-2.0
//
// Wiring (standard 2-axis analog joystick module):
//   VRx -> A0   (left/right axis, analog)
//   VRy -> A1   (up/down axis, analog)
//   SW  -> D2   (push button, digital, wired to GND when pressed)
//   VCC -> 5V or 3.3V, GND -> GND
//
// If left/right or up/down feel reversed for your physical wiring, swap the
// A0/A1 assignments below or flip the sign checks in joystick.cpp.

#pragma once

static const uint8_t VRX_PIN = A0;
static const uint8_t VRY_PIN = A1;
static const uint8_t SW_PIN  = 2;
