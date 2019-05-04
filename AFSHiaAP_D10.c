#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <grp.h>
#include <pwd.h>


static const char *dirpath = "/home/ramrom/shift4";
char ext[100000] = "\0";
int ind_ext =0;
char key[97] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
int ges = 17;
void enc(char* yangdienc)
{
	if(!strcmp(yangdienc,".") || !strcmp(yangdienc,"..")) return;
	for(int i=0;i<strlen(yangdienc);i++)
	{
		for(int j=0;j<94;j++){
			// printf("%c",key[j]);
			if(yangdienc[i]==key[j]){
				yangdienc[i] = key[(j+ges)%94];
				break;
			}
		}
	}
}

void dec(char* yangdidec)
{
	if(!strcmp(yangdidec,".") || !strcmp(yangdidec,"..")) return;
	for(int i=0;i<strlen(yangdidec);i++)
	{
		for(int j=0;j<94;j++){
			// printf("%c",key[j]);
			if(yangdidec[i]==key[j]){
				yangdidec[i] = key[(j+94-ges)%94];
				break;
			}
		}
	}
}

static void* pre_init(struct fuse_conn_info *conn)
{
        char folder[100000] = "/Videos";
		char folde1r[100000] = "/YOUTUBER";
		enc(folder);
		enc(folde1r);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));
		sprintf(fpath,"%s%s", dirpath, folde1r);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));

		pid_t child1;
		child1=fork();
		if(child1==0){
			DIR *dp;
			struct dirent *de;
			dp = opendir(dirpath);
			while((de = readdir(dp))){
				if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
					char ext[1000] = ".mkv";
					enc(ext);
					if(strlen(de->d_name)>7 && strncmp(de->d_name+strlen(de->d_name)-4,ext,1)==0){

							char joined[1000];
							char video[1000] = "/Videos";
							enc(video);
							sprintf(joined,"%s%s/",dirpath,video);
							strncat(joined,de->d_name,strlen(de->d_name)-4);
							struct stat st;
							if(stat(joined,&st)<0) creat(joined,0777);
							FILE* mainj;
							mainj = fopen(joined,"a+");
							FILE* need;
							char this[1000];
							sprintf(this,"%s/%s",dirpath,de->d_name);
							need = fopen(this,"r");
							int c;
   							c = fgetc(need);
							while(c!=EOF) {
   								fprintf(mainj,"%c",c);
   							}
							fclose(need);
							fclose(mainj);
							memset(this,0,sizeof(this));
							memset(joined,0,sizeof(joined));

					}
				}
			}
			exit(EXIT_SUCCESS);
		}

        (void) conn;
        return NULL;
}

