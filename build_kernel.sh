#!/bin/bash

rm -rf builds
rm -rf out
rm -rf images

# export some things
export 
mkdir out
mkdir images
mkdir builds
IMAGE_NAME=FirehouseKernel

current_dir=$(pwd)

# clean source before build
make mrproper && make clean

# compile kernel
make -j12 -C $(pwd) O=$(pwd)/out lineageos_i9300_defconfig
make -j12 -C $(pwd) O=$(pwd)/out

# clean up previous images
cd "$current_dir"/AIK
./cleanup.sh
./unpackimg.sh --nosudo

# back to main dir
cd "$current_dir"

# move generated files to temporary directory
cp "$current_dir"/out/arch/arm/boot/Image "$current_dir"/images/
mv "$current_dir"/images/Image "$current_dir"/images/boot.img-kernel

# cleanup past files and move new ones
rm "$current_dir"/AIK/split_img/boot.img-kernel
mv "$current_dir"/images/boot.img-kernel "$current_dir"/AIK/split_img/boot.img-kernel

# delete images dir
rm -r "$current_dir"/images

# goto AIK dir and repack boot.img as not sudo
cd "$current_dir"/AIK
./repackimg.sh --nosudo

# goto main dir
cd "$current_dir"

# move generated image to builds dir renamed as lito_kernel
mv "$current_dir"/AIK/image-new.img "$current_dir"/builds/"$IMAGE_NAME".img

# clean out dir for new builds
rm -r "$current_dir"/out

echo done! you can find your image at /builds
