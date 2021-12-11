/*
 * skeldev2.c
 *
 * Simple implementation of a device driver with read() and write()
 * file operations. The module creates the /dev/skeldev2 entry. A
 * write to the device stores data into the driver buffer. A read from
 * the device retrieves this data from the buffer.
 *
 * REQUIREMENTS
 *
 * The Device File System (devfs), Linux Kernel 2.4.x.
 *
 * COMPILE/LOAD/UNLOAD
 *
 * To compile the device:
 *
 *   $ gcc -D MODULE -D __KERNEL__ -c skeldev2.c
 *
 * To load the device:
 *
 *   $ sudo /sbin/insmod skeldev2.o
 *
 * To unload the device:
 *
 *   $ sudo /sbin/rmmod skeldev2
 *
 * AUTHOR
 *
 * Emanuele Altieri
 * ealtieri@hampshire.edu
 * June 2002
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/devfs_fs_kernel.h>
#include <asm/uaccess.h>

/* Check  if the Device File System (experimental) is installed */
#ifndef CONFIG_DEVFS_FS
# error "This module requires the Device File System (devfs)"
#endif

/* MODULE CONFIGURATION */

MODULE_AUTHOR("Emanuele Altieri");
MODULE_DESCRIPTION("Basic Device Driver");
MODULE_LICENSE("GPL");

static devfs_handle_t skel_handle;    /* handle for the device     */
static char* skel_name = "skeldev2";  /* create /dev/skeldev2 entry */

#define SKEL_BUFMAX 30
static unsigned char skel_buffer[SKEL_BUFMAX];

/* File Operations */

static ssize_t skel_read(struct file*, char*, size_t, loff_t *);
static ssize_t skel_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations skel_fops = {
	read : skel_read,
	write: skel_write,
	/* NULL (default) */
};

/* Module Initialization */
static int __init skel_init(void)
{
	/* register the module */
	SET_MODULE_OWNER(&skel_fops);
	skel_handle = devfs_register
		(
		 NULL,                      /* parent dir in /dev (none)     */
		 skel_name,                 /* /dev entry name (skeldev2)    */
		 DEVFS_FL_AUTO_DEVNUM |     /* automatic major/minor numbers */
		 DEVFS_FL_AUTO_OWNER,
		 0, 0,                      /* major/minor (not used)        */
		 S_IFCHR,                   /* character device              */
		 &skel_fops,                /* file ops handlers             */
		 NULL                       /* other                         */
		 );
	if (skel_handle <= 0)
		return(-EBUSY);
	return(0);
} /* skel_init() */

/* Module deconstructor */
static void __exit skel_exit(void)
{
	devfs_unregister(skel_handle);
} /* skel_exit() */

/* Read from device */
static ssize_t 
skel_read(struct file *filp, char *buf, size_t count, loff_t *offp)
{
	if (count > SKEL_BUFMAX)
		count = SKEL_BUFMAX;  /* trim data */
	copy_to_user(buf, skel_buffer, count);
	return(count);
} /* skel_read() */

/* Write to device */
static ssize_t 
skel_write(struct file *filp, const char *buf, size_t count, loff_t *offp)
{
	if (count > SKEL_BUFMAX)
		count = SKEL_BUFMAX;
	copy_from_user(skel_buffer, buf, count);
	return(count);
}

/* Specify init and exit functions */
module_init(skel_init);
module_exit(skel_exit);
