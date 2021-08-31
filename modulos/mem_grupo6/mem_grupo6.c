//  Librería que proporcional as funciones para poder crear y escribir un archio en el directorio /proc
#include <linux/proc_fs.h>
//  Librería que proporciona una función que agrega un buffer al archivo en el directorio /proc
#include <linux/seq_file.h>
//  Librería que proporciona lo necesario para definir el módulo como uno módulo de Linux.
#include <linux/module.h>
//  Librería que proporciona los macros para iniciar un módulo, además de macros para finalizarlo.
#include <linux/init.h>
//  Librería que se utiliza para la manipulación de áreas de memoria. Provee la función get_mm_rss(), la cual calcula la memoria de cada proceso.
#include <linux/mm.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 6");
MODULE_DESCRIPTION("Modulo de Memoria RAM.");
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

    seq_printf(archivo, "{\ntotal: %lu MB\n", memoria_total/1024);
    seq_printf(archivo, "libre: %lu MB\n", memoria_libre/1024);
    seq_printf(archivo, "uso: %lu MB\n", memoria_uso/1024);
    seq_printf(archivo, "consumida: %lu MB\n}", (memoria_uso * 100)/memoria_total);
    
    return 0;
}

static int atOpen(struct inode* inode, struct file* file){
    return single_open(file, writeFile, NULL);
}

static struct file_operations ops = {
    .owner = THIS_MODULE,
    .open = atOpen,
    .release = single_release,
    .read = seq_read
};

static int load_module(void){   
    proc_create("mem_grupo6", 0, NULL, &ops);
    printk(KERN_INFO "Hola mundo, somos el grupo 6 y este es el monitor de memoria\n");
    return 0;
}

static void unload_module(void){
    remove_proc_entry("mem_grupo6", NULL);
    printk(KERN_INFO "Sayonara mundo, somos el grupo 6 y este fue el monitor de memoria\n");
}

module_init(load_module);
module_exit(unload_module);