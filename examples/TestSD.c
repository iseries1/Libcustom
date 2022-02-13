/**
 * @brief Test SD Card Functions
 * @author Michael Burmeister
 * @date January 31, 2021
 * @version 1.0
 * 
*/

#define PIN_MISO 20
#define PIN_CLK  21
#define PIN_MOSI 22
#define PIN_SS   23

#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <propeller2.h>
#include <sdcard.h>

void dodir(void);
void doread(void);
void dowrite(void);


char Buffer[512];
FILE *fp;
int i;
unsigned p;
const char message[] = "What is this:\n";


int main(int argc, char** argv)
{
	struct tm ti;
    struct timeval tv;
	time_t t;
	
	ti.tm_year = 2021 - 1900;
	ti.tm_mon = 2;
	ti.tm_mday = 7;
	ti.tm_hour = 6;
	ti.tm_min = 0;
	ti.tm_sec = 0;
	t = mktime(&ti);
    tv.tv_sec = t;
	settimeofday(&tv, 0);
	
	printf("Mounting...\n");
    i = sd_mount(PIN_SS, PIN_CLK, PIN_MOSI, PIN_MISO);

    if (i != 0)
    {
        printf("Mount Failed\n");
        while (1)
            _wait(1000);
    }

    dodir();

    doread();

    dowrite();
    
    printf("Done\n");
    
    while (1)
    {
        _wait(1000);
	}
}

void dodir()
{
    DIR *dir;
	struct dirent ent;

    dir = sd_opendir("/");

    if (dir == NULL)
    {
        printf("Directory Failed(%d)!\n", errno);
        while (1)
        	_wait(1000);
    }
    
    printf("Mounted..\n");
   
    while (!sd_readdir(dir, &ent))
    {
        if ((ent.d_type & 0x10) != 0)
	        printf("d %s\n", ent.d_name);
	    else
	    	printf("  %s\n", ent.d_name);
    }
    
    sd_closedir(dir);

}

void doread()
{
    printf("Opening file OneCall\n");
    
    fp = fopen("SD:/OneCall.txt", "r");
    if (fp == NULL)
    {
        printf(" File Not Found!\n");
        while (1)
        	_wait(1000);
    }

    i = fread(Buffer, 1, 256, fp);

    printf("Read %d bytes\n", i);
    
    Buffer[i] = 0;
    printf("Buffer: %s\n", Buffer);
    
    fclose(fp);
}

void dowrite()
{

    printf("Writing log file\n");

    fp = fopen("SD:/logfile.txt", "a");
    if (fp == NULL)
    {
        printf("File Creation Error!\n");
        while (1)
        	_wait(1000);
    }
	printf("File Pointer %lx\n", (unsigned long)fp->drvarg[0]);
	printf("Buffer Base: %lx\n", (unsigned long)fp->_base);
	printf("Buffer Size %d\n", (int)fp->_bsiz);
	printf("File Flags: %x\n", fp->_flag);
	printf("File Lock: %d\n", fp->_lock);
	printf("Buffer Pointer: %lx\n", (unsigned long)fp->_ptr);
	printf("Buffer Count: %ld\n", fp->_cnt);
	printf("Driver Open: %lx\n", (unsigned long)fp->_drv->fopen);
	printf("Driver Read: %lx\n", (unsigned long)fp->_drv->read);
	printf("Driver Write: %lx\n", (unsigned long)fp->_drv->write);
	printf("Driver Close: %lx\n", (unsigned long)fp->_drv->fclose);


    printf("About to write...\n");
    
    i =  fwrite("Test Data only\n", 1, 15, fp);
    
    printf("Wrote %d\n", i);

    _wait(5000);

	printf("Error: %d\n", errno);

    fclose(fp);
}
