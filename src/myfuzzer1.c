
/*
#########################################################################################################
#--Autor--		* Anicet SIEWE KOUETA																	#
#				* Regine MAKONGA																		#
#																										#
#--Descroption--																						#
#																										#
#				This program makes it possible to test the reliability of the converter tool :			#
#				* if the converter tool presents Bugs or crashe, this program will return "YES",		#
#				  and will enumerate the list of the values of bytes which allowed to crash the 		#
#				  tool converter																		#
#				* otherwise return "NO"																	#
#########################################################################################################
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
//********************************************************DEFINITION OF FUNCTIONS *************************


/*
@param:command
function "Execute_command" allows executing a command line (string of caracter) to parrameter.
and show the result back by the command
take 1 parameter, the full name of the command
*/
char  execute_command(char command[])
{
	FILE* F;
	char temp[UCHAR_MAX];
	F = popen(command, "r");

	if (F != NULL)
	{

		while (fgets(temp, sizeof temp, F) != NULL) {
			fputs(temp, stdout);
		}
		if (pclose(F) == -1) {
			printf("Could not execute the command \"%s\"\n", command);
		}
	}
	else
	{
		printf("Could not launch the command \"%s\"\n", command);
	}
}

/*
@param: filename
allows to fully display each act of the image pass in parameter.
take 1 parameter, the full name of the image in string of character
*/
void printFile(char fileName[]) {

	FILE *F;
	int c, nbr;
	nbr = 0;

	F = fopen(fileName, "rb");
	if (F != NULL)
	{
		while ((c = fgetc(F)) != EOF)
		{
			nbr++;
			printf("%c", (unsigned)(unsigned char)c);
		}
		//printf("\n");
		fclose(F);
	}
	else
	{
		printf("Could not open file \"%s\" \n", fileName);
	}
}

/*
@param: fileName, fileArray
storing each octect of the image in an array.
takes 2 parameters, the first is the full name of the image, the second is the array where it must be registered.
and returns the size size of the array in which the image is stoker
*/
int save_image(char fileName[], char fileArray[])
{
	FILE *F;
	int c, nbr;
	nbr = 0;
	F = fopen(fileName, "rb");
	if (F != NULL)
	{
		while ((c = fgetc(F)) != EOF)
		{
			fileArray[nbr] = (unsigned)(unsigned char)c;
			nbr++;
		}


		fclose(F);

		return nbr;
	}

	else
	{
		printf("\n Could not execute the programme.\n could not find the image file \"%s\" in this directory", fileName);
		return 0;
	}

}


/**
* @param: fileArray		: array containing image file content
*         size				: the new size of the output file
*         ouputFileName	: file where will be saved the **********

allows to create a file from the data contained in a table.
takes 3 parameters, the first one is the tab [] array, the second is 
the array tab size [], and the third is the output file name.

*/
void create_file_for_attack(char fileArray[], int newSize, char ouputFileName[]) {

	FILE *F;
	F = fopen(ouputFileName, "wb");
	if (F != NULL)
	{
		fwrite(fileArray, 1, newSize, F);
		fclose(F);
	}

	else
	{
		printf("Could not open \"%s\" \n", ouputFileName);
	}
}


/**
* @param: fileArray : array containing the image file
*         fileName  : ouput file
*         testNumber: test identifier
*         duration  : test duration
*         position  : byte position
*         newValue  : new value of the byte located at position "position"
*         nber_of_changed_values: number of modified values
*
*create a log file containing all the bytes edit and their values, during each test.
*these files are stoker in /out/FileLog/
*
*
*/
void create_log_file(char fileArray[], char fileName[], int testNumber, int duration, int Position[], int NewValue[], int nber_of_changed_values, char Message[]) {
	FILE *F;
	time_t hour = time(NULL);
	char date[20];
	strftime(date, sizeof date, "%d/%m/%Y - %H:%M", localtime(&hour));
	F = fopen(fileName, "a");

	if (F != NULL)
	{
		int i;
		fprintf(F, "Test N° %d. DateTime: %s, Duration = %d ms .changes made on : (octect_N°,value); \nMessage return : %s \n", testNumber, date, duration, Message);

		for (i = 0; i < nber_of_changed_values; i++)
		{
			fprintf(F, "(%d,%d);", Position[i], NewValue[i]);
		}
		fprintf(F, "\n");
		fprintf(F, "\n\n\n");
		fclose(F);
	}

	else
	{
		printf("Could not open file \"%s\" \n", fileName);
	}
}


/*
@param: min, max
@pre: max > min
Generates a random number in the interval between min and max values *
*/
int random_number_generator(int min, int max) {

	return rand()%(max - min)+min;

}


