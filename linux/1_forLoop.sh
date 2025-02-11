#!/bin/bash

# Define an array of numbers
numbers=(10 20 30 40 50)

# Iterate over the array
for i in "${numbers[@]}"; do
    # Print the current number
    echo "Processing number: $i"
    
    # Calculate the square of the number
    square=$((i * i))
    
    # Print the square of the number
    echo "Square: $square"
done
