#include "second_pass.h"
/** 
 * Performs the second pass of the assembly process, which includes encoding the
 * assembly code into unique machine code, generating entry and extern files, and creating
 * an output file with the encoded program.
 * @param after_macro_source: A string representing the name of the source file.
 * @param lt: A pointer to a label_table struct containing all defined label names.
 * @param entx: A pointer to a label_table struct containing all entry labels.
 * @param RAM: A pointer to an encoded_words struct where the encoded program will be stored.
 * @param general: An integer indicating the addressing method used in the program.
 * @param IC: An integer representing the Instruction Counter at the end of the first pass.
 * @param DC: An integer representing the Data Counter at the end of the first pass.
 */

void second_pass(char *after_macro_source,label_table *lt,label_table *entx,encoded_words *RAM,int general,int IC,int DC){
	char name1[sizeof(after_macro_source)+ENTX_POSTFIX];
	char name2[sizeof(after_macro_source)+ENTX_POSTFIX];
	FILE *ent=NULL;
	FILE *ext=NULL;
	/*Create the name of the entry/exterm file and open it*/
	strcpy(name1,after_macro_source);
	strcpy(name2,after_macro_source);
	sprintf(name1,"%s.ext",after_macro_source);
	sprintf(name2,"%s.ent",after_macro_source);
	ext = fopen(name1,"w");
	if(!ext){
		printf("********Error: Could not open extern file - program termination\n");
		free_label_table(lt);
		free_label_table(entx);
		 return;/*source file will be closed right after this termination*/
	}
 	ent=fopen(name2,"w");	
	if(!ent){
		printf("********Error: Could not open entry file - program termination\n");
		free_label_table(lt);
		free_label_table(entx);
		return;/*source file will be closed right after this termination*/
	}		
	/*Program fully encoded - procceed for the last stage*/
	if(second_pass_endcoding(RAM,lt,entx,ent,ext,general)){
		print_entry_file(lt,entx,ent);
		/*if were not any entry lables remove file */
		fseek(ent, 0L, SEEK_END);
   		if(!ftell(ent)){
       		remove(name2);
       	}
    	/*if were not any extern remove file if needed*/
    	fseek(ext, 0L, SEEK_END);
		if(!ftell(ext)){
       		remove(name1);
       	}
		/*Last stage - create an output with the file fully encoded*/
		unique_base_coding(RAM,after_macro_source,IC,DC,general);
		free_label_table(lt);
		free_label_table(entx);
		fclose(ext);
		fclose(ent);
		printf("***********Assembly process succeed for %s program ************\n\n",after_macro_source);
		printf("-----------------------------------------------------------------------------\n");		
					
	}
	/*Could not deliver "second pass encoding" program ends and closes neccesary files*/
	else{
       	remove(name1); 
       	remove(name2);
		fclose(ext);
		fclose(ent);
		free_label_table(lt);
		free_label_table(entx);
	}
	return;	
}			
				
				
/**
 * Encodes the commands and data in the RAM array during the second pass of the assembler
 * @param RAM An array of encoded_words containing the commands and data to be encoded
 * @param lt A pointer to a label_table containing the label information
 * @param entx A pointer to a label_table containing the .entry labels information
 * @param ent A pointer to an open FILE for the .ent output file
 * @param ext A pointer to an open FILE for the .ext output file
 * @param general An integer representing the total number of encoded_words in the RAM array
 * @return An integer representing the success of the function (1 for success, 0 for failure)
 */	
int second_pass_endcoding(encoded_words *RAM,label_table *lt,label_table *entx,FILE *ent,FILE *ext,int general){
	int i = 0;
	int temp;
	while(i<RAM_SIZE){
		while(RAM[i].encoding[0]!='*'&&i<=RAM_SIZE-1)
			i++;	
		if(i==RAM_SIZE)
			return 1;		
		memset(RAM[i].encoding,'0',WORD_SIZE);
		temp = entx_encode(RAM,lt,entx,ent,ext,i);
			if(!temp){
				temp = cmd_sdata_encode(RAM,lt,i);
				if(!temp){
					printf("********Error: label %s not found during second pass encoding ",RAM[i].command_name);
					printf("program termination \n\n");
					return 0;
				}
			}	
		}
	return 1;
}			
				
