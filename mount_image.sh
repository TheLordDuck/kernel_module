#!/bin/sh

usage () {
	echo "Options:"
	echo "-i		Hard Disk image (default image.img)"
	echo "-d		Directory (image)"
	echo "-o		Offset to the first partition (default 0)"
	echo "-u		Umount the image"
}

# Default values
IMAGE="image.img"
DIRECTORY="image"
OFFSET=0

while [ $# -gt 0 ]; do
	case $1 in
		-h)
			usage
			exit 1 ;;
		-i)	
			shift 1
			IMAGE=$1
			shift ;;
		-d)
			shift 1
			DIRECTORY=$1
			shift ;;
		-o)
			shift 1
			OFFSET=$1
			shift ;;
		-u)
			CMD="umount"
			shift ;;
		*)
			echo "Unknown option $1"
			exit 1
			;;
	esac
done

if [ -z $CMD ] && [ ! -s $IMAGE ]; then
	echo "$IMAGE is not a valid file:"
	usage
	echo ""
	exit 1
fi

if [ ! -d $DIRECTORY ]; then
	echo "$DIRECTORY is not a valid directory"
	usage
	echo ""
	exit 1
fi

# Mount the device
if [ -z $CMD ]; then
	sudo mount $IMAGE $DIRECTORY -o loop,offset=$OFFSET
else
	sudo $CMD $DIRECTORY
fi
