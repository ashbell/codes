
<Lie To Me>

Chapter 3. FILE I/O
	#include <fcntl.h>
		int open(const char *pathname, int oflags, /* mode_t mode */);
	OFLAGS: O_RDONLY, O_WRONLY, O_RDWR
	  MODE: O_APPEND, O_EXCL, O_TRUNC, O_NOCTTY, O_NONBLOCK
		int creat(const char *pathname, mode_t mode);
		int fcntl(int fd, int cmd,.../* int arg */)
	   ERR: -1
	
	#include <unistd.h>
		int lseek(int fd, off_t offset, int whence);
	WHENCE: SEEK-SET, SEEK_CUR, SEEK_END

		ssize_t read(int fd, void *buf, size_t nbytes);
		ssize_t write(int fd, const void *buf, size_t nbytes);
		int dup(int fd);
		int dup2(int fd, int fd2); 
	   ERR: -1			
		

	#include <sys/ioctl.h>
		int ioctl(int fd, int request,...) 
	   ERR: -1			

Chapter 4. FILE AND DIRECTORY
	#include <sys/stat.h>
		struct stat {};
		int stat(const char *restrict_pathname, struct stat *restrict_buf);
		int fstat(int fd, struct stat *buf);
		int lstat(const char *restrict_pathname, struct stat *restrict_buf);
		int chmod(const char *pathname, mode_t mode);
		int fchmod(int fd, mode_t mode);
		int mkdir(const char *pathname, mode_t mode);
		int rmdir(const char *pathname);
	   ERR: -1


	#include <unistd.h>
		int link(const char *existpath, const char *newpath);
		int ulink(const char *pathname);
		int symlink(const char *actualpath, const char *sympath);
		int readlink(const char *restrict_pathname, char *restrict_buf, size_t bufsz);
		int chdir(const char *pathname);
		int fchdir(int fd);
		char *getcwd(char buf, size_t size); /* get the current work directory path */
	   ERR: -1
		

	#include <utime.h>
		int utime(const char *pathname, const struct utimbuf *times);
			struct utimbuf {
			  time_t actime; /* access time */
			  time_t modtime;/* modification time */
			}


	#include <dirent.h> /* Important tips */
		DIR *opendir(const char *pathname);
		struct dirent *readdir(DIR *dp);
			struct dirent {
			  ino_t d_ino;
			  char  d_name[NAME_MAX + 1]; /* add '\0' */
			}
	   ERR: -1
	   	void rewinddir(DIR *dp);
		int closedir(DIR *dp);
	   ERR: -1 
	   	long telldir(DIR *dp)
		void seekdir(DIR *dp, long loc);

	/* The pointer returned by fun 'opendir' used for other 5 functions */


Chapter 5. Standard I/O LIBRARY










