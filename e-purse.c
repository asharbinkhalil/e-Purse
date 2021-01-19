#include<stdio.h>
#include<stdlib.h>
#include<string.h>

double current_income=0;
double currentexpense=0;

struct node
{	
	char date[40];
	double amount;
	char name[60];
	struct node *next;
}*income=NULL,*expense=NULL;

struct record
{
	double x,y;
}*point=NULL;


void create(char x[],double y,char z[],struct node **temp);
void display(int a3);
void writeincome(struct node *ptr);
void writeexpense(struct node *ptr);
void deleterecord(struct node *ptr);
void write(struct record *point);
struct record *readrecord();
struct node *readincome(struct node *ptr);
struct node *readexpense(struct node *ptr);
struct node *readnext(struct node *ptr,FILE *fpointer);

int main()
{
	int choice;
	double b; // b is used for new amount
	char c[60],a[40]; //c is used for name ;; a is used for date month year
	char s1[15],s2[15],s3[15]; // s1 s2 s3 are used for taking input of dd:mm:yyyy then appending it into a
	
	if(fopen("Record.bin","rb") != NULL) 
	{
		point=readrecord();
		current_income=point->x;
		currentexpense=point->y;
	}

	if(fopen("amountreturned.bin","rb") != NULL)
	{
		income=readincome(income);
	}
	if(fopen("amountgiven.bin","rb")!=NULL)
	{
		expense=readexpense(expense);
	}
	do
	{
		printf("                                 TOTAL AMOUNT RETURNED   	=     	Rs. %.2lf       \n ",current_income);      
		printf("                                TOTAL AMOUNT GIVEN  		=     	Rs. %.2lf      \n ",currentexpense);
		printf("                                CURRENT BALANCE  		=       Rs. %.2lf      \n\n",current_income-currentexpense);
		printf("->ENTER ANY OPTION (1-5) \n\n");
		printf("1.ENTER AMOUNT RETURNED\n");
		printf("2.ENTER AMOUNT GIVEN\n");
		printf("3.DISPLAY AMOUNT RETURN RECORD \n");
		printf("4.DISPLAY AMOUNT GIVEN RECORD \n");
		printf("5.EXIT\n");
		scanf("%d",&choice);
		printf("\n\n\n");
		switch(choice)
		{
			case 1:
			printf("**************   ADD RETURNED AMOUNT    *****************\n\n");
			printf("Enter The Date (e.g 15 Nov 2020)\n");
			scanf("%s %s %s",s1,s2,s3);//gets,fgets were not working thats why these steps have been used 
			  strcpy(a,"");
			  strcat(a,s1);
			  strcat(a," ");
			  strcat(a,s2);
			  strcat(a," ");
			  strcat(a,s3);
			printf("Enter The Amount\n");
			scanf("%lf",&b);
			printf("Returned by (NAME)\n");
			scanf("%s",c);
			current_income=current_income+b;
			create(a,b,c,&income); //saving data into structure
			writeincome(income); // saving data into file
			break;
			
			case 2:
			printf("**************    ADD GIVEN AMOUNT   *****************\n\n");
			printf("Enter The Date (e.g 16 Nov 2020)\n");
			scanf("%s %s %s",s1,s2,s3);
			  strcpy(a,"");
			  strcat(a,s1);
			  strcat(a," ");
			  strcat(a,s2);
			  strcat(a," ");
			  strcat(a,s3);
			printf("Enter Amount\n");
			scanf("%lf",&b);
			printf("Given by (NAME)\n");
			scanf("%s",c);
			currentexpense=currentexpense+b;
			create(a,b,c,&expense); //saving data into structure
			writeexpense(expense); // saving data into file
			break;

			case 3:
			printf("*********   AMOUNT RETURNED RECORD IS    *******\n\n");
			display(3);
			break;

			case 4:
			printf("*********   AMOUNT GIVEN RECORD IS   *******\n\n");
			display(4);
			break;

			case 5:
			point=(struct record*)malloc(sizeof(struct record));
			point->x=current_income;
			point->y=currentexpense;
			write(point);
			break;
			default:
			printf("WRONG OPTION SELECTED - \"Enter Valid Option");
			break;
		}
	}while(choice!=5);
return 0;
}
struct record *readrecord()
{
	FILE *fpointer;
	fpointer=fopen("Record.bin","rb");
	struct record *ptr=NULL;
	if(fpointer!=NULL)
	{
		fseek(fpointer,0,SEEK_SET);
		ptr=malloc(sizeof(struct record));
		fread(ptr,sizeof(struct record),1,fpointer);
	}
	else
		printf("CANNOT OPEN FILE\n");
return ptr;
}
struct node *readincome(struct node *ptr)
{
	int i;
	FILE *fpointer;
	fpointer=fopen("amountreturned.bin","rb");
	if(fpointer!=NULL)
	{
		deleterecord(ptr);
		ptr=NULL;
		fseek(fpointer,0,SEEK_END);
		long filesize=ftell(fpointer);
		rewind(fpointer);
		int entries=(int)(filesize/(sizeof(struct node)));
		for(i=0;i<entries;i++)
		{
			fseek(fpointer,(sizeof(struct node)*i),SEEK_SET);
			ptr=readnext(ptr,fpointer);
		}
	}
	else
		printf("ERROR IN OPENINNG FILE\n");
return ptr;
}

