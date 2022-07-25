
# Lab work 3: Write functions on assembly
Authors (team): 
Ostap Trush: https://github.com/Adeon18 <br>
Daria Minieieva: https://github.com/DariaMinieieva <br>
Variant func_1: 1 <br>
Variant func_2: 11 <br>
OS: LINUX

## Before 

- Basically only gcc, since we provide the assembler executable for 64 bit linux in our project.

### Compiling

In the root directory of our project there is a directory 'fasmg' with fasmg.x64 executable
and 'include' directory. To compile our project go to the root directory, type
make FASM_INCLUDE = <ABSOLUTE/path/to/your/include/dir> FS = <path/to/your/fasm/executable> 
(those variable are optional). Executable files will appear in the bin directories of each function with
names 'call_from_c' and 'call_from_s' as was stated in the README in bin directory.

### Installing

There is no installation

### Usage

To use our project either go to bin directories of each function and execute files from there
or remain in the root directory and type ./<func_dir>/bin/<exec_name>.

### Important!

For variance we used the following formula: Var(x) = E(x^2) - E^2(x)
There are two different formulas for variance of sample and variance of
population so we decided to mention that.

# Results

The results of our work are functions written on assembly language that sort unsigned 32-bits integers
and calculates min, max, mean and variance of an array of signed 32-bits integers. Those functions are
linked with c files and assembly files that call this functions.

# Additional tasks
No additional tasks

# ATTENTION!
Additional tasks not listed in the previous paragraph would not be graded.

Be sure to provide a complete list of authors, variant and OS.

Output files names are: __call_from_c__, __call_from_s__

