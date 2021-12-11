#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

int counter = 0;

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_write)(unsigned int fd, const char __user *buf, size_t count);

// fake write system call that calls the original write system call
asmlinkage long new_sys_write(unsigned int fd, const char __user *buf, size_t count)
{   

    return ref_sys_write(fd,buf ,count);
}

// this function returns the pointer of the system call table
static unsigned long **aquire_sys_call_table(void)
{
    unsigned long int offset = PAGE_OFFSET;
    unsigned long **sct;

    while (offset < ULLONG_MAX) {
        sct = (unsigned long **)offset;

        if (sct[__NR_close] == (unsigned long *) sys_close) 
            return sct;

        offset += sizeof(void *);
    }
    return NULL;
}

// This function disable the memory protection so that you can access any memory address
// This is done by modifying the value of the register cr0
static void disable_page_protection(void) 
{
    unsigned long value;
    asm volatile("mov %%cr0, %0" : "=r" (value));

    if(!(value & 0x00010000))
        return;

    asm volatile("mov %0, %%cr0" : : "r" (value & ~0x00010000));
}

// This function enable the memory protection
// This is done by modifying the value of the register cr0
static void enable_page_protection(void) 
{
    unsigned long value;
    asm volatile("mov %%cr0, %0" : "=r" (value));

    if((value & 0x00010000))
        return;

    asm volatile("mov %0, %%cr0" : : "r" (value | 0x00010000));
}

// This function is executed when the module is loaded.
// 1. It gets the pointer of system call table
// 2. It disables page protection
// 3. It modifies the entry of the write system call
// 4. It enables page protection
static int __init interceptor_start(void) 
{
    if(!(sys_call_table = aquire_sys_call_table())) {
             return -1;
    }

    disable_page_protection();

    ref_sys_write = (void *)sys_call_table[__NR_write];
    sys_call_table[__NR_write] = (unsigned long *)new_sys_write;

    enable_page_protection();

    return 0;
}

// This method is executed when the module is removed/unloaded
// 1. It disables page protection
// 2. It restores the entry of the write system call
// 3. It enables page protection
static void __exit interceptor_end(void) 
{
    if(!sys_call_table)
        return;
    disable_page_protection();
    sys_call_table[__NR_write] = (unsigned long *)ref_sys_write;
    enable_page_protection();
}

module_init(interceptor_start);
module_exit(interceptor_end);
