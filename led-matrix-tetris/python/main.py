# SPDX-FileCopyrightText: Copyright (C) Arduino s.r.l. and/or its affiliated companies
#
# SPDX-License-Identifier: MPL-2.0
#
# The entire game runs on the MCU sketch (LED matrix rendering and joystick
# input are both handled there). This side just needs to keep the app alive.

from arduino.app_utils import App

App.run()
