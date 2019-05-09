/*
#########################################################################################################
#--Autor--		* Anicet SIEWE KOUETA																	#
#																		#
#																										#
#--Descroption--																						#
#																										#
#				this program automatically generates files that crash the subprogram converter			#																	#
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


/*
@param: string command to be executed
Executes the command passed in parameter and
prints in command line the results returned by the command's execution
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
@param: fileName, fileArray
Saves the content of the file "fileName" into the array "fileArray"
in which each cell contains a byte  hexadecimal format
and returns the total number of characters writtten to the output file
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

		printf("\n");
		printf(" \" %s \" Contains %d characters \n", fileName, nbr);
		printf("\n");
		fclose(F);

		return nbr;
	}

	else
	{
		printf("\n Could not execute the programme.\n could not find the image file \"%s\"", fileName);
		return 0;
	}

}

/**
* @param: fileArray		: array containing image file content
*         size				: the new size of the output file
*         ouputFileName	: file where will be saved the **********
*
* Creates an ouput file based on the file's Array with size newSize
*
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
*Creates a log file containing all  bytes that have been modified and their values
*
*
*/
void create_log_file(char fileArray[], char fileName[], int testNumber, int Position[], int NewValue[], int nber_of_changed_values) {
	FILE *F;
	time_t hour = time(NULL);
	char date[20];
	strftime(date, sizeof date, "%d/%m/%Y - %H:%M", localtime(&hour));
	F = fopen(fileName, "a");

	if (F != NULL)
	{
		int i;
		fprintf(F, "Test N°: %d  du %s, .changes made on : (octect_N°,value) \n", testNumber, date);

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

void print_result(int size, int Nbre, int duration) {

	printf("\n");
	printf("\n===================== Groupe ========================");
	printf("\n---                                                 ---");
	printf("\n---  Image Size            : %d", size);
	printf("\n---  total files create    : %d", Nbre);
	printf("\n---  Duration         	   : %5d ms	 	    ---", duration);
	printf("\n---                                                 ---");
	printf("\n---  File Directory	   : \"../out/FileCrash/\"    ---");
	printf("\n---  File name	: Attack_FileGenerate_Nx.img        ---");
	printf("\n---                                                 ---");
	printf("\n=======================================================");
	printf("\n");
	
}
int print_error_message(char argv[]) {

	printf("\n");
	printf("\n===================== Groupe ========================");
	printf("\n---                                                 ---");
	printf("\n--- 	Invalid command \"%s\"", argv);
	printf("\n---                                                 ---");
	printf("\n=======================================================");
	printf("\n---      The command should contain 1 parameter:   ---");
	printf("\n---                                                 ---");
	printf("\n---	1. Number of file Generates 			    ---");
	printf("\n=======================================================");
	printf("\n");
	return (EXIT_FAILURE);
}


/*
Checks if a string is a digit or not
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




/************************************************** MAIN *****************************************************************/

 int main(int argc, char *argv[])
{
	int timer = 0;
	int timer1 = 0;
	int duration = 0;
	int size;
	char tablefile[1000];

	int preamble[] = {171, 205, 100, 0, 82, 97, 109, 105, 110, 0, 16, 0, 0, 0 } ;
	size = sizeof(preamble) ;

	//verifie que le nombre d'arguments est 1
	if ( (argc > 1) && (argc < 3 ) )
		{
		
		//first check the type of each input parameter
		if ( is_digit(argv[1]) ) {

			int iteration = atoi(argv[1]);

			//check in the second time the value of each input parameter
			if (iteration > 0 )
			{
				int NewValue;
            	int Debut_octect = 10;
            	int Fin_octect = 22 ;
				int NBR;
				NBR = 0;
				int Position[Fin_octect];
				int NewValue_i[Fin_octect];
			
				while (NBR<iteration)
				{ 
					char Extention[] = ".img" ;
					char inputFile_attack_N[100] = "../out/FileCrash/Attack_FileGenerate_N" ;
					char NBR_conver [20] ;


					for (int i = 0; i <= 13; i++)
					{

						tablefile[i] = (unsigned)(unsigned char)preamble[i];
						Position[i] = i;
						NewValue_i[i] = preamble[i];
					}

					for (int i = 14; i <= 21; i++)
					{

						int NewValue = random_number_generator(200, 255);
						tablefile[i] = (unsigned)(unsigned char)NewValue;
						Position[i] = i;
						NewValue_i[i] = NewValue;
					} 

					timer1 = clock();
					duration = timer1 - timer;
					timer = clock();

					snprintf (NBR_conver, 20 , "%d", NBR) ;
					strcat (inputFile_attack_N,NBR_conver ) ;
					strcat (inputFile_attack_N,Extention ) ;
					create_file_for_attack(tablefile, size, inputFile_attack_N);
					create_log_file(tablefile, "../out/FileLog/LOG_File_Fuzzer2.txt", NBR, Position, NewValue_i, Fin_octect );

					NBR++;
				}

				print_result(size, NBR, duration);
			
				return (EXIT_SUCCESS);
			}
			else
			{
				print_error_message(argv[0]);
				printf ("\n") ;
			}
		}
		else
		{
			print_error_message(argv[0]);
		}

	}
	else
	{
		print_error_message(argv[0]);
	}

	return (EXIT_SUCCESS);
}
