#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/math64.h>
#include <linux/types.h>

#define DEVICE_NAME "probchar"

unsigned long seed = 23545;

ssize_t probchar_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
    char random_char;
	seed = ((seed * 1103515245 + 12345) ) % 32768; 
	random_char = (char)(seed % 26 + 65);
	if( copy_to_user(data, &random_char, 1) ) {
		printk(KERN_ERR "No se pudo copiar el char random");
		return -EFAULT;
	}
    return 1;
}

ssize_t probchar_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
	unsigned long new_seed;
	char chars[s+1];
	if (copy_from_user(chars, data, s)) {
		printk(KERN_ERR "Error al copiar los datos del usuario\n");
		return -EFAULT;
	}
	chars[s] = '\0'; // MARCO LA FINALIZACION DEL CHAR ARRAY
	if( strict_strtoul(chars, 10, &new_seed) ) {
		printk(KERN_ERR "El numero ingresado no es un unsigned long\n");
		return -EFAULT;
	}
	seed = new_seed;		
	printk(KERN_ALERT "Semilla cambiada> %lu\n", seed);
	return s;
}

// Estructura propia del device
struct probchar_dev {
    struct cdev cdev;
};
struct probchar_dev probchar_dev;


// file_operations que se asociara al cdev
static struct file_operations probchar_fops = {
    .owner = THIS_MODULE,
    .read = probchar_read,
    .write = probchar_write,
};

// Device number
static dev_t probchar_devno;

// Class
static struct class *probchar_class;


static int __init probchar_init(void) {
    // Pedimos un device number al kernel dinamicamente
    if(alloc_chrdev_region(&probchar_devno, 0, 1, DEVICE_NAME)) {
        printk(KERN_DEBUG "probchar: No se pudo registrar el device\n");
        return 1;
    }

    // Conectamos el file_operations con el cdev
    cdev_init(&probchar_dev.cdev, &probchar_fops);
    probchar_dev.cdev.owner = THIS_MODULE;

    // Conectamos el device number al cdev
    if (cdev_add(&probchar_dev.cdev, probchar_devno, 1)) {
        printk(KERN_DEBUG "probchar: Error al agregar el char device\n");
        return 1;
    }

    // Hacemos que se creen los nodos en /dev
    probchar_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(probchar_class, NULL, probchar_devno, NULL,
        DEVICE_NAME);
	return 0;
}

static void __exit probchar_exit(void) {
    // Quitamos el cdev
    cdev_del(&probchar_dev.cdev);

    // Liberamos el major number
    unregister_chrdev_region(MAJOR(probchar_devno), 1);

    // Destruimos la clase y el device
    device_destroy(probchar_class, probchar_devno);
    class_destroy(probchar_class);
}

module_init(probchar_init);
module_exit(probchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SisOp");
MODULE_DESCRIPTION("generador de numeros pseudo-aleatorios "
    "(crea 'manualmente' un char device)");
