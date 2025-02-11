#!/bin/bash

# Filename to be modified
FILE="Vzorek.C"

# Change random number generation (1–23 instead of 1–200)
sed -i 's/rand() % 200 + 1/rand() % 23 + 1/' "$FILE"

# Modify data processing (instead of multiplying by 2, add to the previous number)
sed -i 's/processed_data.push_back(x \* 2);/processed_data.push_back(x + (processed_data.empty() ? 0 : processed_data.back()));/' "$FILE"

echo "Modifications have been made."
