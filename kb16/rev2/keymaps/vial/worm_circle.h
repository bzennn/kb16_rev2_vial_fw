/* Copyright 2026
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"

typedef struct __attribute__((packed)) {
    uint8_t enable;
    uint8_t center_h;
    uint8_t center_s;
    uint8_t color_a_h;
    uint8_t color_a_s;
    uint8_t color_b_h;
    uint8_t color_b_s;
} worm_circle_config_t;

enum worm_circle_via_values {
    id_worm_enable    = 1,
    id_worm_center_h  = 2,
    id_worm_center_s  = 3,
    id_worm_color_a_h = 4,
    id_worm_color_a_s = 5,
    id_worm_color_b_h = 6,
    id_worm_color_b_s = 7,
};

/* Vial User-tab keycodes (must match customKeycodes order in vial.json). */
enum worm_circle_keycodes {
    WC_A = QK_KB_0, /* edit Contour A with RGB_HUI/SAI (also Vial Color) */
    WC_B,           /* edit Contour B with RGB_HUI/HUD/SAI/SAD */
    WC_CEN,         /* edit Center with RGB_HUI/HUD/SAI/SAD */
};

extern worm_circle_config_t g_worm_circle;

void worm_circle_init(void);
void worm_circle_save(void);
void worm_circle_apply_mode(void);
void worm_circle_sync_color_a_from_rgb(void);
void worm_circle_set_value(uint8_t *data);
void worm_circle_get_value(uint8_t *data);
bool worm_circle_process_record(uint16_t keycode, keyrecord_t *record);
