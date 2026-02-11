#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
XNU_DIR="$SCRIPT_DIR/xnu-arm"
PATCH_DIR="$SCRIPT_DIR/patches/xnu-arm-armpba8"
SERIES_FILE="$PATCH_DIR/series"
PATCHER="$SCRIPT_DIR/xnu-jit-patcher.rb"

if [ ! -d "$XNU_DIR" ]; then
  echo "error: missing xnu source dir: $XNU_DIR" >&2
  exit 1
fi

if [ ! -f "$SERIES_FILE" ]; then
  echo "error: missing patch series file: $SERIES_FILE" >&2
  exit 1
fi

if [ ! -x "$PATCHER" ]; then
  echo "error: missing patcher script: $PATCHER" >&2
  exit 1
fi

"$PATCHER" --xnu-dir "$XNU_DIR" --patch-dir "$PATCH_DIR" --series "$(basename -- "$SERIES_FILE")"

cd "$XNU_DIR"

: "${MAKEJOBS:=-j4}"
: "${TARGET_CONFIGS:=debug arm genericarm}"
exec make TARGET_CONFIGS="$TARGET_CONFIGS" MAKEJOBS="$MAKEJOBS" NO_DTRACE_SYMS=YES "$@"
