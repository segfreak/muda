#!/bin/sh

OUT_FILE=$1

COMMIT=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")
BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
DIRTY=$(test -n "$(git status --porcelain 2>/dev/null)" && echo "1" || echo "0")
BUILD_TIME=$(date "+%Y-%m-%d %H:%M:%S")

mkdir -p "$(dirname "$OUT_FILE")"

cat <<EOF > "$OUT_FILE"
// This file is auto-generated. Do not edit manually.
#ifndef MUDA_GIT_H
#define MUDA_GIT_H

#define MUDA_GIT_COMMIT "$COMMIT"
#define MUDA_GIT_BRANCH "$BRANCH"
#define MUDA_GIT_DIRTY   $DIRTY
#define MUDA_BUILD_TIME "$BUILD_TIME"

#endif // MUDA_GIT_H
EOF
