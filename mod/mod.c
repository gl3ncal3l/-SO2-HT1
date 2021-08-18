#include <linux/module.h>
#include <linux/init.h>

static int load_module(void){
    printk(KERN_INFO "mod: Hola Mundo!\n");
    return 0;
}

static void unload_module(void){
    printk(KERN_INFO "mod: Adios Mundo!\n");
}

module_init(load_module);
module_exit(unload_module);