#include <stdio.h>

char *item[100]; /* pointer to item data */
char *data; /* pointer to the actual data area */

main()
{
	int i;
	char *strcmp();

	data=(char *)malloc(100*sizeof(char)*20);
	
	for (i=0; i<100; i++) {
		item[i]=&data[100*i];
		printf("%d - %p\n",i,item[i]);
		sprintf(item[i],"item %d",i);
	}

	qsort(item,100,sizeof(char *),strcmp);

	for (i=0; i<100; i++) printf("new %s\n",item[i]);

	free(data);
}

