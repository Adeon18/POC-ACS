#!/usr/bin/python

import sys
import os
import subprocess

DEBUG = False

EPSILON = 10 ** (-8)

TIMES_TO_RUN = int(sys.argv[1])
FILE_IN = sys.argv[2]
PROGRAM_PATH = "./bin/integrate_task_4"

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

    # There are 5 methods
    for i in range(TIMES_TO_RUN):
        program_args = [PROGRAM_PATH, FILE_IN]
        # Test a method n times
        # Start the process
        prog = subprocess.Popen(program_args, stdout=subprocess.PIPE)
        prog.wait()

        # Store the smallest result
        result = str(prog.stdout.read()).split("\\n")
        if len(data) == 0:
            data.append(int(result[0][2]))
            data.append(float(result[1]))
            data.append(float(result[2]))
            data.append(float(result[3]))
            data.append(int(result[4]))
        else:
            if abs(float(result[1]) - data[1]) > EPSILON or abs(float(result[2]) - data[2]) > EPSILON or abs(float(result[3]) - data[3]) > EPSILON:
                data_is_equal = False

        if int(result[4]) < data[4]:
            data[4] = int(result[4])
        if DEBUG:
            print(data)
    print("Quickest time: ", data[4])
    if data_is_equal:
        print("All results are the same")
    else:
        print("The results are different")