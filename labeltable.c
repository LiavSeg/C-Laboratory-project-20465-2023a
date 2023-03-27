#include "label_table.h"
/*
 * Function to add a label to the label table
 * @param head pointer to the head of the label table
 * @param name name of the label to add
 * @param type type of the label to add (ST, DT, EX, EN)
 * @param address address of the label to add
 * @param value value of the label to add (for ST and DT types)
 * @return a pointer to the head of the updated label table
 * Implemented by a linked list of structs
 */
label_table *add_label(label_table *head, char *name, char *type, int address, int value) {
	label_table *temp;
	label_table *new_label;
	int flag = find_label(head,name); /*searcing its a defined label*/  
	
	/*error handle -  ignoring any labels before enrty or extern */
	 if(!strcmp(type,"EX")||!strcmp(type,"EN")){
		 printf("********warning: Label found prior to %s guidance type [label ignored]\n",type); 
		 return head;
		 }	
    
    /*error handle -  a label already exists*/			
	if(flag>=0){
		printf("****Error:Label %s is already found ",name);
		return head;
	}
	else	
  		new_label=(label_table *)malloc(sizeof(label_table));
    if(!new_label){
        printf("*****Error: memory allocation failed\n");
        exit(0);
   	 }
    
    /*if its a label prior string or data add their value(DC) to the new label */
    
    	new_label->value = value;
    

    
    /*init new label */				
    strcpy(new_label->name,name);
    strcpy(new_label->type,type);
    new_label->address = AD_POS+address;   
    new_label->next = NULL;
   
    /*Empty table case*/
    if(!head){ 
    	new_label->address = AD_POS; 	   
        return new_label;
    }
    
    /*placing new label */
    temp = head;
    while(head->next)
    	head=head->next;
    head->next = new_label;
   	head = temp;
    return head;
	}

/*Finds a label if it exists return its address*/
int find_label(label_table *head, char *name){
	label_table *temp;
	temp = head;
	while(temp){
 		if(!strcmp(temp->name,name)){
   			return temp->address;
    	}
  		temp = temp->next;
   }
	return -1;
}		


/*Gets a specific label and its details*/
label_table *get_label(label_table *head, char *name){
	label_table *temp;
	temp = head;
	while(temp){
 		if(!strcmp(temp->name,name)){
   			return temp;
    	}
  		temp = temp->next;
   	}
	return NULL;
}		

/*Tags a label with a matching type and returns its address*/
int mark_label(label_table *head,label_table *label){
	label_table *temp;
	temp = head;
	while(temp){
    	if(!strcmp(temp->name,label->name)){
   			strcpy(temp->type,label->type);
   			return temp->address;
    	 }
  		temp = temp->next;
	}
	return -1;
}		

/*Free any dynamically alocted memory for the label table*/
void free_label_table(label_table *head){
	label_table *temp;
	while(head){
	 	temp = head;
    head = head->next;
    free(temp);
    }
}
/*Print func*/	
void print_labels(label_table *head){
	label_table *temp = head;
	printf("\nLabel table\n-----------");
    while(temp){
        printf("\n*|%d|%s|Type:%s|Value:%d|*",temp->address,temp->name,temp->type,temp->value);
        temp = temp->next;
    }
	printf("\n-----------\n");
}

/*Adding/creating new enrty&extern table same as above*/
label_table *add_entx(label_table *head, char *name, char *type){
	label_table *temp;
  	label_table *new_label;
	 new_label=(label_table *)malloc(sizeof(label_table));
     if(!new_label){
     	printf("*****Error: memory allocation failed\n");
        exit(0);
   	 }	
    strcpy(new_label->name,name);
    strcpy(new_label->type,type);
    new_label->address = 0;   
    new_label->value = 0;
    new_label->next = NULL;
    if(!head){ /*Empty table case*/
    	new_label->address = 0; 	   
        return new_label;
    }
    temp = head;
    while(head->next)
    	head=head->next;
    	head->next = new_label;
   		head = temp;
    	return head;
	}
