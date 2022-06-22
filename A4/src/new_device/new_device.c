
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>


#define LICENSE "GPL"

MODULE_LICENSE(LICENSE);

static struct miscdevice my_device;
static struct semaphore full;
static struct semaphore empty;
static struct semaphore _read;
static struct semaphore _write;

static int no_of_open_devices;
static int buffer_size;
static int bytes_string = 200;
static int index_r = 0;
static int index_w = 0;
static int buf_clear;
static struct semaphore full;
static struct semaphore empty;
static struct semaphore _read;
static struct semaphore _write;
char** buffer;

module_param(buffer_size, int, 0000);

static ssize_t dequeue_data(struct file*, char*, size_t, loff_t*);
static ssize_t enqueue_data(struct file*, const char*, size_t, loff_t*);
static int open_device(struct inode*, struct file*);
static int close_device(struct inode*, struct file*);
static struct file_operations my_device_fops = {
	.open = &open_device,
	.read = &dequeue_data,
	.write = &enqueue_data,
	.release = &close_device
};

int init_module(){
	my_device.name = "new_device";
	my_device.fops = &my_device_fops;
	my_device.minor = MISC_DYNAMIC_MINOR;
	int res;
	if((res = misc_register(&my_device))){
		printk(KERN_ERR "Device new_device cannot be registered\n");
		return res;
	}
	printk(KERN_INFO "New Device Registered:\n");
	printk(KERN_INFO "device name: new_device\n");
	printk(KERN_INFO "buffer size: %d\n", buffer_size);
    sema_init(&_read, 1);
	sema_init(&_write, 1);
    sema_init(&full, 0);
	sema_init(&empty, buffer_size);
	int data_allocated = 0;

	buffer = (char**)kmalloc(buffer_size * sizeof(char*), GFP_KERNEL);

	while(data_allocated < buffer_size)
    {
		buffer[data_allocated] = (char*)kmalloc((bytes_string + 1) * sizeof(char), GFP_KERNEL);
		buffer[bytes_string] = '\0';
		++data_allocated;
	}
    no_of_open_devices = 0;
    buf_clear = buffer_size;
	return 0;
}

static int open_device(struct inode* _inode, struct file* fp){
	++no_of_open_devices;
	return 0;
}
static int close_device(struct inode* _inode, struct file* fp){
    --no_of_open_devices;
	return 0;
}
static ssize_t dequeue_data(struct file* fp, char* user_buffer, size_t char_read_size, loff_t* pos){
    int i = 0;
	down_interruptible(&_read);
	down_interruptible(&full);
	index_r %= buffer_size;
	for(i = 0; i < char_read_size; i++){
		if(buf_clear >= buffer_size){
			break;
		}
		copy_to_user(&user_buffer[i], &buffer[index_r][i], 1);
	}
	++buf_clear;
    ++index_r;
	up(&empty);
	up(&_read);
	return i;
}

static ssize_t enqueue_data(struct file* fp, const char* user_buffer, size_t char_write_size, loff_t* pos){
	int i = 0;
	down_interruptible(&_write);
	down_interruptible(&empty);
	index_w %= buffer_size;
	for(i = 0; i < char_write_size; i++)
    {
		if(buf_clear <= 0)
        {
			break;
		}
		copy_from_user(&buffer[index_w][i], &user_buffer[i], 1);
	}
	--buf_clear;
    ++index_w;
    // while(1){
    //     result = syscall(READER_SYSCALL_NO, random_str, BYTE_COUNT);
    //     //result = read(fd1, random_str, sizeof(random_str));
    //     if(result < 0){
    //         perror("syscall");
            
    //     }
    //     if(result >= 0){
    //         printf("%d\n", result);
    //         printf("******************************************\n\nstring received: %d %s\n\n", cnt,random_str);
    //         cnt++;
    //         //memset(random_str, 0, 8);

    //     }
    //     sleep(2);
    // }
	up(&full);
	up(&_write);
	return i;
}


void cleanup_module(){
	int free_space;
	for(free_space = 0; free_space < buffer_size; free_space++){
		kfree(buffer[free_space]);
	}
	misc_deregister(&my_device);
	printk(KERN_INFO "Device Unregistered: new_device\n");
}
