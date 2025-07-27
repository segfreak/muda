#!/bin/sh

OUT_FILE=$1

COMMIT=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")
BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
DIRTY=$(test -n "$(git status --porcelain 2>/dev/null)" && echo "1" || echo "0")
BUILD_TIME=$(date "+%Y-%m-%d %H:%M:%S")

# Read version from VERSION file
VERSION_FILE="VERSION"
if [ -f "$VERSION_FILE" ]; then
    VERSION=$(cat "$VERSION_FILE")
    VERSION_MAJOR=$(echo "$VERSION" | cut -d. -f1)
    VERSION_MINOR=$(echo "$VERSION" | cut -d. -f2)
    VERSION_PATCH=$(echo "$VERSION" | cut -d. -f3)
else
    VERSION="0.0.0"
    VERSION_MAJOR=0
    VERSION_MINOR=0
    VERSION_PATCH=0
fi

mkdir -p "$(dirname "$OUT_FILE")"

cat <<EOF > "$OUT_FILE"
// This file is auto-generated. Do not edit manually.
#ifndef MUDA_VER_H
#define MUDA_VER_H

#define MUDA_VERSION        "$VERSION"
#define MUDA_VERSION_MAJOR   $VERSION_MAJOR
#define MUDA_VERSION_MINOR   $VERSION_MINOR
#define MUDA_VERSION_PATCH   $VERSION_PATCH

#endif // MUDA_VER_H
EOF

if [ -f "Doxyfile" ]; then
  sed -i "s/^PROJECT_NUMBER *=.*/PROJECT_NUMBER         = \"$VERSION\"/" Doxyfile
fi
