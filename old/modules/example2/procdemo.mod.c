#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfa54fba2, "module_layout" },
	{ 0xa1b59294, "single_release" },
	{ 0x64b23d29, "seq_read" },
	{ 0xff8ee860, "seq_lseek" },
	{ 0xaf718e90, "remove_proc_entry" },
	{ 0x46c41011, "proc_create" },
	{ 0xe445e0e7, "printk" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x813133de, "seq_printf" },
	{ 0xd604c05, "pid_task" },
	{ 0xddf0ab64, "find_vpid" },
	{ 0xab31c3cf, "single_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "44CBD34EE570F9A43B03C70");
