#!/bin/sh

# apt-get update -qq && apt-get install -y build-essential git cmake wget unzip python3 python3-pip
ubuntu_install() {
  sudo apt-get update -qq && \
      sudo apt-get install -y $UBUNTU_PKGS
}

# prompt <question> [exit]
prompt() {
  read -p "$1 [Y/n] "
  if [[ ! $REPLY =~ (^[Yy]$|^$) ]]; then
    if [[ $# -gt 1 && "$2" = "exit" ]]; then
      echo exiting
      exit
    fi
    return 0
  else
    return 1
  fi
}

# ensure_have_archive <url> <dirname> <description>
ensure_have_archive() {
  if [[ -d "$2" ]]; then
    echo "found $3"
    return
  fi

  prompt "$3 not found, download?" exit

  echo "Downloading $3"
  ARCHIVE=$(basename "$1")
  wget -q --show-progress "$1"

  echo -n "extracting..."
  case $1 in
    *.tar.bz)
        tar -xjf "$ARCHIVE" #-C "$2"
        ;;
    *.zip)
        unzip -q "$ARCHIVE" #-d "$2"
        ;;
  esac

  echo " done"
  rm "$ARCHIVE"
}

UBUNTU_PKGS="build-essential git cmake wget unzip python3 python3-pip virtualenv"

if [[ $(lsb_release -is) = "Ubuntu" ]]; then
  echo "Ubuntu needs these packages: $UBUNTU_PKGS"
  prompt "Do you want to install them?" && \
      ubuntu_install
fi

GCC_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz"
GCC_DIR="gcc-arm-none-eabi-9-2020-q2-update"
SDK_URL="https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v15.x.x/nRF5_SDK_15.3.0_59ac345.zip"
SDK_DIR=$(basename "$SDK_URL" .zip)

ensure_have_archive "$GCC_URL" "$GCC_DIR" "gcc-arm-none-eabi"
ensure_have_archive "$SDK_URL" "$SDK_DIR" "nRF5 SDK"

# create and activate virtualenv
[[ ! -d "infinitime-venv" ]] && virtualenv -p python3 infinitime-venv
source infinitime-venv/bin/activate

# install stuff
pip install adafruit-nrfutil

[[ ! -d "mcuboot" ]] && \
    git clone https://github.com/JuulLabs-OSS/mcuboot.git && \
    pip install -r mcuboot/scripts/requirements.txt

prompt "Setup done. Build using cmake? " exit

# build infinitime
mkdir -p build && \
    cd build

cmake -DARM_NONE_EABI_TOOLCHAIN_PATH="$PWD/../$GCC_DIR" -DNRF5_SDK_PATH="$PWD/../$SDK_DIR" -DUSE_OPENOCD=1 ../
make -j$(nproc)

cd ..

# post-build
sh tools/post_build.sh

# deactivate virtualenv
deactivate
