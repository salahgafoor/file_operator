#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define BUFFER_SIZE 128

static char *message_buffer;
static ssize_t message_length;

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("salahgafoor"); 
MODULE_DESCRIPTION("File operator driver in Linux");

static struct proc_dir_entry *proc_entry;

static ssize_t put_file_content(struct file * file_pointer, 
                                const char *user_space_buffer, 
                                size_t count, 
                                loff_t *offset)
{
    if (count > BUFFER_SIZE - 1)
        return -EINVAL;

    memset(message_buffer, 0, BUFFER_SIZE); // Clear buffer
    message_length = count;

    if (copy_from_user(message_buffer, user_space_buffer, count)) {
        return -EFAULT;
    }

    message_buffer[count] = '\0';
    printk("Buffer content %s has been written to module", message_buffer);
    return message_length;

}    

static ssize_t get_file_content(struct file * file_pointer, 
                                char *user_space_buffer, 
                                size_t count, 
                                loff_t *offset)
{
    int result;

    if (*offset > 0 || count < message_length) // Check if already read or buffer size too small
        return 0;

    result = copy_to_user(user_space_buffer, message_buffer, message_length);
    if(result) // we are expecting a zero; if it's one, then return error
    {
        return -EFAULT;
    }
    printk("This is from file operator driver");
    
    *offset = message_length;
    
    return message_length;
}

struct proc_ops proc_ops_driver = {
    .proc_read = get_file_content,
    .proc_write = put_file_content
};

static int opening_function (void) 
{ 
    message_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!message_buffer) {
        return -ENOMEM;
    }
    proc_entry = proc_create("file_operator_driver", 
                                    0666,
                                    NULL,
                                    &proc_ops_driver);

    printk("file_operator_driver Module has been inserted");
    return 0;
}

static void closing_function (void) 
{ 

    proc_remove(proc_entry);
    kfree(message_buffer);
    printk("file_operator_driver Module has been removed");
}

module_init(opening_function);
module_exit(closing_function);