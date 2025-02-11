#!/bin/bash

# Define a function to run a ROOT macro
run_root_macro() {
    # 'local' makes the variables only accessible within this function
    # $1 and $2 represent the first and second arguments passed to the function
    local macro_name=$1      # Name of the ROOT macro
    local input_file=$2      # Input file list
    
    # Check if an input file is provided. -z checks if the string is empty.
    if [[ -z "$input_file" ]]
    then
        # If no input file is provided, run the ROOT macro without it
        # -l: Start ROOT in interactive mode
        # -b: Run ROOT in batch mode (no GUI)
        # -q: Exit ROOT after executing the macro
        root -l -b -q "${macro_name}" || exit
    else
        # If an input file is provided, run the ROOT macro with it
        root -l -b -q "${macro_name}(\"${input_file}\")" || exit
    fi    
}

# Call the function with the macro name "drawGaussian.cxx" (no input file)
run_root_macro "drawGaussian.cxx"

# Call the function with the macro name "drawGaussianFromFile.cxx" and input file "gausInput.root"
run_root_macro "drawGaussianFromFile.cxx" "gausInput.root"