/*
@param: argv: command line argument
@param: Message: Personnal message
Prints error message
*/
int print_error_message(char argv[], char Message[]) {

	printf("\n");
	printf("\n===================== Groupe ==========================");
	printf("\n---                                                 ---");
	printf("\n--- 	Invalid command \"%s\"		    ---", argv);
	printf("\n--- 	%41s   ---", Message);
	printf("\n---                                                 ---");
	printf("\n=======================================================");
	printf("\n---      The command should contain 4 parameters:   ---");
	printf("\n---                                                 ---");
	printf("\n---	1. Input file of \".img\" extension           ---");
	printf("\n---	2. Number of tests > 0 			    ---");
	printf("\n---	3. Number value modifie > 0 		    ---");
	printf("\n---	4. Percentage between [0,1]		    ---");
	printf("\n=======================================================");
	printf("\n");
	return (EXIT_FAILURE);
}


/*
@param: filename		:
@param: author			:
@param:	size			:
@param: percentage		:
@param: nber_of_tests	:
@param: duration		:
@param:	BURGE			:

Prints the result of the test and some image information

Prints error message
*/
void print_result(char *filename, char *author, int size, int percentage, int Prcent, int nber_of_tests, int duration, char BURGE[]) {

	printf("\n");
	printf("\n===================== Groupe ========================");
	printf("\n---                                                 ---");
	printf("\n---	Input file       : %15s	    ---", filename);
	printf("\n---	Autor      	 : %6s		    ---", author);
	printf("\n---	Image Size       : %4d			    ---", size);
	printf("\n---	Modif percentage : %3d */* (~%4d)	    ---", percentage, Prcent);
	printf("\n---	Number of tests  : %5d		    ---", nber_of_tests);
	printf("\n---	Duration         : %10d ms	    ---", duration);
	printf("\n---	Bugs             : %3s			    ---", BURGE);
	printf("\n---                                                 ---");
	printf("\n=======================================================");
    printf("\n---                                                 ---");
	printf("\n---       ##*List test of crash Attempts*##         ---");
	printf("\n---                                                 ---\n");
	
	
}



/*
@param: Namefile	: file name
@param: tablefile	: table name

takes the message in a file, and store it in an array.
*/
void Save_Error_message (char Namefile[], char tablefile[]) {
    
    FILE *F;
    int c, nbr ;
    nbr = 0 ;
    F = fopen(Namefile, "rb") ;

    if (F != NULL)  
    {
        while((c=fgetc(F)) !=EOF) 			//until we are the end of the file EOF = End_of_File
        {
            tablefile[nbr] = (unsigned)(unsigned char)c ;       
            nbr++ ; 
        }
    }
     
    else 
    {
        printf("IMPOSIBLE D'OUVRIR LE FICHIER \"%s\" \n",Namefile);
    } 
}


/*
Checks if a string or not
*/
int is_directory (char *Str) {
	int i, leng ;
	int type = 1 ;
	leng = (strlen(Str)) ;
	for (int i=0; i<leng ; i++){
		if (isalpha(Str[i]) == 0) {
			type = 0 ;
		}
	}
	return type ;
}


/*
Checks if a digit or not
*/
int is_digit(char *Str) {

	int i, leng;
	int type = 1;
	leng = (strlen(Str));
	for (i = 0; i<leng; i++) {
		if (isdigit(Str[i]) == 0) {
			type = 0;
			return type;
		}
	}
	return type;
}

/*
Checks if a float or not
*/
int is_float(float Num) {

	int i = ceil(Num);
	int type = 1;

		if (i == Num) {
			type = 0;
			return type;
		}
	return type;
}

