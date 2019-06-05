#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

typedef struct node{
	struct node* next;
	int value;
}node;

typedef struct parameters{
	struct node* head;
	void (*callback)(node* value);
}parameters;


typedef void (*callback)(node* value);
pthread_mutex_t mutex;

void add(struct node* head, int val){
	
	struct node *new_node;
	struct node* temp = head;

	new_node = malloc(sizeof(struct node));
	new_node->value = val;
	new_node->next = NULL;
	
	while(temp->next != NULL){	
	temp = temp->next;	
	}
	
	temp->next = new_node;

}

void delete(struct node** head, int elem){
	
	struct node* temp = *head;
	
	struct node* prev = NULL;
	temp->value = &(*head)->value;
	if(temp != NULL && temp->value == elem){
		*head = temp->next;
		free(temp);
		return;	
	} 

	while(temp != NULL && temp->value != elem){
		prev = temp;
		temp = temp->next;	
	}

	if(temp == NULL){
		return;
	}
	prev->next = temp->next;
	free(temp);

}

void swap(struct node* elem1,  struct node* elem2){
	
	int temp = elem1->value;
	elem1->value = elem2->value;
	elem2->value = temp;
}

void sort_list(struct node* head){
	
	struct node* lptr = NULL;

	if(head == NULL)
		return;
	if(head->next->next == NULL){
		if(head->value > head->next->value)
			swap(head, head->next);
	}
	else{
	head = head->next;
	while(head != NULL){
		lptr = head->next;
		while(lptr != NULL){
			if(head->value > lptr->value){
				swap(head, lptr);	
		}
			lptr = lptr->next;	
		}
		head = head->next;	
	}
	}
	
}

void disp(struct node* head){
	
	struct node* temp = head;
	temp = temp->next;
	while(temp != NULL){
		printf("%d ", temp->value);
		temp = temp->next;	
	}
	printf("\n");
}

void print_list(struct node* head){
	
	struct node* temp = head;
	if(temp == NULL)
		return;
	while(temp != NULL){
		disp(temp);
		temp = temp->next;	
	}
	printf("\n");
}

void flush_list(struct node* head){
	
	struct node* current = head;
	print_list(current);
	while(head != NULL){
		current = head;
		head = head->next;
		free(current);
	}
}

void* f_thread1(void* params){
	
	pthread_mutex_lock(&mutex);
	struct parameters *p  = (struct parameters*) params;
	add(p->head, 2);
	add(p->head, 10);
	add(p->head, 4);
	delete(&p->head, 2);
	sort_list(p->head);
	delete(&p->head, 10);
	delete(&p->head, 5);
	pthread_mutex_unlock(&mutex);
}

void *f_thread2(void* params){
	
	pthread_mutex_lock(&mutex);
	struct parameters *p  = (struct parameters*) params;
	add(p->head, 11);
	add(p->head, 1);
	delete(&p->head, 11);
	add(p->head, 8);
	print_list(p->head);
	pthread_mutex_unlock(&mutex);
}

void* f_thread3(void* params){
	
	pthread_mutex_lock(&mutex);
	struct parameters *p  = (struct parameters*) params;
	add(p->head, 30);
	add(p->head, 25);
	add(p->head, 100);
	sort_list(p->head);
	delete(&p->head, 100);
	print_list(p->head);
	pthread_mutex_unlock(&mutex);
}

int main(){


	pthread_t t1, t2, t3;
	pthread_mutex_init(&mutex, NULL);

	if (pthread_mutex_init(&mutex, NULL) != 0){
		printf("Mutex failed\n");
		return 0;	
	}

	struct node* head = malloc(sizeof(struct node));
	head->next = NULL;
	
	callback funct = disp;
	
	struct parameters args1;
	args1.head = head;
	pthread_create(&t1, NULL, &f_thread1, &args1);
	pthread_create(&t2, NULL, &f_thread2, &args1);	
	pthread_create(&t3, NULL, &f_thread3, &args1);		
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_mutex_destroy(&mutex);
	flush_list(head);
	return 0;	

}
