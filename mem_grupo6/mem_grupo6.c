#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 6");
MODULE_DESCRIPTION("Basic module");
MODULE_VERSION("1.0");

static int writeFile(struct seq_file* archivo, void *v){
    struct sysinfo inf;

    long memoria_total;
    long memoria_libre;
    long memoria_buffer;
    long memoria_compartida;
    long memoria_freehigh;
    long memoria_uso;

    si_meminfo(&inf);

    memoria_total = (inf.totalram * 4);
    memoria_libre = (inf.freeram * 4);
    memoria_buffer = (inf.bufferram * 4);
    memoria_compartida = (inf.sharedram * 4);
    memoria_freehigh = (inf.freehigh * 4);
    memoria_uso = memoria_total - (memoria_libre + memoria_compartida + memoria_buffer);

    seq_printf(archivo, "Memoria total: %lu MB\n", memoria_total/1024);
    seq_printf(archivo, "Memoria libre: %lu MB\n", memoria_libre/1024);
    seq_printf(archivo, "Memoria en uso: %lu MB\n", memoria_uso/1024);
    
    return 0;
}

static int atOpen(struct inode* inode, struct file* file){
    return single_open(file, writeFile, NULL);
}

static struct file_operations ops = {
    .open = atOpen,
    .read = seq_read
};

static int load_module(void){   
    proc_create("mem_grupo6", 0, NULL, &ops);
    printk(KERN_INFO "Hola Grupo 6\n");
    return 0;
}

static void unload_module(void){
    remove_proc_entry("mem_grupo6", NULL);
    printk(KERN_INFO "Adiós Grupo 6\n");
}

module_init(load_module);
module_exit(unload_module);