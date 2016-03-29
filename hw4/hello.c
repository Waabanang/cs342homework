#include <linux/init.h>
#include <linux/module.h>
#include <linux/hernel.h>

static int hello_init(void){
	//using printk to print "Hello Josh!" to the
	//debug loglevel
	printk(KERN_DEBUG "Hello Josh!");
	return 0;
}
static void hello_exit(void){
	//using printk (kernel version of printf) to 
	//print "Goodbye!" at log-level KERN_NOTICE
	printk(KERN_NOTICE "Goodbye!");
}

module_init(hello_init);//modules init function is hello_int
module_exit(hello_exit);//modules exit function is hello_exit

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Waabafet");
MODULE_DESCRIPTION("Practice module for HM4");


