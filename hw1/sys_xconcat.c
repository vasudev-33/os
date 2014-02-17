#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/vmalloc.h>
#include<linux/stat.h>
#include<linux/types.h>
asmlinkage extern long (*sysptr)(void *arg);
int wrapfs_read_file(const char *filename,void *buf,int len, int startPos);
int wrapfs_write_file(const char* filename,void *buf,int len);
int wrapfs_create_file(const char *filename,int oflags, mode_t mode);
struct file_details{
	const char *outfile;
	const char **infiles;
	int infile_count;
	int oflags;
	mode_t mode;
	unsigned int flags;

};

asmlinkage long xconcat(void *arg)
{
	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	int i;
	struct file_details *details=(struct file_details *)arg; 
	int len=5;
	int read_bytes=len;
	int startPos=0;
	int write_bytes=0;
	char *buf;
	int total_bytes_written=0;
	int total_bytes_read=0;
	int percent_data_written=0;
	int total_files_written=0;
	printk("Creating file %s",details->outfile);
	wrapfs_create_file(details->outfile,details->oflags,details->mode);
		
	for(i=0;i<details->infile_count;i++){
		printk("Reading file %s\n", details->infiles[i]);
		startPos=0;
		read_bytes=len;
		while(read_bytes==len){
			buf=vmalloc(len);
			read_bytes=wrapfs_read_file(details->infiles[i],(void *) buf,len,startPos );
			printk("%s %d\n",buf,read_bytes);
			write_bytes=wrapfs_write_file(details->outfile,(void *)buf,len);
			startPos+=read_bytes;
			total_bytes_read+=read_bytes;
			total_bytes_written+=write_bytes;
			vfree(buf);
		}
		total_files_written+=1;
	}

	
	if(details->flags==0x00){
		return total_bytes_written;
	}else if(details->flags==0x01){
		return total_files_written;
	}else if(details->flags==0x02){
		percent_data_written=(total_bytes_written/total_bytes_read)*100;
		return percent_data_written;
	}
}

int wrapfs_create_file(const char* filename,int oflags,mode_t mode){

	struct file *filp;
	mm_segment_t oldfs;
	filp=filp_open(filename,oflags,mode);
	printk("%s",filename);
	if(IS_ERR(filp)){
		printk("wrapfs_write_file error %d\n",(int) PTR_ERR(filp));
		return -1;
	}
	if(!filp){
		printk("filp is null");
		return -2;
	} 		
	oldfs=get_fs();
	set_fs(KERNEL_DS);
	set_fs(oldfs);
	filp_close(filp,NULL);
	return 1;

}



int wrapfs_read_file(const char* filename, void *buf, int len,int startPos){
	struct file *filp;
	mm_segment_t oldfs;
	int bytes;
	filp=filp_open(filename,O_RDONLY,0);
	//printk("here");
	if(IS_ERR(filp)){
		printk(KERN_ALERT "wrapfs_read_file error %d\n",(int) PTR_ERR(filp));
		printk("in erro");
		return -5;
	}
	if(!filp){
		printk("filp is null");
		return -6;
	} 		
	if(!filp->f_op->read)
		return -2;
	//printk("i am here");
	filp->f_pos=startPos;
	oldfs=get_fs();
	set_fs(KERNEL_DS);
	bytes=filp->f_op->read(filp,buf,len,&filp->f_pos);
	set_fs(oldfs);
	
	filp_close(filp,NULL);
	return bytes;
}


int wrapfs_write_file(const char *filename,void *buf,int len){
	struct file *filp;
	mm_segment_t oldfs;
	int bytes;
	filp=filp_open(filename,O_APPEND,0);
	
	if(IS_ERR(filp)){
		printk(KERN_ALERT "wrapfs_write_file error %d\n",(int) PTR_ERR(filp));
		return -5;
	}
	if(!filp){
		printk("filp is null");
		return -6;
	} 		
	if(!filp->f_op->write)
		return -2;
	filp->f_pos=0;
	oldfs=get_fs();
	set_fs(KERNEL_DS);
	bytes=filp->f_op->write(filp,buf,len,&filp->f_pos);
	set_fs(oldfs);
	
	filp_close(filp,NULL);
	return bytes;
}


static int __init init_sys_xconcat(void)
{
	printk("installed new sys_xconcat module\n");
	if (sysptr == NULL)
		sysptr = xconcat;
	return 0;
}
static void  __exit exit_sys_xconcat(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	printk("removed sys_xconcat module\n");
}
module_init(init_sys_xconcat);
module_exit(exit_sys_xconcat);
MODULE_LICENSE("GPL");
