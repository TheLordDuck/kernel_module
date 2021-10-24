/* Example pending to complete
*/

/*
 * program to monitor write syscall interactions
 */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/uaccess.h>	/* copy_from_user */
#include <linux/proc_fs.h>
#include <linux/sched.h>	/* find_task_by_pid_ns */
#include <linux/seq_file.h>

MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION ("/proc write");

/* /proc interface to the module */

#define PROC_ENTRY "write"	/* File name */


/* Information is written using seq_printf function */
static int
read_proc (struct seq_file *m, void *p)
{
  printk(KERN_INFO "READ function called\n");
  return 0;
}

/* Write: captures the pid of the process */
ssize_t
write_proc (struct file * f, const char __user * buff, size_t len, loff_t * o)
{
  return printk (KERN_INFO "WRITE function called, buffer %s\n", buff);
  /*return len;
  */
}

static int
ex2_open (struct inode *inode, struct file *file)
{
  return single_open (file, read_proc, NULL);
}

static struct file_operations proc_operations = {
  .owner = THIS_MODULE,
  .open = ex2_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .write = write_proc,
  .release = single_release
};

static int __init
procdemo_init (void)
{
  /* Register /proc entry for our module */
  if (!proc_create (PROC_ENTRY, S_IFREG, NULL, &proc_operations))
      return (-1);

  printk (KERN_INFO "tracewrite: Correctly installed\n Compiled at %s %s\n",
	  __DATE__, __TIME__);
  return (0);
}

static void __exit
procdemo_cleanup (void)
{
  remove_proc_entry (PROC_ENTRY, NULL); /* Unregister /proc entry for our module */
  printk (KERN_INFO "EXEMPLE: Cleanup successful\n");
}

module_init (procdemo_init);
module_exit (procdemo_cleanup);
