# LINGI2347_Computer-system-security



## Introduction 


for this project, let's write a fuzzer, i.e. tools that try to find vulnerabilities in programs by testing them with random input.
The goal of this group work is to write a fuzzer for a simple program called Converter.

### The program Converter
Converter is a small tool written in C that reads a picture with indexed colors and converts it into a picture with non-indexed colors. 
We explain in the following what that means.
Converter only supports one file format, the ABCD image file format (invented at UCLouvain just for a student). 
An image file in this format looks like the following:

<table>
    <thead>
        <tr>
            <th>section</th>
            <th >Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>0xAB 0xCD</td>
            <td >A so-called magic number. All ABCD files start with these two bytes</td>
        </tr>
        <tr>
            <td>version</td>
            <td >16-bit value. Version number of the file format. Current version is 100, although some older versions are also accepted.</td>
        </tr>
        <tr>
            <td>authorname</td>
            <td >A sequence of 8-bit characters. Terminated with a 0-byte.</td>
        </tr>
        <tr>
            <td>width</td>
            <td >32-bit value. The width of the picture (in pixels). A negative value indicates a compressed file but this feature is not supported by the current version of the tool.</td>
        </tr>
        <tr>
            <td>height</td>
            <td >32-bit value. The height of the picture (in pixels)</td>
        </tr>
        <tr>
            <td>numcolors</td>
            <td >32-bit value. The number of colors in the color table (see the next field). In the current version, we only support color tables with up to 256 colors.</td>
        </tr>
        <tr>
            <td>colortable</td>
            <td >An array of 32-bit color values</td>
        </tr>
        <tr>
            <td>pixels</td>
            <td >A sequence of unsigned 8-bit values, representing the colors of the pixels. Each value is interpreted as an index in the color table.</td>
        </tr>
</table>


The ABCD file format is a binary format where numbers are stored in Little-Endian. 

That means that a 2x2 image with 2 colors 0x00000000 (black) and 0x00FFFFFF (white) and author name “Ram” contains the following 30 bytes:

* AB CD // the magic number in Little Endian
* 64 00 // version = 100
* 52 61 6D 00 // author = Ram
* 02 00 00 00 // width = 2
* 02 00 00 00 // height = 2
* 02 00 00 00 // the size of the color table. The color table contains two colors.
* 00 00 00 00 // the first color
* FF FF FF 00 // the second color
* 00 01 // the 4 pixels of the image: black white
* 01 00 // white black

The tool is started like this:

``` bash
./converter inputfilename outputfilename 
```
The tool reads the input image file and produces an output image file where the pixels with the color indexes are replaced by pixels with the full 32-bit color values from the color table. The output version is always 100.
* CD AB // the magic number in Little Endian
* 64 00 // version = 100
* 52 61 6D 00 // author = Ram
* 02 00 00 00 // width = 2
* 02 00 00 00 // height = 2
* 00 00 00 00 // no color table (color table size is 0).
* 00 00 00 00 FF FF FF 00 // the 4 pixels of the image: black white
* FF FF FF 00 00 00 00 00 // white black

The folder **int/** contains an example input file **testinput.img** with 16x16 pixels and 4 colors and also the corresponding output file “testoutput.img”. On Linux, you can look inside these files with
		
		
## Mutation-based fuzzer

### CONTENT

 Folder Project1 contains  5 sub folders:
	
 Sub folder attack_file/ :contains the 5 attack files

* inputFile_attack_N0_Height.img            : crash the converter program by changing the height values.
* inputFile_attack_N1_ColorTables.img       : crash the converter program by changing the ColorTables values
* inputFile_attack_N2_author+width.img      : crash the converter program by changing the author values and width values
* inputFile_attack_N3.img                   : crash the converter program by changing the ColorTables values
* inputFile_attack_N3.img                   : crash the converter program by changing the ColorTables values

Sub folder **out/** contains  3 sub folders :

* FileCrash/    :	contains all '.img' files that crash the program during its execution
* FileLog/      :	contains all log of bytes that have been changed and their values in unsigned decimal
* FileTest/     :	contains all the '.img' files that were used to test the program during its execution

Sub folder **src/** : containing initially source codes
	
* **fuzzer1.c**     : source code of Mutation-based fuzzer
* **fuzzer2.c**    : source code of Generation-based fuzzer 
* **FUZZER1**       : compiled version of fuzzer1.c
* **FUZZER2**       : compiled version of fuzzer2.c 
* Converter     : program initially given for test purposes
* testinput.img : input file for test purposes
* testoutput.img : output file for test purposes
		
		
### EXECUTION.
	
In order to execute the program:

1- Go to  src/

2- To avoid, compiler errors about missing functions. You had to compile by creating a link to the math library when compiling. 

This is done by passing the "```-lm```" a gcc option. The reason for this is that the math library is not linked by default because 
for many system programs it is not necessary.
Compile the program to be executed  with gcc : 
```
gcc input_file -o outputfile -lm
```

For instance :

• fuzto compile fuzzer1 run 
``` bash
gcc myfuzzer1.c –o MYFUZZER1 -lm
```

• fuzto compile fuzzer2 run 
```bash
gcc myfuzzer2.c –o MYFUZZER2 -lm
```

3- Launch the generated program.

• For FUZZER1 : 
```bash
#./MYFUZZER1 input_file number_of_tests percentage
```
For example : 
```bash
#./MYFUZZER1 testinput.img 200 0.05
```
Constraints : 

* l’argument_1 (Testinput.img) should be an iput string 
* L’argument_2 (200) should be greater > 0
* L’argument_2 (200) should be greater > 0
* L’argument_3 (100) should be greater > 0
* L’argument_4 (0.01) must be an integer between [0.0 et 1.0].
				
• for FUZZER2 : 
```bash
#./MYFUZZER2 number_of_generated_file
```
For example : 
```bash
#./FUZZER2 17
```
			
Constraints : 
• L’argument_1 (200) should be greater > 0

#### NOTE
the approach logic and a small description of the operation of the fuzzer1.c and fuzzer2.c code can be found in the /Report/Code_description file


### Versions of program-crashing image file
		

Off limit <height> field value I.e value > 256
For instance when, modifying the 10th byte of the program  so that that the height value becomes 10 00 64 00 in  Little-Endian, 
and  6553601 in decimal.

Off limit <numcolors> value i.e value > than 256 in decimal, makes the program bug.
	