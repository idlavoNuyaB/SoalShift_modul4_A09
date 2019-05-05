#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

static const char *dirpath = "/home/arvanna/Downloads";

static int a09_getattr(const char *path, struct stat *stbuf)
{
  int res,i,j;
	int key=17;
	char fpath[1000],name[1000];
	char plain[1000];
	char check;
	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	strcpy(name,path);
	// printf("%s\n",name);
	if(strcmp(name,".")!=0 && strcmp(name,"..")!=0){
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
	}
	//int tes=strlen(path)-4;
	//strcpy(temp,path);
	//temp[tes]='\0';
	sprintf(fpath,"%s%s",dirpath,name);
	//printf("%s\n",name);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int a09_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000];
	char check,temp[1000];
  	int i=0;
	int j=0;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
		// puts(fpath);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//puts(fpath);
	}
	int res = 0;

	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;
	//puts(fpath);
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		strcpy(name,de->d_name);
		sprintf(temp,"%s/%s",fpath,name);//no.3
		stat(temp,&st);
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		struct passwd *owner=getpwuid(st.st_uid);//no.3
		struct group *grup=getgrgid(st.st_gid);//no.3
		struct tm *times=localtime(&st.st_atime);//no.3
		// printf("%s %s %s\n",temp,owner->pw_name,grup->gr_name);
		// strcat(de->d_name,".bak");
		if(strcmp(name,".")!=0 && strcmp(name,"..")!=0){
			if((strcmp(owner->pw_name,"chipset")==0 || strcmp(owner->pw_name,"ic_controller")==0) && strcmp(grup->gr_name,"rusak")==0){
				mode_t chek = st.st_mode;
				if((chek && S_IRGRP)||(chek && S_IROTH)||(chek && S_IRUSR)){
					FILE *fill;//no.3
					int akhir=times->tm_year+1900;
					char pat[1000];
					char to[1000];
					sprintf(pat,"%s/V[EOr[c[Y`HDH",dirpath);
					sprintf(to,"%s %d %d %02d:%02d:%02d %02d-%02d-%04d \n",temp,owner->pw_uid,grup->gr_gid,times->tm_hour,times->tm_min,times->tm_sec,times->tm_mday,times->tm_mon,akhir);
					fill=fopen(pat,"a+");
					fputs(to,fill);
					fclose(fill);
				}
				if(S_ISDIR(st.st_mode)){
					rmdir(temp);
				}
				else{
					remove(temp);
				}
			}
			else{
				for(i=0;i<strlen(name);i++){
					//decode
					if(name[i]=='/'){
						name[i]='/';
					}
					else{
						for(j=0;j<strlen(plain);j++){
							if(name[i]==plain[j]){
								j=j-key;
								if(j<strlen(plain)&&j>=0){
									name[i]=plain[j];
								}
								else if(j<0){
									j=j+strlen(plain);
									name[i]=plain[j];
								}
							}
						}
					}
				}
				//printf("%s\n",de->d_name);
				res = (filler(buf, name, &st, 0));
					if(res!=0) break;
			}
		}
	}
	closedir(dp);
	return 0;
}

static int a09_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	int res = 0;
	int fd = 0;
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

static int a09_mkdir(const char *path,mode_t mode){
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
	int res;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	if(strstr("@ZA>AXio",fpath)==0){
		res=mkdir(fpath,0750);
	}
	else{
		res=mkdir(fpath,mode);
	}
	if(res==-1)
		return -errno;
	
	return 0;
}

