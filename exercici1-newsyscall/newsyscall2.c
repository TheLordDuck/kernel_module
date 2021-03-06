/*
 * New system call
 */

#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/thread_info.h>
#include <asm/asm-offsets.h>   /* NR_syscalls */

#include <linux/pid.h>
#include <linux/sched.h>

#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>

#include "newsyscall2.h"

MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION ("New system call");

unsigned int free_ident = -1;

extern unsigned sys_call_table[];

asmlinkage long
sys_newsyscall (int pid, int type)
{
  //tractament dels casos trivials

  // type = 0 --> calcular NUM_CHILDREN
  // type = 1 --> calcular NUM_SIBLINGS
  // type != 0 o 1 --> retornar error, tipus d'operació no correcte
  // si el tipus de operació no es correcte, codi error -EINVAL
  if((type != NUM_CHILDREN) || (type != NUM_SIBLINGS)){
    return -EINVAL;
  }

  //si el pid no esta definit
  if(!pid){
    return -ESRCH;
  }

  //busco si existeix el pid que s'ha pasat com a parametre
  struct task_struct *task = NULL;
  task = pid_task(find_vpid(pid), PIDTYPE_PID);

  // si el process que s'especifica no existeis, codi error -ESRCH
  if(!task){
    return -ESRCH;
  }

  // contadors per a guardar els valors de children i siblings
  // i que quan un process no tingui ni childrens ni sibligns que retorni 0 
  // com a cas trivial que demanes, per aixo els inicio a 0
  int numero_childrens = 0;
  int numero_siblings = 0;

  // crear structura de llista per a iterar sobre els posibles children i siblings de la task
  struct list_head *list;

  if(type == NUM_CHILDREN){
    //si type = 0; calcular NUM_CHILDREN
    list_for_each(list, &(task->children)){
      numero_childrens++;
    }
    return numero_childrens;
  } else if (type == NUM_SIBLINGS){
    //si type = 1; calcular NUM_SIBLINGS
    list_for_each(list, &(task->siblings)){
      numero_siblings++;
    }
    return numero_siblings;
  }
}

#define GPF_DISABLE write_cr0(read_cr0() & (~ 0x10000)) /* Disable RO protection */
#define GPF_ENABLE write_cr0(read_cr0() | 0x10000) /* Enable RO protection */

static int __init
newsyscall_init (void)
{
  unsigned int i;

  /* Look for an unused sys_call_table entry */
  for (i = 0; i < NR_syscalls; i++)
    if (sys_call_table[i] == (unsigned) sys_ni_syscall)
      break;

  /* Found? */
  if (i == NR_syscalls)
    {
      printk ("No free entry available");
      return (-1);
    }

  /* Use this sys_call_entry */
  free_ident = i;

  GPF_DISABLE; /* Disable RO protection (sys_call_table is on a RO page )*/
  sys_call_table[free_ident] = (unsigned) sys_newsyscall;
  GPF_ENABLE; /* Enable RO protection */

  printk (KERN_INFO "New syscall installed. Identifier = %d\n", free_ident);
  return (0);
}

static void __exit
newsyscall_exit (void)
{
  /* Restore previous state */
  if (free_ident != -1) {
    GPF_DISABLE; /* Disable RO protection (sys_call_table is on a RO page )*/
    sys_call_table[free_ident] = (unsigned) sys_ni_syscall;
    GPF_ENABLE; /* Enable RO protection */

    free_ident = -1;
    printk (KERN_INFO "New syscall removed\n");
  }
  else 
    printk (KERN_INFO "Unexpected error\n");
}

module_init (newsyscall_init);
module_exit (newsyscall_exit);
