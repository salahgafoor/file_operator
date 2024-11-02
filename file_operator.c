#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("salahgafoor"); 
MODULE_DESCRIPTION("File operator driver in Linux");

static struct proc_dir_entry *proc_entry;

static ssize_t get_file_content(struct file * file_pointer, 
                                char *user_space_buffer, 
                                size_t count, 
                                loff_t *offset)
{
    char msg[] = "Module: File Operator\n";
    size_t len = strlen(msg);
    int result;

    if(*offset >= len)
    {
        return 0;
    }

    result = copy_to_user(user_space_buffer, msg, len);
    printk("This is from file operator driver");
    
    *offset += len;
    
    return len;
}
struct proc_ops proc_ops_driver = {
    .proc_read = get_file_content
};

static int opening_function (void) 
{ 
    proc_entry = proc_create("file_operator_driver", 
                                    0,
                                    NULL,
                                    &proc_ops_driver);

    printk("file_operator_driver Module has been inserted");
    return 0;
}

static void closing_function (void) 
{ 
    proc_remove(proc_entry);
    printk("file_operator_driver Module has been removed");
}

module_init(opening_function);
module_exit(closing_function);