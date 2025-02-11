#!/bin/bash

# Define an array of conditions with key-value pairs to modify a task
CONDITIONS=("kFALSE_kTRUE|Bool_t pion = kFALSE, Bool_t kaon = kTRUE"
            "kTRUE_kFALSE|Bool_t pion = kTRUE, Bool_t kaon = kFALSE")

# Generic function to process a task with different conditions
processTask() {
    local task=$1  # Store the task name passed as the first argument
    
    # Loop through each condition in the CONDITIONS array
    for condition in "${CONDITIONS[@]}"; do
        key="${condition%%|*}"    # Extract the key before the "|" character (i.e., the first part of the condition)
                                  # The '%%' operator removes the longest match of '|' and everything after it
        newInputParam="${condition##*|}"  # Extract the new task parameters after the "|" character
                                      # The '##' operator removes the shortest match of everything before and including '|'
        
        # Use a general pattern to match any function signature for the given task
        pattern="void $task\s*\(.*\)"  # Matches "void <task_name>(...)"
        replacement="void $task($newInputParam)"  # Define the replacement pattern with new parameters
        
        echo "Running $task for $key"  # Print which condition is being processed
        
        # Use sed to modify the task's C++ file by replacing the function parameters with the new ones
        sed -i "s|$pattern|$replacement|" "$task.cxx"
        
        # Verify that the changes were made in the file
        grep "void $task" "$task.cxx"
        
        # Run the task using ROOT, exit if there's an error
        root -l -b -q "$task.cxx" || exit
        
        echo "Done"  # Print "Done" after the task is completed
    done
}

# Call the processTask function with the argument "particles"
processTask "particles"
