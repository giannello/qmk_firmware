// Copyright 2026 Giuseppe Iannello (@giannello)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The NUC123's GPIO input sampling is slower than the platform default.
// Matches the value used by the ducky one2mini/1861st and the old das_keyboard
// ports: 12 clock cycles (~0.167 µs at 72 MHz).
#define GPIO_INPUT_PIN_DELAY (NUC123_HCLK / 6 / 1000000L)

#define BACKING_STORE_WRITE_SIZE 4
#define WEAR_LEVELING_BACKING_SIZE     (18UL * 1024UL)
#define WEAR_LEVELING_EFL_FLASH_SIZE   (18UL * 1024UL)
#undef MFS_CFG_MAX_RECORDS
#define MFS_CFG_MAX_RECORDS 16

#undef MFS_CFG_BUFFER_SIZE
#define MFS_CFG_BUFFER_SIZE 16

#undef MFS_CFG_TRANSACTION_MAX
#define MFS_CFG_TRANSACTION_MAX 4
