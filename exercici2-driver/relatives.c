/*
 * Example driver
 *
 */

/*
--------------------------------
Parts a completar:

open --> ok
read --> 
lseek --> ok
ioctl --> 
write --> ok
release --> ok
--------------------------------
*/

#include <linux/kvm_irqfd.h> /* copy_to_user */
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/sched.h>	/* find_task_by_pid_ns */

#include <linux/semaphore.h>

#define DRIVER_MAJOR 231
#define DRIVER_NAME "relatives"

MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION ("Relatives driver");
MODULE_AUTHOR ("Flavio Ferreira");

// Varaible per a controlar els accessos al fitxer
static int Device_Open = 0; //  dispositiu ja obert? utilitzat per a prevenir multiples accesos

// Variable semafor per a cambiar les varaibles critiques
static struct semaphore sem;

int
do_open (struct inode *inode, struct file *file)
{
  // Si variable == 1, recurs ocupat
  if(Device_Open == 1)
    return -EBUSY;

  /* Just O_RDONLY mode */
  if (file->f_flags != O_RDONLY) 
    return -EACCES;

  /* Utilitzo semafor per a cambiar la variable critica Device_Open */
  down(&sem);// V
  // Posar variable a 1 per a que bloqueji altres accessos
  Device_Open++;
  up(&sem);// P

  return 0;
}


//do_read (struct file * file, char *buf, size_t count, loff_t * f_pos)
ssize_t
do_read (struct file * file, char *buf, size_t count, int num)
{
  //
  // pendent a implementar
  //
}

ssize_t
do_write (struct file * file, const char *buf, size_t count, loff_t * f_pos)
{
  // no fa falta implementar
  return 0;
}

/* close system call */
int
do_release (struct inode *inode, struct file *file)
{
  /* Utilitzo semafor per a cambiar la variable critica Device_Open */
  down(&sem);// V
  // Posar variable a 0 un altre cop per a es permeti accedir un altre cop
  Device_Open--;
  up(&sem);// P

  return 0;
}

static long
do_ioctl (struct file *file, u_int request, u_long arg)
{
  //
  // pendent a implementar
  //
}

static loff_t
do_llseek (struct file *file, loff_t offset, int orig)
{
  loff_t ret;

  switch (orig)
    {
    case SEEK_CUR:
      ret = file->f_pos + offset;
      break;
    case SEEK_SET:
      ret = offset;
      break;
    default:
      // si orig es igual a SEEK_END --> llavors retorna -EINVAL, no permes
      ret = -EINVAL;
    }

  if (ret >= 0)
    file->f_pos = ret;
  else
    ret = -EINVAL;

  return ret;
}

struct file_operations relatives_op = {
  open:do_open,
  read:do_read,
  write:do_write,
  release:do_release,            /* close system call */
  unlocked_ioctl:do_ioctl,
  llseek:do_llseek
};

static int __init
relatives_init (void)
{
  int result;

  result = register_chrdev (DRIVER_MAJOR, DRIVER_NAME, &relatives_op);
  if (result < 0)
    {
      printk ("Unable to register device");
      return result;
    }

  printk (KERN_INFO "Correctly installed\n Compiled at %s %s\n", __DATE__,
          __TIME__);
  return (0);
}

static void __exit
relatives_cleanup (void)
{
  unregister_chrdev (DRIVER_MAJOR, DRIVER_NAME);

  printk (KERN_INFO "Cleanup successful\n");
}

module_init (relatives_init);
module_exit (relatives_cleanup);
