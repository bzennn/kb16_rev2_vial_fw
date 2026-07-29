/* Copyright 2026
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#include "worm_circle.h"

#include "rgb_matrix.h"
#include "via.h"
#include "nvm_via.h"
#include <lib/lib8tion/lib8tion.h>

worm_circle_config_t g_worm_circle;

enum { WORM_EDIT_A = 0, WORM_EDIT_B, WORM_EDIT_CENTER };
static uint8_t worm_edit_target = WORM_EDIT_A;

static void worm_circle_set_defaults(void) {
    g_worm_circle.enable    = 0;
    g_worm_circle.center_h  = 0;
    g_worm_circle.center_s  = 0;   /* white center */
    g_worm_circle.color_a_h = 128; /* cyan */
    g_worm_circle.color_a_s = 255;
    g_worm_circle.color_b_h = 170; /* blue */
    g_worm_circle.color_b_s = 255;
}

void worm_circle_save(void) {
    worm_circle_sync_color_a_from_rgb();
    nvm_via_update_custom_config(&g_worm_circle, 0, sizeof(g_worm_circle));
}

void worm_circle_sync_color_a_from_rgb(void) {
    g_worm_circle.color_a_h = rgb_matrix_get_hue();
    g_worm_circle.color_a_s = rgb_matrix_get_sat();
}

void worm_circle_apply_mode(void) {
    if (g_worm_circle.enable) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_WORM_CIRCLE);
    }
}

void worm_circle_init(void) {
    nvm_via_read_custom_config(&g_worm_circle, 0, sizeof(g_worm_circle));
    if (g_worm_circle.color_a_s == 0xFF && g_worm_circle.color_b_s == 0xFF && g_worm_circle.center_s == 0xFF) {
        worm_circle_set_defaults();
        worm_circle_save();
    }
}

void worm_circle_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_worm_enable:
            g_worm_circle.enable = *value_data ? 1 : 0;
            if (g_worm_circle.enable) {
                rgb_matrix_mode(RGB_MATRIX_CUSTOM_WORM_CIRCLE);
            } else if (rgb_matrix_get_mode() == RGB_MATRIX_CUSTOM_WORM_CIRCLE) {
                rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
            }
            break;
        case id_worm_center_h:
            g_worm_circle.center_h = *value_data;
            break;
        case id_worm_center_s:
            g_worm_circle.center_s = *value_data;
            break;
        case id_worm_color_a_h:
            g_worm_circle.color_a_h = *value_data;
            rgb_matrix_sethsv_noeeprom(*value_data, g_worm_circle.color_a_s, rgb_matrix_get_val());
            break;
        case id_worm_color_a_s:
            g_worm_circle.color_a_s = *value_data;
            rgb_matrix_sethsv_noeeprom(g_worm_circle.color_a_h, *value_data, rgb_matrix_get_val());
            break;
        case id_worm_color_b_h:
            g_worm_circle.color_b_h = *value_data;
            break;
        case id_worm_color_b_s:
            g_worm_circle.color_b_s = *value_data;
            break;
    }
}

void worm_circle_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_worm_enable:
            *value_data = (rgb_matrix_get_mode() == RGB_MATRIX_CUSTOM_WORM_CIRCLE) || g_worm_circle.enable;
            break;
        case id_worm_center_h:
            *value_data = g_worm_circle.center_h;
            break;
        case id_worm_center_s:
            *value_data = g_worm_circle.center_s;
            break;
        case id_worm_color_a_h:
            *value_data = rgb_matrix_get_hue();
            break;
        case id_worm_color_a_s:
            *value_data = rgb_matrix_get_sat();
            break;
        case id_worm_color_b_h:
            *value_data = g_worm_circle.color_b_h;
            break;
        case id_worm_color_b_s:
            *value_data = g_worm_circle.color_b_s;
            break;
    }
}

bool worm_circle_process_record(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case WC_A:
            worm_edit_target = WORM_EDIT_A;
            return false;
        case WC_B:
            worm_edit_target = WORM_EDIT_B;
            return false;
        case WC_CEN:
            worm_edit_target = WORM_EDIT_CENTER;
            return false;
    }

    if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_WORM_CIRCLE) {
        return true;
    }

    /* Contour A is driven by Vial Color / default RGB_* handlers. */
    if (worm_edit_target == WORM_EDIT_A) {
        return true;
    }

    uint8_t *h = (worm_edit_target == WORM_EDIT_B) ? &g_worm_circle.color_b_h : &g_worm_circle.center_h;
    uint8_t *s = (worm_edit_target == WORM_EDIT_B) ? &g_worm_circle.color_b_s : &g_worm_circle.center_s;

    switch (keycode) {
        case RGB_HUI:
            *h += RGB_MATRIX_HUE_STEP;
            worm_circle_save();
            return false;
        case RGB_HUD:
            *h -= RGB_MATRIX_HUE_STEP;
            worm_circle_save();
            return false;
        case RGB_SAI:
            *s = qadd8(*s, RGB_MATRIX_SAT_STEP);
            worm_circle_save();
            return false;
        case RGB_SAD:
            *s = qsub8(*s, RGB_MATRIX_SAT_STEP);
            worm_circle_save();
            return false;
    }

    return true;
}

void via_init_kb(void) {
    worm_circle_init();
}

void keyboard_post_init_user(void) {
    /* Push stored Contour A into the live RGB HSV used by Vial Color. */
    rgb_matrix_sethsv_noeeprom(g_worm_circle.color_a_h, g_worm_circle.color_a_s, rgb_matrix_get_val());
    worm_circle_apply_mode();
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        switch (*command_id) {
            case id_custom_set_value:
                worm_circle_set_value(value_id_and_data);
                break;
            case id_custom_get_value:
                worm_circle_get_value(value_id_and_data);
                break;
            case id_custom_save:
                worm_circle_save();
                break;
            default:
                *command_id = id_unhandled;
                break;
        }
        return;
    }

    *command_id = id_unhandled;
}
