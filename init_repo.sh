#! /bin/bash

# ---------------------   PREPARING LOG LIB   -----------------------

set -e

echo "Repository initialization script"

# Проверяем что мы в git репозитории
if [ ! -d ".git" ]; then
  echo "Error: This is not a git repository"
  exit 1
fi

echo "Loading logger lib submodule..."

git submodule init
git submodule update

# Проверяем что нужный подмодуль загрузился
if [ -d "3dPartyModules/LoggerLib" ]; then
  echo "✓ LoggerLib submodule initialized successfully"
else
  echo "✗ LoggerLib submodule failed to initialize"
  exit 1
fi

echo "LoggerLib submodule ready!"

# ---------------------   PREPARING CMAKE   -----------------------

echo "Preparing cmake..."

mkdir -p bin # no error if it exists
cd bin
cmake ..
cd ..
cmake --build bin --target show_help_msg

