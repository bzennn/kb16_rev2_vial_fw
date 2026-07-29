#!/usr/bin/env bash
# Build DOIO KB16 Rev2 Vial firmware using local vial-qmk tree.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
VIAL_QMK="${VIAL_QMK:-/Users/bzennn/Workspace/embedded/vial-qmk}"
QMK_TOOLCHAIN="${QMK_TOOLCHAIN:-$HOME/Library/Application Support/qmk/bin}"

if [[ ! -d "$VIAL_QMK" ]]; then
  echo "vial-qmk not found at: $VIAL_QMK" >&2
  echo "Clone it first: git clone --recurse-submodules https://github.com/vial-kb/vial-qmk.git \"$VIAL_QMK\"" >&2
  exit 1
fi

export PATH="$PATH:$QMK_TOOLCHAIN"
export QMK_HOME="$VIAL_QMK"

KEYMAP_LINK="$VIAL_QMK/keyboards/doio/kb16/rev2/keymaps/vial"
PROJECT_KEYMAP="$ROOT/kb16/rev2/keymaps/vial"

if [[ ! -e "$KEYMAP_LINK" ]]; then
  ln -s "$PROJECT_KEYMAP" "$KEYMAP_LINK"
elif [[ -L "$KEYMAP_LINK" ]]; then
  :
elif [[ ! -e "${KEYMAP_LINK}.upstream" ]]; then
  mv "$KEYMAP_LINK" "${KEYMAP_LINK}.upstream"
  ln -s "$PROJECT_KEYMAP" "$KEYMAP_LINK"
fi

cd "$VIAL_QMK"
qmk compile -kb doio/kb16/rev2 -km vial "$@"

BIN="$VIAL_QMK/doio_kb16_rev2_vial.bin"
if [[ -f "$BIN" ]]; then
  cp "$BIN" "$ROOT/doio_kb16_rev2_vial.bin"
  echo "Firmware: $ROOT/doio_kb16_rev2_vial.bin"
fi
