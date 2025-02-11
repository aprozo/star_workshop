#!/bin/bash

# Initialize the count and number variables
count=1
number=5

# Check if count is equal to number
if [ $count -eq $number ]; then
    # If count equals number, print this message
    echo "Count is $number!"
    
# Check if count is less than number
elif [ $count -lt $number ]; then
    # If count is less than number, print this message
    echo "Count is less than $number."
    
# If neither of the above conditions are true, count must be greater than number
else
    # Print this message if count is greater than number
    echo "Count is greater than $number."
fi