int check_ext(char* file){
	ind_ext=0;
	while(ind_ext<strlen(file)&&file[ind_ext]!='.') ind_ext++;
	memset(ext,0,sizeof(ext));
	strcpy(ext,file+ind_ext);
	return ind_ext;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
    char fpath[1000];
	char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
        char katak[100000];
		strcpy(katak,de->d_name);
		char file[1000];
		sprintf(file,"%s%s",fpath,katak);
		dec(katak);

		struct stat tmp;
		stat(file,&tmp);
		struct passwd *name = getpwuid(tmp.st_uid);
    	struct group *grup = getgrgid(tmp.st_gid);
		
		if( (strcmp(name->pw_name,"chipset") == 0 || strcmp(name->pw_name,"ic_controller") == 0) 
			&& strcmp(grup->gr_name,"rusak")==0 
			&& ((tmp.st_mode & S_IRUSR) == 0 || (tmp.st_mode & S_IRGRP) == 0 || (tmp.st_mode & S_IROTH) == 0) )
		{
			printf("%s\n",file);
			char root[1000];
			strcpy(root,dirpath);
			char note[10000] = "/filemiris.txt";
			enc(note);
			strcat(root,note);
			FILE * fp;
   			fp = fopen (root, "a+");
			char t[1000];
			time_t now = time(NULL);
			strftime(t, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			char buffer[1000];
			sprintf(buffer,"%s%s-%d-%d-%s",path,katak,name->pw_uid,grup->gr_gid,t);
			fprintf(fp,"%s\n",buffer);
			remove(file);
			fclose(fp);
			chown(root,1000,1000);
		}
		else{
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if (filler(buf, katak, &st, 0))
				break;
		}
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0)
	{
		enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, 0750);	
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, mode);
		if (res == -1)
			return -errno;
	}
	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strstr(name,".swp")==0 && strstr(name,".gooutpustream")==0 && strstr(name,"/RecycleBin/") == 0){
		char folder[100000] = "/RecycleBin";
		enc(folder);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);

		char t[1000];
		time_t now = time(NULL);
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));

		char zip[1000];
		char fzip[1000];
		char fname[1000];
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enc(name);
		sprintf(fname,"%s%s",dirpath,name);
		enc(zip);
		sprintf(fzip,"%s%s",dirpath,zip);
		pid_t child1;
		char dum[10000];
		dec(name);
		sprintf(dum,"%s%s",dirpath,name);
		enc(name);

		int tunggu;
		child1=fork();
		if(child1==0){
			execl("/bin/cp","/bin/cp",fname,dum,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			waitpid(child1, &tunggu, 0);
			
		}
		
		child1=fork();
		if(child1==0){
			execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j",fzip,dum,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			waitpid(child1, &tunggu, 0);
		}
		remove(fname);
		strcat(fzip,".zip");

		char fback[1000] = "/Backup";
		enc(fback);
		char foldbackp[1000];
		sprintf(foldbackp,"%s%s",dirpath,fback);
		DIR *dp;
		struct dirent *de;

		dp = opendir(foldbackp);
		while((de = readdir(dp))){
			if(strncmp(name+1,de->d_name,strlen(de->d_name)-29)==0){
				
				memset(fname,0,sizeof(fname));
				dec(name);
				sprintf(fname,"%s/%s",foldbackp,de->d_name);
				enc(name);
				char dum[10000];
				dec(de->d_name);
				sprintf(dum,"%s/%s",dirpath,de->d_name);
				enc(de->d_name);				

				child1=fork();
				if(child1==0){
					execl("/bin/cp","/bin/cp",fname,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					waitpid(child1, &tunggu, 0);
				}

				child1=fork();
				if(child1==0){
					execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j","-u",fzip,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					waitpid(child1, &tunggu, 0);					
				}
				remove(fname);
			}
		}
		dec(name);
		memset(fzip,0,sizeof(fzip));
		enc(name);
		memset(fname,0,sizeof(fname));
		sprintf(fzip,"%s%s",dirpath,zip);
		strcpy(fname,fzip);
		strcat(fzip,".zip");
		rename(fzip,fname);
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = unlink(fpath);
		if (res == -1)
			return -errno;
	}
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = rmdir(fpath);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char new_from[1000];
    char new_to[1000];
	char name1[1000];
	char name2[1000];
	sprintf(name1,"%s",from);
	sprintf(name2,"%s",to);
	enc(name1);
	enc(name2);
    sprintf(new_from,"%s%s",dirpath,name1);
    sprintf(new_to,"%s%s",dirpath,name2);
	res = rename(new_from, new_to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	dec(name);
	struct stat st;
	stat(fpath,&st);
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0 && strcmp(name+strlen(name)-4,".iz1")==0 && !S_ISDIR(st.st_mode) && strstr(name+10,"/")==0)
	{

		pid_t child1;
		child1=fork();
		if(child1==0){
			execl("/usr/bin/zenity","/usr/bin/zenity","--error","--text=File ekstensi iz1 tidak boleh diubah permissionnya.","--title=Tidak bisa merubah",NULL);
		}
		else{
			wait(NULL);
		}
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = chmod(fpath, mode);
	}
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = lchown(fpath, uid, gid);
	if (res == -1) 
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
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

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];

	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath,"%s%s",dirpath,name);

	char newname[1000];
	char folder[1000] = "/Backup";
	enc(folder);
	char folderdir[1000];
	sprintf(folderdir,"%s%s",dirpath,folder);
	mkdir(folderdir,0755);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	printf("%s\n",buf);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	struct stat sd;
	if(stat(fpath,&sd)>-1 && strstr(path,".swp")==0){
		char t[1000];
		time_t now = time(NULL);
		char fname[1000];
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
		dec(name);
		check_ext(name);
		char shortn[100000];
		memset(shortn,0,strlen(shortn));
		strncpy(shortn,name,ind_ext);
		sprintf(newname,"/Backup%s%s%s",shortn,t,ext);
		enc(newname);
		memset(fname,'\0',sizeof(fname));
		sprintf(fname,"%s%s",dirpath,newname);
		// printf("%s\n",fpath);
		pid_t child1;
		child1=fork();
		if(child1==0){
			execl("/bin/cp","/bin/cp",fpath,fname,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}

		return res;
	}
	
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{

    (void) fi;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    int res;
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0)
	{
		strcat(name,".iz1");
		enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
    	res = creat(fpath, 0640);
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
    	res = creat(fpath, mode);
	}
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}


static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	(void) fpath;
	(void) fi;
	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static void post_destroy(void* private_data)
{
	char fpath[10000];
	char folder[100000] = "/Videos";
	enc(folder);
    sprintf(fpath,"%s%s", dirpath, folder);

	DIR *dp;
	struct dirent *de;
	dp = opendir(fpath);
	while((de = readdir(dp))){
		if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
			char file[1000];
			sprintf(file,"%s/%s",fpath,de->d_name);
			remove(file);
		}
	}
	rmdir(fpath);

	return;
}

static struct fuse_operations xmp_oper = 
{
	.init		= pre_init,
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.access		= xmp_access,
	.rename		= xmp_rename,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.create		= xmp_create,
	.release	= xmp_release,
	.destroy	= post_destroy,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
