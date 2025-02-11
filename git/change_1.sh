#!/bin/bash

# Filename to be modified
FILE="Vzorek.C"

# Change the number of generated data points from 50 to 100
echo "Modifying the number of generated data points to 100..."

# Use sed to replace the value 50 with 100 in the generate_data function
sed -i 's/generate_data(50)/generate_data(100)/g' $FILE

# Optionally, add a new logging output (let's assume we want to log a new output)
echo "Adding a new logging output to the simulate_operation function..."

# Add a new log line after data analysis
sed -i '/log_data(processed_data, "Analyzed data");/a\    log_data(processed_data, "New data after analysis");' $FILE

echo "Modifications have been made."
