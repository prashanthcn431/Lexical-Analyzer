#include "header.h"
extern int error_flag;
/*
   Declare all keywords,operators,symbols and special characters and delimiters in seperate arrays
 */
char* keyword[] = {"goto","char","int","float","double","break","continue","return","if","else","while","void","for","do","switch","case","default","typedef","struct","union","enum","extern","static","volatile","const","unsigned","signed","short","long","NULL","sizeof","auto","register"};

char operator[] = {'+','-','/','%','^','!','~','*',','};
char symbol[] = {'{','}','[',']'};
char special[] = {'(',')',';'};

char delim[] = {'\n','\t',' '};


void lexical_analyze(FILE* fptr)
{
	char line[150];								//take a temporary array to scan a line
	while(fscanf(fptr,"%[^\n]\n",line) != EOF)	//scan each line until end of file
	{
		if(line[0] == '#')								//if 1st character is # skip iteratiom
		{
			printf("\nStart with header file\n")
			continue;									//because it is a pre-processor directive
		}
		check_type(line);						//pass the whole line to a function and print 
	}
}
void check_type(char* line)
{
	char* temp_line = line;					//take a temporary line to store whole line
	char arr[50];

	// String literals Extraction 
	while(*temp_line)						//run loop till null character
	{
		if( strchr(delim,*temp_line) != NULL)			//if delimiter occured 
		{
			temp_line++;							//increment the address
			continue;									//skip that iteration
		}
		int i = 0;
		if( *temp_line == '"')						//if character is "
		{
			char ch;
			temp_line++;								//skip that character and store the string
			while(*temp_line && *temp_line != '"')			//run loop till reaching of "
			{
				if(*temp_line == '\\')							//check for escape sequence
				{
					ch = *(temp_line+1);
					if(ch != 'b' && ch != 'f' && ch != 'n' && ch != 'a' && ch != 'n' && ch != 'v' && ch != 't' && ch != '0')//if escape sequence is not among all those
					{
						printf("\nEscape sequence is not allowed as %c%c\n",*temp_line,ch);		//print error message
						error_flag = 1;										//make error flaga as 1
						temp_line++;										//increment line and continue
						continue;
					}

				}
				if(*temp_line == ')' || *temp_line == ';')					//in between string if ; or ) comes then it indicates missing of closing "
				{
					printf("\nERROR: closing of \" missing\n");				//print error 
					error_flag = 1;										//make error flag as 1
					break;											//break the loop

				}
				arr[i++] = *temp_line++;						//store the character into an array 
			}
			arr[i] = '\0';										//add null and print it is a string
			printf("%-20s is a string literal\n",arr);
			if(*temp_line == '"')
			{
				temp_line++;						//if current character is " skip iteration
			}
			continue;
		}

		// Character Constant Extraction

		if(*temp_line == 39)								//if current character is a single quote '
		{
			temp_line++;
			arr[i++] = *temp_line++;					//store the charactr in array
			if(*temp_line == 39)							//if next character is '
			{
				printf("\n%-20c is a character constant\n",arr[0]);		//print it is a character constant
				temp_line++;										//increment line and continue
				continue;
			}
			else
			{
				int flag = 0;								
				while(*temp_line != 39 && *temp_line)			//if not run loop till you find ' and null character
				{
					if(*temp_line == ';' || *temp_line == ')')		//in between if we find end of statement or )
					{
						flag = 1;										//make flag as 1
					}
					arr[i++] = *temp_line++;							//store o=in array
				}
				arr[i] = '\0';
				if(flag == 0)
				{
					printf("\nCan't assign %s into a character\n",arr);		//if flag is 0 print error message
					error_flag = 1;
				}
				else
				{
					printf("\nMissing of \' single quote\n"); 		//else print another error message
					error_flag = 1;
				}
				if(*temp_line == 39)
				{
					temp_line++;			//if current character is 39 increment and continue 
				}
				continue;
			}
		}
		if( is_operator(*temp_line) == 1)				//call the function to check character is operator
		{
			printf("%-20c is an operator\n",*temp_line);		//print it is a operator
			temp_line++;									//increment line and skip iteration
			continue;
		}
		else if(is_special(*temp_line) == 1)				//else if check character is special character
		{
			printf("%-20c is a special character\n",*temp_line);		//if true print message and skip iteration
			temp_line++;
			continue;
		}
		else if(is_assignment(*temp_line) == 1)					//check the character is a assignment operator
		{
			printf("%-20c is a assignment operator\n",*temp_line);		//if true print message and skip iteraion
			temp_line++;
			continue;
		}
		else if(is_symbol(*temp_line) == 1)					//check character is a symbol
		{
			printf("%-20c is a symbol\n",*temp_line);			//if true print message and skip iteration
			temp_line++;
			continue;
		}
		else if( *temp_line == '<' || *temp_line == '>')			//if any of these characters occurs
		{
			if( *(temp_line+1) == '=')									//chack next character is assignment operator
			{
				printf("\n%c%-19c is a relational operator\n",*temp_line,*(temp_line+1));		//print message
				temp_line++;			//increment address and continue
				temp_line++;
				continue;
			}
			else if( *temp_line == '<')		//else if check for shift operator
			{
				if(*(temp_line+1) == '<')
				{
					if(*(temp_line+2) == '=')						//check for <<=
					{
						printf("\n%c%c%-18c is an operator\n",*temp_line,*(temp_line+1),*(temp_line+2));//if ture print message ad continue
						temp_line++;
						temp_line++;
						temp_line++;
						continue;
					}
					else
					{
						printf("\n%c%-19c is a left shift operator\n",*(temp_line),*(temp_line+1));//print it is a left shift operator
						temp_line++;
						temp_line++;
						continue;
					}
				}
				else
				{
					printf("\n%-20c is a relational operator\n",*temp_line);		//print it is a relational operator 		
					temp_line++;
				}
			}
			else if(*temp_line == '>')
			{
				if(*(temp_line+1) == '>')
				{
					if(*(temp_line+2) == '=')				//check for right shift operator
					{
						printf("\n%c%c%-18c is an operator\n",*temp_line,*(temp_line+1),*(temp_line+2));
						temp_line++;
						temp_line++;
						temp_line++;
						continue;
					}
					else
					{
						printf("\n%c%-19c is a right shift operator\n",*(temp_line),*(temp_line+1));
						temp_line++;
						temp_line++;
						continue;
					}
				}
				else
				{
					printf("\n%-20c is a relational operator\n",*temp_line);
					temp_line++;
				}

			}
		}
		else if( *temp_line == '&')
		{
			if(*(temp_line+1) == '&')			/*logic to check bitwise AND and logical AND*/
			{
				printf("\n%c%-20c is a logcal operator\n",*temp_line,*(++temp_line));
				temp_line++;
				continue;
			}
			printf("\n%-20c is a bitwise operator\n",*temp_line);
			temp_line++;
		}
		else if( *temp_line == '|')				/*logic to find bitwise OR and logical OR*/
		{
			if(*(temp_line+1) == '|')
			{
				printf("\n%c%-20c is a logcal operator\n",*temp_line,*(++temp_line));
				temp_line++;
				continue;
			}
			printf("\n%-20c is a bitwise operator\n",*temp_line);
			temp_line++;
		}
/*if all above case fails copy the charcters intio an array till you find delimiters,digit,operators*/
		while( *temp_line && (!strchr(delim,*temp_line)) && ( !is_special(*temp_line)) && (!is_assignment(*temp_line)) && (!is_operator(*temp_line)) && (!is_symbol(*temp_line)))
		{
			arr[i++] = *temp_line++;	
		}
		arr[i] = '\0';
		if( strlen(arr) > 0)
		{
			/*check the given array contains keyword or digit or octal or binary or hexadecimal value*/
			if( is_keyword(arr) == 0 && is_digit(arr) == 0 && is_bin(arr) == 0 && is_octal(arr) == 0 && is_hexa(arr) ==0 )
			{
				/*if all above condition fails check for array*/
				/*logic to find the empty array*/
				if(*temp_line == '[')
				{
					char temp[50];
					i=strlen(arr);
					if(*(temp_line+1) == ']')
					{
						strcpy(temp,arr);
						temp[i++] = *temp_line++;
						temp[i++] = *temp_line++;
						temp[i] = '\0';
						printf("%-20s is an array\n",temp);
					}
					else
					{
						/*logic to find the array with size*/
						char num[20];
						strcpy(num,arr);
						num[strlen(arr)] = '[';
						num[i+1] = '\0';
						int i=0;
						temp_line++;
						while(*temp_line != ']' && *temp_line)
						{
							temp[i++] = *temp_line;
							temp_line++;
						}
						temp[i] = '\0';	
						if(is_digit(temp) == 1)			//check for size is digit
						{
							temp[i++] = ']';
							temp[i] = '\0';
							strcat(num,temp);
							printf("%-20s is an array\n",num);			//print it as array
							temp_line++;
						}
					}
				}
				else
				{
					printf("%-20s is an identifer\n",arr);			//if above condition fails print it is a keyword
				}
			}

		}
	}
}
/*run loop to find the given array is a keyword*/
/*in all cases if true return 1 else return 0*/
int is_keyword(char* token)
{
	for(int i = 0;i<32;i++)
	{
		if(strcmp(token,keyword[i]) == 0)
		{
			printf("%-20s is a keyword\n",token);
			return 1;
		}
	}
	return 0;
}
/*logic to check given array is a digit*/
int is_digit(char* token)
{
	if(token[0] == '0' && strlen(token) > 1)		//if 1st character is 0 and length is greater than 1 it represents octal value
	{
		return 0;

	}
	int flag = 0,count = 0,char_flag = 0,digit_flag = 0;
	for(int i = 0; i < strlen(token) ;i++)
	{
		if(token[i] == 'f' && flag)				//exception for float value character f
		{
			digit_flag++;

		}
		else if(  (token[i] >= '0' && token[i] <= '9'))
		{
			digit_flag++;
		}
		else if( ((token[i] >= 'a' && token[i] <= 'z' ) || (token[i] >= 'A' && token[i] <='Z')) && digit_flag)	//if digit comes first and characters come next
		{
			if( token[1] != 'b' && token[1] != 'x' )
			{
				char_flag = 1;
			}
		}
		else if(token[i] == '.')
		{
			flag = 1;				//if decimal point occures in between represents float value
			count++;
			digit_flag++;
		}
	}
	if(strlen(token) == digit_flag && flag == 0)
	{
		printf("%-20s is a integer constant\n",token);		//it is a integer constant
		return 1;
	}
	else if( strlen(token) == digit_flag && flag == 1 && count < 2)
	{
		printf("%-20s is a float constant\n",token);		//based on decimal point flag print it is a float value
		return 1;
	}
	else if( char_flag == 1 || count > 1)			//if character comes after digit 
	{
		printf("ERROR: can't initialize a number as %s\n",token);		//we can't initialize a variable like that
		error_flag = 1;
		return 1;
	}

	return 0;

}
/*logic to check given character is a operator*/
int is_operator(char token)
{
	for(int i = 0;i<9;i++)
	{
		if(token == operator[i])
			return 1;
	}
	return 0;

}
/*Logic to check the assignment operator*/
int is_assignment(char ch)
{
	if(ch == '=')
		return 1;
	return 0;
}
/*Logic to check the symbol*/
int is_symbol(char ch)
{
	for(int i=0;i<4;i++)
	{
		if(ch == symbol[i])
			return 1;
	}
	return 0;

}
/*Logic to check the special character*/
int is_special(char ch)
{
	for(int i=0;i<3;i++)
	{
		if(ch == special[i])
			return 1;
	}
	return 0;
}
/*Logic to find binary number*/
int is_bin(char* number)
{
	if(strncmp(number,"0b",2) == 0)		//check prefix for binary number as 0b
	{
		for(int i=2;i < strlen(number);i++)
		{
			if(number[i] != '0' && number[i] != '1')		//if any character other than 1 and 0 comes print error
			{
				printf("Can't initialize binary number as %s\n",number);
				error_flag = 1;
				return 1;
			}
		}
		printf("%s is a binary number\n",number);
		return 1;

	}
	return 0;
}
/*Logic to check the octal value*/
int is_octal(char* number)
{
	if(number[0] == '0' &&  number[1] != 'x')		//if 0th index is 0 and 1st index is not x
	{
		for(int i=0;i<strlen(number);i++)
		{
			//check condition to be 0 to 7 and not any characters between a to z
			if( (!(number[i] >= '0' && number[i] <= '7')) || (number[i] >= 'a' && number[i] <= 'z') || (number[i] >='A' && number[i] <= 'Z'))
			{
				printf("ERROR: can't initialize octal value as %s\n",number);
				error_flag = 1;
				return 1;
			}
		}
		printf("%-20s is an octal value\n",number);
		return 1;
	}
	return 0;
}
/*Logic to check the hexadecimal value*/
 int is_hexa(char* number)
{
	if(strncmp(number,"0x",2) == 0)		//first 2 characters should be 0x
	{
		for(int i = 2;i<strlen(number);i++)
		{
			if( (!(number[i] >= '0' && number[i] <= '9')) && (!(number[i] >= 'A' && number[i] <= 'F')) && (!(number[i] >='a' && number[i] <= 'f')))
			{		//check condition that characters shouls be a-f and 0 to 9
				printf("ERROR: can't initialize %s as hexadecimal value\n",number);
				error_flag = 1;
				return 1;
			}
		}
		printf("%-20s is a hexadecimal value\n",number);
		return 1;
	}
	return 0;
}
