#!/usr/bin/python

import sys
import os
import subprocess

DEBUG = False

EPSILON = 10 ** (-8)

TIMES_TO_RUN = int(sys.argv[1])
FILE_IN = sys.argv[2]
FILE_OUT = sys.argv[3]
PROGRAM_PATH = "./bin/method_testing"

# Store the smallest times
methods = {1: float("inf"),
           2: float("inf"),
           3: float("inf"),
           4: float("inf")}

# Store everything for checking the equality of data
data = []
data_is_equal = True

if __name__ == "__main__":

    if os.name == "Windows":
        OS = "Windows"
    else:
        OS = "Linux"

    # Compile everything(Only if there is no executable)
    if OS == "Windows":
        if not os.path.isfile(PROGRAM_PATH):
            print("Please compile the program first")
        else:
            PROGRAM_PATH = "./bin/method_testing.exe"
    else:
        if not os.path.isfile(PROGRAM_PATH):
            process = subprocess.Popen(["./compile.sh", "-O"])
            process.wait()

    print("Method\tMicroseconds")
    # There are 5 methods
    for i in range(1, 5):
        program_args = [PROGRAM_PATH, str(i), FILE_IN, FILE_OUT]
        # Test a method n times
        for j in range(TIMES_TO_RUN):
            # Start the process
            prog = subprocess.Popen(program_args, stdout=subprocess.PIPE)
            prog.wait()

            # Store the smallest result
            result = int(prog.stdout.read())
            if result < methods[i]:
                methods[i] = result
            if DEBUG:
                print("Method " + str(i) + " Test " + str(j))
                print(result, methods[i])
            # Check for results equality
            with open(FILE_OUT, "r") as f:
                read_data = f.read().split()
                if len(data) == 0:
                    data.append(float(read_data[0]))
                    data.append(int(read_data[1]))
                else:
                    if not (abs(float(read_data[0]) - data[0]) < EPSILON and int(read_data[1]) == data[1]):
                        data_is_equal = False

        print("Method " + str(i) + "\t" + str(methods[i]))

    if data_is_equal:
        print("All results are the same")
    else:
        print("The results are different")