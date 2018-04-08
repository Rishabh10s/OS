#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
int *avail;		// no. of available resources.
int **allocated;		//process and resources 
int **max;
int **need;
int *done;
int *temp;
int processes=0,resources=0,flag=0;
sem_t s;
pthread_mutex_t allocate;
pthread_mutex_t available;				
pthread_mutex_t mut;
pthread_cond_t cond;
void *alloc(void *);
int check(int);
void fun1(int);
//void complete();
void initialise();
int checkcomplete();

int checkcomplete(){
flag=0;
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

void *alloc(void *a){
	int p=(int)a;
		fun1(p);
		
		if(check(p)==0){
			printf("THIS RESULT IN UNSAFE STATE\n");
			pthread_exit(NULL);
		//	exit(0);
					
		}
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
		printf("Resource Allocated to process %d\n",p+1);
		for(int i=0;i<resources;i++){		
			printf("now Available resource %d is :%d\n",i+1,avail[i]);
		}
		for(int i=0;i<resources;i++){		
			printf("Total Allocated resource %d is :%d\n",i+1,allocated[p][i]);
		}
		//complete(p);		
			for(int i=0;i<resources;i++){
			avail[i]+=allocated[p][i];		
			//printf("%d\n",avail[i]);		
			}

		printf("Process %d complete, Resources given back to available\n",p+1);
		for(int i=0;i<resources;i++){		
			printf("Now Available resource %d is :%d\n",i+1,avail[i]);
		}
		/*for(int i=0;i<resources;i++){
			allocated[p][i]=0;		
		}*/			
		done[p]=1;		
		temp[p]=0;		
		pthread_mutex_unlock(&available);
		
		sem_post(&s);
		if(checkcomplete()==1){
			printf("\nALL PROCESSES COMPLETED");		
		}
		pthread_exit(NULL);
}
	



void fun1(int a){
		int p=a;
		if(check(p)==0){
		
			if(temp[p]==0){
				temp[p]=1;					
				sem_wait(&s);
				sleep(1);	
						
			}
			
		}
		
		
		
}

int check(int a){
	int p=a;
	for( int i=0;i<resources;i++){
	if(need[p][i]<=avail[i]){
	
	flag=1;
	}
	else{	
	flag=0;
	return flag;	
	}
	}
	return flag;	
}
/*
void complete(int p){
	printf("completed %d\n",p+1);
}
*/
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

	
	temp=malloc(processes*(sizeof(int)));
	for(int i=0;i<processes;i++){
		temp[i]=0;
	}
	
	

	
}

int main(){
int i,j;
printf("enter no. of total processes\n");
scanf("%d",&processes);
printf("enter total no. of resources\n");
scanf("%d",&resources);

initialise();
sem_init(&s,0,1);
pthread_t t[processes];


for(i=0;i<processes;i++){

pthread_create(&t[i],NULL,alloc,(void*)i);


}

for(i=0;i<processes;i++){
	
	pthread_join(t[i],NULL);

}

}


