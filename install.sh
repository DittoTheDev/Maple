#!/bin/bash
# Maple Install Script 🚀
# This script downloads precompiled binaries and installs them into /usr/bin.
# It does NOT compile from source.

VERSION="1.1"

# Update the URLs below with the actual link to your release binaries on GitHub.
MAPLE_BINARY_URL="https://github.com/ItsCodey1095/Maple/releases/download/v${VERSION}/maple"
MVM_BINARY_URL="https://github.com/ItsCodey1095/Maple/releases/download/v${VERSION}/mvm"

DEST_DIR="/usr/bin"
TMP_DIR="/tmp/maple_install_$$"

mkdir -p "$TMP_DIR"

echo "Downloading Maple (compiler for .maple files) 😎..."
curl -L -o "$TMP_DIR/maple" "$MAPLE_BINARY_URL" || { echo "Failed to download Maple binary."; exit 1; }
chmod +x "$TMP_DIR/maple"

echo "Downloading MVM (bytecode virtual machine) 🚀..."
curl -L -o "$TMP_DIR/mvm" "$MVM_BINARY_URL" || { echo "Failed to download MVM binary."; exit 1; }
chmod +x "$TMP_DIR/mvm"

echo "Installing binaries into ${DEST_DIR}..."
sudo cp "$TMP_DIR/maple" "${DEST_DIR}/maple" || { echo "Failed to install Maple."; exit 1; }
sudo cp "$TMP_DIR/mvm" "${DEST_DIR}/mvm" || { echo "Failed to install MVM."; exit 1; }

echo "Cleaning up temporary files..."
rm -rf "$TMP_DIR"

echo "Installation complete! 🎉"
echo "You can now run the Maple compiler with 'maple' and execute bytecode with 'mvm'."
