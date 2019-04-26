#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

static const char *dirpath = "/home/ramrom/shift4";

char key[100] = {"qE1~ YMUR2\"`hNIdPzi\%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};
//enkripsi no1
void enkripsi(char *path){
	int pnjng = strlen(path), i, j;

	for(i=0; i<pnjng; i++){
		if(path[i] == '/'){
			continue;
		}
		for(j=0; j<94; j++){
			if(path[i] == key[j]){
				path[i] = key[(j + 17)%94];
				break;
			}
		}
	}
}
//dekripsi no1
void dekripsi(char *path){
	int pnjng = strlen(path), i, j;
	for(i=0; i<pnjng; i++){
		if(path[i] == '/'){
			continue;
		}
		for(j=0; j<94; j++){
			if(path[i] == key[j]){
				path[i] = key[(94+(j - 17))%94];
				break;
			}
		}
	}
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
	if(strcmp(path,"..")==0 || strcmp(path,".")==0) return 0;
  int res;
	enkripsi(path);
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	if(strcmp(path,"..")==0 || strcmp(path,".")==0) return 0;
  char fpath[1000];
	enkripsi(path);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		dekripsi(de->d_name);
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	if(strcmp(path,"..")==0 || strcmp(path,".")==0) return 0;
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}
//bagian dari no4
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	enkripsi(path);
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	//printf("%s\n", fpath);
	if(strstr(fpath, "YOUTUBER/") != NULL){
		res = creat(fpath, 0640);	
	}else{
		res = creat(fpath, mode);
	}
	//res = mkdir(fpath, 0750);
	if (res == -1)
		return -errno;

	return 0;
}
//bagian dari no4
static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;
	
	char fpath[1000], tmpath[100];
	sprintf(tmpath,"%s.iz1",path);
	enkripsi(tmpath);
	sprintf(fpath,"%s%s",dirpath,tmpath);
	char newpath[1000];
	strcpy(newpath, fpath);

    int res;
	//strcat(newpath,".iz1");
	if(strstr(newpath, "YOUTUBER/") != NULL){
		res = creat(newpath, 0640);	
	}else{
		res = creat(newpath, mode);
	}
	
    
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}
//bagian dari no4
static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	
	char cek[4] = {".iz1"};
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);	
	
	if(strstr(fpath, "YOUTUBER/") != NULL){
		if(strstr(fpath, cek) != NULL){
			pid_t child_id;
			child_id = fork();
			if(child_id == 0){
				char pesan[100] = {"File ekstensi iz1 tidak boleh diubah permissionnya."};
				char *argvnot[] = {"zenity", "--notification", "--text", pesan, NULL};
				execv("/usr/bin/zenity", argvnot);
			}else{
				return 0;
			}
			return 0; 
		}
	}	

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}
//bagian dari no4
static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}
//bagian dari no4
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];

	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
	.create 	= xmp_create,
	.chmod		= xmp_chmod,
	.write		= xmp_write,
	.utimens	= xmp_utimens,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