/* This function generates the final output file  */				
void unique_base_coding(encoded_words *RAM,char *fname,int IC,int DC,int general){
	FILE *fp;
	int j,i;
	char name[sizeof(fname)+NAME_POSTFIX];
	i=j=0;
	/*Create the name of the encoded  file and open it*/
	sprintf(name,"%s.ob",fname);
	fp = fopen(name,"w");
	if(!fp){
		printf("********Error: Could not open encoded file - program termination\n");
		return;
		}
	/*IC and DC headline*/
	fprintf(fp,"      %d %d\n",IC,DC);
	/* Iterate through the RAM array */
	while(i<general){
		for(j=0;j<WORD_SIZE;j++){
			if(RAM[i].encoding[j]=='0')/*zero get .*/
				RAM[i].encoding[j]='.';
			else if(RAM[i].encoding[j]=='1')/*one get / */
				RAM[i].encoding[j]='/';	
			}	
		i++;
		}
	print_ic_dc(RAM,fp,general);/*print first instructions and then guidance*/
	fclose(fp);/*Mission accomplished - closing file*/
}		
			
				
/* This function encodes data for a an entry/extern and if founded prints to a filelabel type)
 * @param RAM: a pointer to an array of encoded_words
 * @param lt: a pointer to a label_table struct
 * @param entx: a pointer to a label_table struct
 * @param ent: a pointer to a file stream for entry file
 * @param ext: a pointer to a file stream for extern file
 * @param i: an integer indicating the index of the current encoded_word in the RAM array
 * @return: 1 if encoding is successful, 0 otherwise
 */
 int entx_encode(encoded_words *RAM,label_table *lt,label_table *entx,FILE *ent,FILE *ext,int i){
	label_table *temp=NULL;
	int address;
	if((temp = get_label(entx,RAM[i].command_name))){
		/* If the label type is EX, set the ERA to 10 and encode the address */	
		if(!strcmp(temp->type,"EN")){
				ERA('1','0');/*ERA macro*/
				address = mark_label(lt,temp);
				NUMCODE(address);
				return 1;
				}
        	/* If the label type is EX, set the ERA to 01 and encode the address */			
			if(!strcmp(temp->type,"EX")){
				ERA('0','1');/*ERA macro*/
				address = RAM[i].location+ADDRESS_START;
				fprintf(ext,"%s %d\n",RAM[i].command_name,address);			
		 		return 1;
		 	}
	}			
	return 0;
}		
			
/* This function encodes data for a command  label type(before a cmd/st/data line) */				
int cmd_sdata_encode(encoded_words *RAM,label_table *lt,int i){			
	label_table *temp=NULL;
	int address;	
	 /*Check if the command name is a label in the label_table struct*/
	if((temp = get_label(lt,RAM[i].command_name))){
	/* If the label type is data/string, find the label in the label_table and copy the encoding*/
		if(!strcmp(temp->type,"DT")||!strcmp(temp->type,"ST")){
			ERA('1','0');/*ERA macro*/
			NUMCODE(temp->address);
			return 1;
			}
	   	/* If the label type is C, find the label in the label_table and copy the encoding */
		if(!strcmp(temp->type,"C")){
			address = find_label(lt,temp->name);
			LABELCODE(address);
			return 1;
			}		
		}
	/* command name isnt a label in the label_table struct - this func will be called last so if nothing founed 	found - invalid label*/		
	return 0;
	}			
/**
 * Prints the IC and DC tables to a given file,first instrucitons and then data as requested 
 * @param RAM An array of encoded_words containing the instructions and data to be printed
 * @param fp A pointer to the file to which the IC and DC tables will be printed
 * @param size The size of the RAM array
 */				
void print_ic_dc(encoded_words *RAM,FILE *fp,int size){
	int address = ADDRESS_START;
	int i;
	for(i = 0;i<size;i++)
		if(!strcmp(RAM[i].type,"C")){
			fprintf(fp,"%d%d %s\n",0,address,RAM[i].encoding);
			address++;
		}		
		
		for(i = 0; i<size;i++)
			if(strcmp(RAM[i].type,"C")){
				fprintf(fp,"%d%d %s \n",0,address,RAM[i].encoding);
				address++;
			}	
		}		
		
void print_entry_file(label_table *lt,label_table *entx,FILE *ent){
	label_table *head = entx;
	int address;
	
	while (head){
		address = find_label(lt,head->name);
		if(!strcmp(head->type,"EN"))
			fprintf(ent,"%s %d\n",head->name,address);
	head = head->next;
	}
}			
				