/************************************************** MAIN *****************************************************************/

 int main(int argc, char *argv[])
{
	
	//verifie que le nombre d'arguments est 4
	if ( (argc > 4) && (argc < 6 ) )
	{
		char tablefile[10000];
    	int size = save_image(argv[1], tablefile);
    	char error_message2[] = "*** The program has crashed" ;
    	char error_message_save[1000] ;
    	int testBURGE =0 ;
    	int testBURGEX =0 ;

	 	char NameAutor[6] ;
	 	NameAutor[0]= tablefile[4] ;
	 	NameAutor[1]= tablefile[5] ;
	 	NameAutor[2]= tablefile[6] ;
	 	NameAutor[3]= tablefile[7] ;
	 	NameAutor[4]= tablefile[8] ;
	 	NameAutor[5]= tablefile[9] ;


		int iteration = atoi(argv[2]);
		int ModifNbre = atoi(argv[3]);
		float PrcentInputs = atof(argv[4]);

	

		//first check the type of each input parameter
		if ((!is_directory(argv[1])) && (is_digit(argv[2])) && (is_digit(argv[3]))  && (is_float(PrcentInputs))  && (size!=0) )
		{

			//check in the second time the value of each input parameter
			if (argv[1] && (iteration > 0) && (ModifNbre > 0) && (PrcentInputs >= 0.0) && (PrcentInputs <= 1.0) )
			{
				int PrcentInput = PrcentInputs*100;
				int timer = 0;
				int timer1 = 0;
				int duration = 0;
				int ErrorValue[1000];
				int k = 0;
				char BURGE[4];
				char YES[3] = "YES";
				char NO[2] = "NO";
				int NewValue;
            	int Debut_octect = 10;
				int NBR;
				NBR = 0;
				int Prcent = (int)round((size*PrcentInput) / 100);
				int Position[ModifNbre];
				int NewValue_i[ModifNbre];
				strncpy(BURGE, NO, 2);

				/*
				check that the percentage of the octect range to change (percentage) does not exceed the size
				of the image.because the values to randomly change start from the octet number "Debut_octect"
				*/
				if (Prcent+Debut_octect > size)
				{
					Prcent = Prcent- Debut_octect ;
				}
			
				//until we have reached the test number (second parameter)
				while (NBR<iteration)
				{
					char Extention[] = ".img" ;
					char inputFile_attack_Crash[100] = "../out/FileCrash/inputFile_attack_N" ;
					char inputFile_attack_Test[100] = "../out/FileTest/inputFile_attack_N" ;
					char NBR_conver [20] ; 

					int i;
					int MaxItteration = Debut_octect + ModifNbre -1;

					//for a certain number of bytes of the original image, randomly changes their values
					for (i = Debut_octect; i <= MaxItteration ; i++)
					{	
						int Octect = random_number_generator(Debut_octect, Prcent+Debut_octect);
						NewValue = random_number_generator(127, 355);

						tablefile[Octect] = (unsigned)(unsigned char)NewValue;
						Position[i - Debut_octect] = Octect;
						NewValue_i[i - Debut_octect] = NewValue;
					}

					create_file_for_attack(tablefile, size, "inputFile_attack.img");
					execute_command("./converter inputFile_attack.img ouputFile.img 2> ../out/exectute.txt");
					Save_Error_message ("../out/exectute.txt", error_message_save) ;
					printFile ("../out/exectute.txt") ;

    				for (int i = 0 ; i<=2 ; i++)
					{

        				if (error_message_save[i] == error_message2[i] )
						{
            				testBURGE = 1 ;
            				testBURGEX = 1 ;
        				}
       		 			else 
						{
            				testBURGE = 0 ;
        				}
    				}
				
					timer1 = clock();
					duration = timer1 - timer;
					timer = clock();
					create_log_file(tablefile, "../out/FileLog/LOG_File_Attack.txt", NBR, duration, Position, NewValue_i, ModifNbre, error_message_save);
					snprintf (NBR_conver, 20 , "%d", NBR) ;
					strcat (inputFile_attack_Test,NBR_conver ) ;
					strcat (inputFile_attack_Test,Extention ) ;
					create_file_for_attack(tablefile, size, inputFile_attack_Test);
				
					//if the program to crash
					if (testBURGE) 
					{
						strncpy(BURGE, YES, 3);
						create_log_file(tablefile,  "../out/FileLog/LOG_File_CRACHE_OK.txt", NBR, duration, Position, NewValue_i, ModifNbre, error_message_save);
						strcat (inputFile_attack_Crash,NBR_conver ) ;
						strcat (inputFile_attack_Crash,Extention ) ;
						create_file_for_attack(tablefile, size, inputFile_attack_Crash);

						//save the number of the test that caused the crash
						ErrorValue[k] = NBR;
						k++;
					}
					testBURGE = 0 ;
				
					NBR++;

				}

				timer = clock();
				print_result(argv[1], NameAutor, size, PrcentInput, Prcent, NBR, timer, BURGE);
			
				if (testBURGEX)
				{
					printf("--- ");
					for (int i = 0; i <= k; i++)
					{
						printf("%d, ", ErrorValue[i]);
					}
				
				}
				else
				{
					printf("\n---	              No value                      ---");
					printf("\n");
				}
				printf("\n=======================================================");
				printf("\n");
			
				return (EXIT_SUCCESS);
			}
			else
			{
				print_error_message(argv[0], "The negatif number is not authorised");
			}

		}
		else 
		{
			print_error_message(argv[0], "parametre 2, 3 & 4 : is a possitif number") ;
		}

	}else 
	{
		print_error_message(argv[0], "this program takes 4 input argument") ;
	}

	return (EXIT_SUCCESS);
}
