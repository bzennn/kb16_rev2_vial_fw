VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes
LTO_ENABLE = yes

# Encoder enabled
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = yes

COMMAND_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes
SEND_STRING_ENABLE = yes
TAP_DANCE_ENABLE = yes

QMK_SETTINGS = yes

# Keep USB as HID-only (no virtual Serial/CDC)
VIRTSER_ENABLE = no
CONSOLE_ENABLE = no

# Custom Worm-circle RGB effect
RGB_MATRIX_CUSTOM_USER = yes
SRC += worm_circle.c

# eeprom setting
EEPROM_DRIVER = vendor

