#include<stdio.h>
#include<pthread.h>

int *avail;		// no. of available resources.
int **allocated;		//process and resources
int **max;
int **need;

int processes=0,resources=0;

pthread_mutex_t allocate;
pthread_mutex_t available;				



void *alloc(void *a){
	int p=(int *)a;
	if(check(p)==1){
		
		for(int i=0;i<resources;i++){
	
		pthread_mutex_lock(&allocate);
		allocated[p][i]+=need[p][i];
		
		pthread_mutex_unlock(&allocate);
		pthread_mutex_lock(&available);		
		avail[i]-=need[p][i];
		pthread_mutex_unlock(&available);
		printf("Resouce %d allocated= %d\n",i+1,allocated[p][i]);
		printf("Available resource %d :%d\n",i+1,avail[i]);	
		

		}

		for(int i=0;i<3;i++){
		avail[i]+=allocated[p][i];		
		}		
		
	}
	else
	{
	printf("unsafe");	
	}
}

int check(int a){
	int p=a;
	if(need[p][0]<=avail[0] && need[p][1]<=avail[1] && need[p][2]<=avail[2]){
	
	return 1;
	}
	else{
	return 0;	
	}
		
}


void initialise(){
	avail=malloc(resources*sizeof(int));
	for(int i=0;i<resources;i++){
		printf("Enter available resource %d\n",i+1);
		scanf("%d",&avail[i]);
	}

	max=malloc(processes*(sizeof(int)));
	for(int i=0;i<processes;i++){
		max[i]=malloc(resources*sizeof(int));
	}
	
	allocated=malloc(processes*(sizeof(int)));
	for(int i=0;i<processes;i++){
		allocated[i]=malloc(resources*sizeof(int));
	}
	

	need=malloc(processes*(sizeof(int)));
	for(int i=0;i<processes;i++){
		need[i]=malloc(resources*sizeof(int));
	}

	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			printf("enter no. of allocated resource %d for process %d ",j,i);
			scanf("%d",&allocated[i][j]);
		}
	
	}
	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			printf("enter max resource %d for process %d ",j,i);
			scanf("%d",&max[i][j]);
		}
	
	}

	
	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			need[i][j]=max[i][j]-allocated[i][j];			
		}
	
	}
	
	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			printf("resource %d needed for process %d is %d",j,i,need[i][j]);
			
		}
	
	}

	
	

	
}

int main(){
int i,j;
printf("enter no. of total processes\n");
scanf("%d",&processes);
printf("enter total no. of resources\n");
scanf("%d",&resources);

initialise();

pthread_t t[processes];
for(i=0;i<processes;i++){

pthread_create(&t[i],NULL,alloc,(void*)i);

}
for(i=0;i<processes;i++){
pthread_join(t[i],NULL);
}

}