struct node *readexpense(struct node *ptr)
{
	int i; 
	FILE *fpointer;
	fpointer=fopen("amountgiven.bin","rb");
	if(fpointer!=NULL)
	{
		deleterecord(ptr);
		ptr=NULL;
		fseek(fpointer,0,SEEK_END);
		long filesize=ftell(fpointer);
		rewind(fpointer);
		int entries=(int)(filesize/(sizeof(struct node)));
		for(i=0;i<entries;i++)
		{
			fseek(fpointer,(sizeof(struct node)*i),SEEK_SET);
			ptr=readnext(ptr,fpointer);
		}	
	}
	else
		printf("cannonot open file\n");
return ptr;
}

struct node *readnext(struct node *ptr,FILE *fpointer)
{
	if(ptr==NULL)
	{
		ptr=(struct node *)malloc(sizeof(struct node));
		fread(ptr,sizeof(struct node),1,fpointer);
		ptr->next=NULL;
	}
	else
	{
		struct node *ptr1=ptr;
		struct node *ptr2=(struct node *)malloc(sizeof(struct node));
		while(ptr1->next!=NULL)
		{
			ptr1=ptr1->next;
		}
		fread(ptr2,sizeof(struct node),1,fpointer);
		ptr1->next=ptr2;
		ptr2->next=NULL;
	}
return ptr;
}
void create(char x[],double y,char z[],struct node **temp)        /* it isokay to write *temp if we pass only income rather than &income*/
{
	struct node *newnode,*ptr;
	newnode=(struct node*)malloc(sizeof(struct node));
	if(*temp==NULL)
	{
		strcpy(newnode->date,x);
		newnode->amount=y;
		strcpy(newnode->name,z);
		newnode->next=NULL;
		*temp=newnode;
	}
	else
	{
		ptr=*temp;
		while(ptr->next!=NULL)
		{
			ptr=ptr->next;
		}
		strcpy(newnode->date,x);
		newnode->amount=y;
		strcpy(newnode->name,z);
		newnode->next=NULL;
		ptr->next=newnode;
	}
}

void deleterecord(struct node *ptr)
{
	struct node *freeme =ptr;
	struct node *holdme=NULL;
	while(freeme!=NULL)
	{
		holdme=freeme->next;
		free(freeme);
		freeme=holdme;
	}
}



void display(int a3)
{
   if(a3==3)//if case 3 is executed, we have to print amount returned record
    {
    	if(fopen("amountreturned.bin","rb")==NULL)
        	printf("NO RECORDS AVAILABLE\n\n");    
        else
        { 
        	struct node *ptr2=income;
            while(ptr2!=NULL)
            {
            	printf("Date: %s\nAmount: Rs. %.2lf\nName: %s\n\n",ptr2->date,ptr2->amount,ptr2->name);
                ptr2=ptr2->next;
            }
    	}
	} 
	else if(a3==4)//if case 4 is executed then we have to print expenese record
	{
   		if(fopen("amountgiven.bin","rb")==NULL)
        {
            printf("NO RECORDS AVAILABLE\n\n");
        }
        else 
        { 
			struct node *ptr2=expense;
            while(ptr2!=NULL)
            {
            	printf("Date: %s\nAmount: Rs. %.2lf \nName: %s\n\n",ptr2->date,ptr2->amount,ptr2->name);
				ptr2=ptr2->next;
            }
        } 
	} 
}

void writeincome(struct node *ptr) //function for writing data in to file
{
	FILE *fpointer;
	fpointer=fopen("amountreturned.bin","wb");    
	if(fpointer!=NULL)
	{
		struct node *ptr1=ptr;
		struct node *holdnext=NULL;
		while(ptr1!=NULL)
		{
			holdnext=ptr1->next;
			ptr1->next=NULL;
			fseek(fpointer,0,SEEK_END);
			fwrite(ptr1,sizeof(struct node),1,fpointer);
			ptr1->next=holdnext;
			holdnext=NULL;
			ptr1=ptr1->next;
		}
		fclose(fpointer);
		fpointer=NULL;
		printf("\nAMOUNT RETURNED SAVED SUCCESSFULLY\n\n");
	}
	else
		printf("\nCANNOT SAVE INCOME..TRY AGAIN\n");
}

void writeexpense(struct node *ptr)
{
	FILE *fpointer;
	fpointer=fopen("amountgiven.bin","wb");    
	if(fpointer!=NULL)
	{
		struct node *ptr1=ptr;
		struct node *holdnext=NULL;
		while(ptr1!=NULL)
		{
			holdnext=ptr1->next;
			ptr1->next=NULL;
			fseek(fpointer,0,SEEK_END);
			fwrite(ptr1,sizeof(struct node),1,fpointer);
			ptr1->next=holdnext;
			holdnext=NULL;
			ptr1=ptr1->next;
		}
		fclose(fpointer);
		fpointer=NULL;
		printf("\nAMOUNT GIVEN ADDED SUCCESSFULLY\n\n");
	}
	else
		printf("\nCANNOT SAVE EXPENSE..TRY AGAIN\n\n");
}



void write(struct record *point)
{
	FILE *fpointer;
	fpointer=fopen("Record.bin","wb");    
	if(fpointer!=NULL)
	{
	fseek(fpointer,0,SEEK_END);
	fwrite(point,sizeof(struct record),1,fpointer);
	}
	else
		printf("FILEOPEN ERROR\n");
fclose(fpointer);
fpointer=NULL;
}


