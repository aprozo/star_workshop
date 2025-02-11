#!/bin/bash

# Initialize the count variable
count=1

# Start the first while loop to count from 1 to 5
while [ $count -le 5 ]; do
    # Print the current count value
    echo "Count: $count"
    
    # Increment the count by 1
    count=$((count + 1))
done

# Start the second while loop, triggered when count is equal to 6
while [ $count -eq 6 ]; do
    # Print that the count is equal to 6
    echo "Count is equal to $count"
    
    # Exit the loop after the condition is satisfied
    break  # Breaks the loop and exits immediately
done
