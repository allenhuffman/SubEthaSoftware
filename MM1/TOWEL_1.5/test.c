#include <stdio.h>
#include <modes.h>

char *dirbuf;
char **filename;
long *filestart;
char *fileflag;

union FILESTART {
	char pad[4];
	long start;
} tmp;

main()
{
	int i,j,dpath;
	int dirsize;
	char path[41];

	while(1) {
		puts("enter path");
		readln(0,path,40);
		chdir(path);

		dpath=open(".",S_IREAD | S_IFDIR);
		printf("disk path is %d\n",dpath);
		if (dpath==NULL) {
			puts("unable to open directroy");
			exit();
		}
		dirsize=_gs_size(dpath);
		printf("size of directory is %d\n",dirsize);

		printf("allocating %d bytes\n",dirsize);
		dirbuf=(char*)malloc(dirsize);
		if (dirbuf==NULL) {
			printf("unable to malloc\n");
			close(dpath);
			exit();
		}
		i=read(dpath,dirbuf,dirsize);
		if (i==-1) {
			puts("well, I wasn't able to read anything for some reason...");
			close(dpath);
			exit();
		}
		printf("just read %d of %d bytes into dirbuf\n",i,dirsize);
		close(dpath);
		dirsize=dirsize/32;
		printf("directory entries: %d\n",dirsize);
		filename=(char **)malloc(sizeof(char *) * dirsize);
		if (filename==NULL) {
			puts("unable to allocate filename variable\n");
			exit(0);
		}
		printf("allocated %d filename entries\n",dirsize);
		fileflag=(char *)malloc(sizeof(char) * dirsize);
		if (fileflag==NULL) {
			puts("unable to allocate fileflag variable\n");
			exit(0);
		}
		printf("allocated fileflag entries\n");
		filestart=(long *)malloc(sizeof(long) * dirsize);
		if (filestart==NULL) {
			puts("unable to allocate filestart longs\n");
			exit(0);
		}
		printf("allocated filestart longs...maybe\n");
		for (i=0; i<dirsize; i++) {
			filename[i]=dirbuf+i*32;
			fileflag[i]=42;
			tmp.pad[0]=0; /* brute force but it works... */
			tmp.pad[1]=dirbuf[i*32+29]; /* point it to the fileID thing? */
			tmp.pad[2]=dirbuf[i*32+30];
			tmp.pad[3]=dirbuf[i*32+31];
			filestart[i]=tmp.start;
			for (j=0; j<27; j++) {
				if (filename[i][j] & 128) { /* if high bit set */
					filename[i][j]=filename[i][j]-128;
					filename[i][j+1]=0;
				}
			}
			if (filename[i][0]!=NULL) {
				printf("%-20.20s (fid -> %x)\n",filename[i],filestart[i]);
			}
		}
		free(dirbuf);
		free(filename);
		free(fileflag);
		free(filestart);
	}
}