static int a09_create(const char *path, mode_t mode,struct fuse_file_info *fi){
  	char fpath[1000];
	char iz[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
  	(void) fi;
	int res;
	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	if(strstr(fpath,"/@ZA>AXio")){
		res=creat(fpath,0640);
		pid_t child;
		child=fork();
		if(child==0){
			strcpy(iz,fpath);
			strcat(iz,"`[S%");
			char *argv[]={"mv",fpath,iz,NULL};
			execvp(argv[0],argv);
		}
		return 0;
	}
	else{
		res=creat(fpath,mode);
	}
	if(res==-1)
		return -errno;
	
	return 0;
}

static int a09_chmod(const char *path, mode_t mode){	  	
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
	int res;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	if(strstr(path,"YOUTUBER")){
		if(strstr(path,"iz1")){
			pid_t child;
			child=fork();
			if(child==0){
				char *argv[]={"zenity","--warning","--text='File ekstensi iz1 tidak boleh diubah permissionnya.'",NULL};
				execv("/usr/bin/zenity",argv);
			}
			return 0;
		}
		else{
			res=chmod(fpath,mode);
		}
	}
	else{
		res=chmod(fpath,mode);
	}
	if(res==-1)
		return -errno;
	return 0;
}

static int a09_write(const char *path, const char *buf, size_t size,off_t offset,struct fuse_file_info *fi){
	int fd;
	int res;
	(void) fi;
	char fpath[1000],*ekstensi;
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
	int status;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	pid_t child;
	child=fork();
	if(child!=0){
		char backup[1000];
		sprintf(backup,"%s/XB.Jhu",dirpath);
		mkdir(backup,0750);
		ekstensi=strrchr(path,'.');
		char now[1000],namae[1000],final[1000],extens[1000],extens2[1000],temp[1000],almost[1000],tes[1000];
		strcpy(extens,ekstensi);
		time_t times=time(NULL);
		strftime(now,100,"_%Y-%m-%d_%H:%M:%S",localtime(&times));
		if(extens==NULL && extens2==NULL)sprintf(almost,"%s%s",path,now);
		strcpy(tes,strtok(path,"."));
		strcpy(extens2,strtok(NULL,"."));
		if(extens2!=NULL)sprintf(temp,".%s",extens2);
		if(strcmp(temp,extens)==0 && extens!=NULL)sprintf(almost,"%s%s%s",tes,now,extens);
		else if(strcmp(temp,extens)!=0 && extens!=NULL)sprintf(almost,"%s%s%s%s",tes,temp,now,extens);
		strcpy(namae,almost);
		for(i=0;i<strlen(namae);i++){
			check=namae[i];//encode
			if(check=='/'){
					namae[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(namae[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							namae[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							namae[i]=plain[j];
						}
					}
				}
			}
			sprintf(final,"%s/%s",backup,namae);
			//printf("%s\n",final);
		}
		char *argv[]={"cp",fpath,final,NULL};
		execvp(argv[0],argv);
	}
	else{
		fd=open(fpath,O_WRONLY);
		if (fd == -1)
			return -errno;

		res = pwrite(fd, buf, size, offset);
		if (res == -1)
			res = -errno;

		close(fd);
	}
	return res;
}

static int a09_unlink(const char *path){
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	// char backup[1000], sampah[1000],asli[1000],temp[1000],ekstens[1000],temp2[1000],*ekstension;
	// ekstension=strrchr(path,".");
	// strcpy(asli,fpath);
	// sprintf(backup,"%s/XB.Jhu",dirpath);
	// sprintf(sampah,"%s/oO.k.EOX[)",dirpath);
	// strftime(now,100,"_%Y-%m-%d_%H:%M:%S",localtime(&times));
	// mkdir(sampah,0750);
	// strcpy(temp,strtok(path,"."));
	// strcpy(ekstens,strtok(NULL,"."));
	// if(strcmp(ekstension,ekstens)!=0 && ekstens!=NULL)sprintf(temp2,".%s",ekstens);
	
	
	int res;
	res =unlink(fpath);
	if(res==-1)
		return -errno;
	
	return 0;
}

static int a09_truncate(const char *path,off_t size){
	int res;
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	res =truncate(fpath,size);
	if(res==-1)
		return -errno;

	return 0;
}

static int a09_utimens(const char *path,const struct timespec ts[2]){
	int res;
	char fpath[1000];
	char name[1000];
	int key = 17;
	char plain[1000],check;
	int i=0;
	int j=0;
  	strcpy(plain,"qE1~ YMUR2");
  	strcat(plain,"\"");
  	strcat(plain,"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0");
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		strcpy(name,path);
		for(i=0;i<strlen(name);i++){
			check=name[i];//encode
			if(check=='/'){
				name[i]=check;
			}
			else{
				for(j=0;j<strlen(plain);j++){
					if(name[i]==plain[j]){
						j=j+key;
						if(j<strlen(plain)&&j>=0){
							name[i]=plain[j];
						}
						else if(j>=strlen(plain)){
							j=j-strlen(plain);
							name[i]=plain[j];
						}
					}
				}
			}
		}
		sprintf(fpath, "%s%s",dirpath,name);
		//printf("%s\n",fpath);
	}
	struct timeval tv[2];
	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res =utimes(fpath,tv);
	if(res==-1)
		return -errno;

	return 0;
}

static struct fuse_operations a09_oper = {
	 .getattr    	= a09_getattr,
	// .access 		= a09_access,
	// .readlink	    = a09_readlink,
	 .readdir    	= a09_readdir,
	// .mknod  		= a09_mknod,
	 .mkdir		    = a09_mkdir,
	// .symlink    	= a09_symlink,
	 .unlink	    = a09_unlink,
	// .rmdir	    	= a09_rmdir,
	// .rename 		= a09_rename,
	// .link		    = a09_link,
	 .chmod	    	= a09_chmod,
	// .chown  		= a09_chown,
	 .truncate	    = a09_truncate,
	 .utimens    	= a09_utimens,
	// .open   		= a09_open,
	 .read		    = a09_read,
	 .write	    	= a09_write,
	// .statfs 		= a09_statfs,
	 .create         = a09_create,
	// .release    	= a09_release,
	// .fsync  		= a09_fsync,
  	// .setxattr	    = a09_setxattr,
	// .getxattr   	= a09_getxattr,
	// .listxattr	    = a09_listxattr,
	// .removexattr	= a09_removexattr,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &a09_oper, NULL);
}