//  Librería que proporcional as funciones para poder crear y escribir un archio en el directorio /proc
#include <linux/proc_fs.h>
//  Librería que proporciona una función que agrega un buffer al archivo en el directorio /proc
#include <linux/seq_file.h>
//  Librería que proporciona el acceso del usuario.
#include <asm/uaccess.h> 
//  Librería que permite utilizar páginas de memoria virtual de gran tamaño.
#include <linux/hugetlb.h>
//  Librería que proporciona lo necesario para definir el módulo como uno módulo de Linux.
#include <linux/module.h>
//  Librería que proporciona los macros para iniciar un módulo, además de macros para finalizarlo.
#include <linux/init.h>
//  Librería que indica el módulo del Kernel
#include <linux/kernel.h> 
//  Librería que proporciona las estructuras de apoyo para manejar
#include <linux/fs.h>
//  Librería que proporciona la estructura para tareas o procesos.
#include <linux/sched.h>     
//  Libreria que proporciona Macros para realizar iteraciones.
#include <linux/sched/signal.h>  
//  Librería que se utiliza para la manipulación de áreas de memoria. Provee la función get_mm_rss(), la cual calcula la memoria de cada proceso.
#include <linux/mm.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 6");
MODULE_DESCRIPTION("Modulo de Administrador de Procesos");
MODULE_VERSION("1.0");

//  Estructura definida en sched.h para tareas o procesos.
struct task_struct *task_list;
//  Estructura necesaria para iterar a través de los hijos de cada tarea.
struct task_struct *task_list_child;
//  Estructura necesaria para recorrer la lista en cada estructura task->children.
struct list_head *list;

static int writeFile(struct seq_file* archivo, void *v){

    seq_printf(archivo, "[");
    // Macro para iterar a través de cada tarea en el sistema operativo ubicado en linux/sched/signal.h
    for_each_process(task_list) {
            seq_printf(archivo,"{\"pid\":%d,\"nombre\":\"%s\",\"estado\":%ld,\"ram\":%ld,\"usuario\":%u,\"hijos\":[", task_list->pid,task_list->comm,task_list->state,(task_list->mm != NULL? get_mm_rss(task_list->mm): 0)/1024,(task_list->cred-> uid.val));
            // Macro para iterar a través de la estructura task->children
            list_for_each(list, &task_list->children){ 
                // Se utiliza list_entry para decarar todas las variables en la estructura task_child      
                task_list_child = list_entry( list, struct task_struct, sibling );             
                seq_printf(archivo,"{\"pid\":%d,\"nombre\":\"%s\",\"estado\":%ld,\"ram\":%ld,\"usuario\":%u},",task_list_child->pid, task_list_child->comm, task_list_child->state,(task_list_child->mm != NULL? get_mm_rss(task_list_child->mm): 0)/1024,(task_list_child->cred-> uid.val));
            }
            seq_printf(archivo, "]},");
    }
    
    seq_printf(archivo, "]");
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