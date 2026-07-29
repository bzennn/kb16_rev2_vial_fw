/* Worm-circle: NuPhy Halo75 v2 "New"-style contour worm on a 4x4 matrix. */
#define RGB_MATRIX_EFFECT_WORM_CIRCLE
RGB_MATRIX_EFFECT(WORM_CIRCLE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    include "worm_circle.h"

/* Outer contour clockwise starting at top-left:
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */
static const uint8_t worm_contour[] = {0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4};
static const uint8_t worm_center[]  = {5, 6, 9, 10};

#    define WORM_CONTOUR_LEN (sizeof(worm_contour) / sizeof(worm_contour[0]))
#    define WORM_CENTER_LEN (sizeof(worm_center) / sizeof(worm_center[0]))
#    define WORM_HALF (WORM_CONTOUR_LEN / 2)

static HSV worm_hsv(uint8_t h, uint8_t s, uint8_t v) {
    HSV hsv = {.h = h, .s = s, .v = v};
    return hsv;
}

static void worm_set_led_hsv(uint8_t led, HSV hsv) {
    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
}

static bool WORM_CIRCLE(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    /* Brightness from VialRGB / rgb_matrix val; speed drives clockwise travel. */
    uint8_t v      = rgb_matrix_config.hsv.v;
    uint8_t speed  = rgb_matrix_config.speed;
    uint8_t offset = scale16by8(g_rgb_timer, scale8(speed, 8) + 1) % WORM_CONTOUR_LEN;

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    /* Contour A follows Vial Lighting color picker (rgb_matrix HSV). */
    HSV center  = worm_hsv(g_worm_circle.center_h, g_worm_circle.center_s, v);
    HSV color_a = worm_hsv(rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, v);
    HSV color_b = worm_hsv(g_worm_circle.color_b_h, g_worm_circle.color_b_s, v);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    for (uint8_t i = 0; i < WORM_CENTER_LEN; i++) {
        uint8_t led = worm_center[i];
        if (led >= led_min && led < led_max) {
            worm_set_led_hsv(led, center);
        }
    }

    for (uint8_t i = 0; i < WORM_CONTOUR_LEN; i++) {
        uint8_t led = worm_contour[(i + offset) % WORM_CONTOUR_LEN];
        if (led >= led_min && led < led_max) {
            /* First half of the moving strip = A, second half = B (50/50). */
            worm_set_led_hsv(led, (i < WORM_HALF) ? color_a : color_b);
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
