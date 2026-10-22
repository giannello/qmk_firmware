// Copyright 2026 Giuseppe Iannello (@giannello)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The NUC123's GPIO input sampling is slower than the platform default.
// Matches the value used by the ducky one2mini/1861st and the old das_keyboard
// ports: 12 clock cycles (~0.167 µs at 72 MHz).
#define GPIO_INPUT_PIN_DELAY (NUC123_HCLK / 6 / 1000000L)
