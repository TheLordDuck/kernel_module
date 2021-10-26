# Exercices: Kernel Modules

Multiple exercices to learn kernel module

# Setup environment
## Download image
```bash
wget http://einfmlinux1.uoc.edu/aso/image.img
```

## Download kernel
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.19.206.tar.xz
```

## Decompile the file with the kernel code
```bash
tar -xJf linux-4.19.206.tar.xz
rm -f linux-4.19.206.tar.xz
```

## Install flex & bison
```bash
sudo apt-get install flex bison
```
## Compile the kernel
```bash
cd linux-4.19.206
cp ../kernel.config .config
make -j4
cd ..
```

## Install qemu 
```bash
sudo apt-get install qemu-kvm
```

# Usage
## Boot guest VM
```bash
./boot.sh
```

## Prepare cscope

#### Install cscope
```bash
sudo apt-get install cscope

cd linux-4.19.206
cp ../cscope.files .
cscope 
cd ..
```

## Compile modules
```bash
cd modules/example1
make
cd ../..

cd modules/example2
make
cd ../..


cd modules/example3
make
cd ../..
```

# File transfert

## From guest, copy host files to guest

### Install ssh server (on the host)
```bash
sudo apt-get install openssh-server
```

### Copy from guest
```bash
scp user@10.0.2.2:aso/modules/example1/hw.ko .
```

## Copy files to the Guest machine riding your file system
(You need to stop the Guest machine)

```bash
./mount_image.sh
sudo cp modules/example?/*.ko image/root
./mount_image.sh -u
```