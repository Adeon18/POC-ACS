#!/bin/bash
#
# A script to check if the results of the indexer are always the same
# To run: ./run.sh [number_of_tests] [path_to_conf] (name_of_exe(opt))

if (( $# < 2)); then
    echo "Not enough arguments. Use ./run.sh [number_of_tests] [path_to_conf] (name_of_exe(opt))"
    exit
fi

readonly TEST_NUM="$1"
readonly CONFIG_PATH="$2"
readonly EXE_NAME="$3"
readonly PREV_IFS="${IFS}"
readonly CONFIG_OUT_A_NAME="out_by_a"
readonly CONFIG_OUT_N_NAME="out_by_n"
declare same_results=true
declare -a smallest_times=()

if [[ -n "$EXE_NAME" ]]; then
    EXE_PATH="./bin/$EXE_NAME"
else
    EXE_PATH="./bin/word_indexer"
fi

# Redefine the default separator for config read
IFS="="
while read -r name value
do  
    if [[ "$name" == "$CONFIG_OUT_A_NAME" ]]; then
        out_file_a_path="$(echo ${value%%[[:space:]]*} | tr -d \")"
    elif [[ "$name" == "$CONFIG_OUT_N_NAME" ]]; then
        out_file_n_path="$(echo ${value%%[[:space:]]*} | tr -d \")"
        break
    fi
done < $CONFIG_PATH
IFS=$PREV_IFS

# Check if file paths are nonzero
if [[ -z "$out_file_a_path" ]] || [[ -z "$out_file_n_path" ]]; then
    echo "Could not read out_paths. out_by_a: $out_file_a_path; out_by_n: $out_file_n_path;"
fi

# Compile the program if it is ot in bin
if ! [[ -f "$EXE_PATH" ]]; then
    echo "No executable at $EXE_PATH, compiling.."
    ./compile.sh -O > /dev/null 2>&1
    if ! [[ -f "$EXE_PATH" ]]; then
        echo "The compiled executable's name does not match $EXE_PATH"
        exit
    fi
fi

# Run the program TEST amount of times
for (( i=1; i<=${TEST_NUM}; i++ ))
do
    res=$("$EXE_PATH" "$CONFIG_PATH")
    err=$?
    if (( $err != 0 )); then
        echo "ERROR: ${EXE_NAME} finished with exit code $err"
        exit
    fi

    res_split=(${res//=/ })
    # If the run is not the first - compare + get the smallest time else gather data
    if [[ ${smallest_times[0]} ]]; then
        res_a=$(< $out_file_a_path)
        res_n=$(< $out_file_n_path)
        if [[ "$res_n" != "$res_n_main" ]] || [[ "$res_a" != "$res_a_main" ]]; then
            same_results=false
        fi

        if (( ${res_split[1]} < ${smallest_times[0]} )); then
            for j in 0 1 2 3; do
                smallest_times[$j]="${res_split[$((j * 2 + 1))]}"
            done
        fi
    else
        res_a_main=$(< $out_file_a_path)
        res_n_main=$(< $out_file_n_path)
        for j in 1 3 5 7; do
            smallest_times+=("${res_split[$j]}")
        done
    fi
done

for i in 0 1 2 3; do
    echo "Min${res_split[$((i*2))]}=${smallest_times[$i]}"
done
# Check for final results
if [[ "$same_results" == "true" ]]; then
    echo "Results are the same."
else
    echo "Results are NOT the same."
fi