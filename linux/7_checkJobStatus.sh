#!/bin/bash
# CHANGE the following parameters:
runningTimeLimitHours=12                          #hours
sleepTime=10                                      #seconds
outputDir="/path/to/root/files"                   #path to the directory containing your new root files
outputFile="output.root"                          #name of the output file to be placed in the current directory

#  ========================================================
username=$(whoami) # Set the username for which to check the jobs
runningTime=0     # Initialize the running time to 0
condorQFile='condor.out' # File to save the output of condor_q
runningTimeLimit=$((runningTimeLimitHours * 3600)) #seconds


# Function to check if all jobs for the user are finished
check_jobs() {
    condor_q >$condorQFile # Save the output of condor_q to a file
    
    # Extract the line containing the user's job information
    userJobsLine=$(grep "Total for $username" $condorQFile)
    
    #example userJobLine:
    #Total for username: 10 jobs; 1 completed, 2 removed, 3 idle, 0 running, 4 held, 0 suspended
    
    # Extract the number of jobs from the user's line
    leftJobs=$(echo "$userJobsLine" | awk '{print $4}') # 4th column (word) in the line
    idleJobs=$(echo "$userJobsLine" | awk '{print $10}') # 10th column (word) in the line
    runningJobs=$(echo "$userJobsLine" | awk '{print $12}') # 12th column (word) in the line
    heldJobs=$(echo "$userJobsLine" | awk '{print $14}') # 14th column (word) in the line
    
    echo "$userJobsLine"
    # Check if all jobs are finished
    if [ "$leftJobs" = "0" ]; then
        echo "Jobs for user $username are finished! -------------------> Let's merge the root files!"
        return 1
        # Check if the running time exceeds the limit
        elif [ $runningTime -gt $runningTimeLimit ]; then
        echo "Jobs for user $username are running for too long! Removing all remaining jobs..."
        condor_rm $username
        echo "Further resubmission needed"
        return 1
        # Check if there are held jobs
        elif [ "$heldJobs" != "0" ] && [ "$idleJobs" = "0" ] && [ "$runningJobs" = "0" ]; then
        echo "There are held jobs for user $username. Removing all remaining jobs..."
        condor_rm $username
        echo "Further resubmission needed"
        return 1
        
    else
        # If there are still running jobs, wait for 10 seconds
        echo "Some jobs are still running for user $username. Please wait."
        echo "Sleep $sleepTime seconds..."
        
        runningTime=$((runningTime + sleepTime)) # Increment runningTime by 10 seconds
        # output percentage of time passed
        echo "Percentage of time passed: $((runningTime * 100 / runningTimeLimit))%"
        echo " "
        sleep $sleepTime
    fi
}

merge_files() {
    echo " "
    echo "Merging root files..."
    setup 64b          # star system is a 32bit system, to use ROOT6 we need to switch to 64bit
    setup root 6.20.08 # setup the root version which can merge files in parallel
    
    # beware not to use the same folder for your new analysis and old one, or delete your old root files
    hadd -f -k -j ${outputFile} ${outputDir}/*.root # -f force overwrite(current outputFile), -k skip corrupt files, -j parallelize
    
}

# Main loop to continuously check job status
while :; do
    # The colon `:` is a no-op (no operation), which always returns true. This creates an infinite loop.
    # The loop will continue executing until a `break` condition is met.
    
    # Call the check_jobs function; if it returns a non-zero exit status (jobs are still running or stuck), exit the loop
    check_jobs || break  # If all jobs are finished, exit the loop
    
done  # End of the loop


rm $condorQFile
merge_files