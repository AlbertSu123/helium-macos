#!/bin/bash
set -euo pipefail
repo="$(cd "$(dirname "$0")/.." && pwd)"
test_binary="$(mktemp -t helium-workspace-scroll)"
trap 'rm -f "$test_binary"' EXIT
sdk="$(xcrun --sdk macosx --show-sdk-path)"
xcrun clang++ -std=c++20 -isysroot "$sdk" -I "$repo/build/src" \
  "$repo/tests/workspace_scroll_test.cc" -o "$test_binary"
"$test_binary"
echo 'Workspace scroll tests passed.'
