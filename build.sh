#!/usr/bin/env bash

info() {
  echo -e "\033[1;32m[ build.sh ]\033[0m :: $1"
}

question() {
  echo -e "\033[1;33m[ build.sh ]\033[0m :: $1"
}

error() {
  echo -e "\033[1;31m[ build.sh ]\033[0m :: $1"
}

check_cmake() {
  if ! command -v cmake &> /dev/null
  then
    error "CMake could not be found"
    exit 1
  else 
    info "CMake found"
  fi
}

check_ninja() {
  if ! command -v ninja &> /dev/null
  then
    error "Ninja could not be found"
    exit 1
  else 
    info "Ninja found"
  fi
}

info "Looking for CMake..."
check_cmake

question "What build generator would you like to use?"
question "1. Ninja (faster, default)"
question "2. Unix Makefiles (use this if you don't have Ninja installed)"

GENERATOR="Ninja"
CONFIG="Debug"

read -p "Enter your choice: " choice

case $choice in
  1)
    info "Using Ninja"
    GENERATOR="Ninja"
    ;;
  2)
    info "Using Unix Makefiles"
    GENERATOR="Unix Makefiles"
    ;;
  *)
    question "Assuming Ninja..."
    ;;
esac

question "What build configuration would you like to use?"

question "1. Release (default)"
question "2. Debug"

read -p "Enter your choice: " choice

case $choice in
  1)
    info "Using Release"
    CONFIG="Release"
    ;;
  2)
    info "Using Debug"
    CONFIG="Debug"
    ;;
  *)
    question "Assuming Release..."
    ;;
esac

info "Creating build directory..."
mkdir -p build

info "Building the project..."

cd build

cmake -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$CONFIG" ..

if [ "$GENERATOR" = "Ninja" ]
then
  check_ninja
  ninja
else
  make
fi

info "The simulator is now available in ./bin/simulator"