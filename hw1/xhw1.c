#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<fcntl.h>
#define __NR_xconcat	349	/* our private syscall number */

struct file_details{
	const char *outfile; //name of output file
	const char **infiles; //array with names of input files
	unsigned int infile_count; //number of input files in infiles array
	int oflags;
	mode_t mode; //open flags to change behavior of syscall
	unsigned int flags; //speacial flags to change behavior of syscall
};

struct file_details args;//=malloc(sizeof(struct file_details*));
//args.infile_count=2;
int main(int argc, char *argv[])
{
	int rc;
	//struct file_details args;
	//args.infile_count=2;
	//const char *a[infile_count];
	//a[0]="file1.txt";
	//a[1]="file2.txt";
	args.outfile="/usr/src/hw1-vabhat/hw1/out2.txt";
	args.infiles=(const char *[]){"/usr/src/hw1-vabhat/hw1/file1.txt","/usr/src/hw1-vabhat/hw1/file2.txt"};
	args.infile_count=2;	
	args.mode=0264;
	args.oflags=O_CREAT|O_EXCL;
	args.flags=0x02;
	//int rc;
	//void *dummy = (void *) atoi(argv[1]);

  	rc = syscall(__NR_xconcat, (void *)&args);
	//if (rc == 0)
	printf("syscall returned %d\n", rc);
	//else
	//	printf("syscall returned %d (errno=%d)\n", rc, errno);

	exit(rc);
}
