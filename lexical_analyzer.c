/*
Author : Pavan Kumar AS
Organisation : Emertxe Information Thechnologies (P) Ltd.
Date : 23/09/2025
Usage :
 */
#include "header.h"
int error_flag;									//declare a global variable error flag
int main(int argc,char* argv[])						//collect the command line arguments
{
	if(argc < 2)									//if sufficient command line arguments are not passed
	{
		printf("Usage: ./a.out <sample.c file>\n");		//print error and stop the process
		return 0;
	}
	char filename[50];
	strcpy(filename,argv[1]);						//if passed copy the file name
	if(!strstr(filename,".c"))						//if passed file is not a c file 
	{
		printf("Please pass <.c file>\n");			//print error and stop process
		return 0;
	}
	FILE* fptr = fopen(filename,"r");					//open the filename in read mode
	printf("Open : %s : ",filename);
	if(fptr)
	{
		printf("SUCCESS\n");						//if file is opened print success
	}
	else
	{
		printf("FAILED\n");								//if file is not present in curren directory print failure and stop
		return 0;
	}
	printf("Parsing : %s : Started\n",filename);

	lexical_analyze(fptr);								//pass the file pointer to function
	fclose(fptr);										//after completion of paesing close the file
	if(error_flag)
	{
		printf("Tokens cannot be generated.\n");		//based on error flag prin the output at last
	}
	else
	{
		printf("Tokens generated.\n");
	}
	return 0;




}
