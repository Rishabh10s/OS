#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
int *avail;		// no. of available resources.
int **allocated;		//process and resources 
int **max;
int **need;
int *done;
int *temp,sig;
sigset_t sst; 
int processes=0,resources=0,flag=0;

pthread_mutex_t allocate;
pthread_mutex_t available;				
pthread_mutex_t mut;
pthread_cond_t cond;
void *alloc(void *);
int check(int);
void complete();
void initialise();
int checkcomplete();
/*
int checkcomplete(){
for(int i=0;i<processes;i++){
	if(done[i]==1){
	flag=1;
	}
	else{
	flag=0;
	return 0;	
	}
}

return flag;
}
*/
void *alloc(void *a){
	int p=(int)a;
	if(check(p)==1){
		
		for(int i=0;i<resources;i++){
		//pthread_mutex_lock(&mut);		
		pthread_mutex_lock(&allocate);
		allocated[p][i]+=need[p][i];
		
		pthread_mutex_unlock(&allocate);
		pthread_mutex_lock(&available);		
		avail[i]-=need[p][i];
		
		pthread_mutex_unlock(&available);
		//printf("Resouce %d allocated= %d for process%d\n",i+1,allocated[p][i],);
		//printf("Available resource %d :%d\n",i+1,avail[i]);	
		

		}
		printf("         PROCESS %d        \n",p+1);
		
		pthread_mutex_lock(&available);
		printf("resouce taken from avail ,now avail \nres1:%d \nres2:%d \nres3:%d\n",avail[0],avail[1],avail[2]);
		printf("resource allocated to %d :is %d  %d  %d\n",p+1,allocated[p][0],allocated[p][1],allocated[p][2]);
		complete(p);		
		for(int i=0;i<resources;i++){
		avail[i]+=allocated[p][i];		
		//printf("%d\n",avail[i]);		
		}

		printf("resouce given to avail ,now avail \nres1:%d \nres2:%d \nres3:%d\n",avail[0],avail[1],avail[2]);		//make lloop.	
		done[p]=1;		
		pthread_mutex_unlock(&available);
		
		/*pthread_mutex_lock(&mut);
		pthread_cond_broadcast(&cond);		
		pthread_mutex_unlock(&mut);	
		*/
		//raise(sig);
		pthread_exit(NULL);
			}
	else
	{//sleep(2);
	int count=0;
/*	printf("%d",sig);
	printf("%d",sigwait(&sst,&sig));
	printf("%d",sig);
	*/
	
	
		for(int i=p;i<processes;i++){
			if(done[i]==0){
			alloc((void*)i);
			count++;
			}
			if(p=processes-1){
			i=0;		
			}
			if(count>p){
			break;		
			}
		
		}
	
	
			printf("no safe sequence possible");
		
	
	//printf("unsafe process %d\n",p+1);	
	
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

void complete(int p){
	printf("completed %d\n",p+1);
}

void initialise(){
	avail=malloc(resources*(sizeof(int)) );
	for(int i=0;i<resources;i++){
		printf("Enter available resource %d\n",i+1);
		scanf("%d",&avail[i]);
	}

	max=malloc(processes*(sizeof(int*)));
	for(int i=0;i<processes;i++){
		max[i]=malloc(resources*sizeof(int));
	}
	
	allocated=malloc(processes*(sizeof(int*)));
	for(int i=0;i<processes;i++){
		allocated[i]=malloc(resources*sizeof(int));
	}
	
	
	need=malloc(processes*(sizeof(int*)));
	for(int i=0;i<processes;i++){
		need[i]=malloc(resources*sizeof(int));
	}

	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			printf("enter no. of allocated resource %d for process %d ",j+1,i+1);
			scanf("%d",&allocated[i][j]);
		}
	
	}
	for(int i=0;i<processes;i++){
	
		for(int j=0;j<resources;j++){
			printf("enter max resource %d for process %d ",j+1,i+1);
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
			printf("resource %d needed for process %d is %d\n",j+1,i+1,need[i][j]);
			
		}
	
	}
	
	done=malloc(processes*(sizeof(int)));
	for(int i=0;i<processes;i++){
		done[i]=0;
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


