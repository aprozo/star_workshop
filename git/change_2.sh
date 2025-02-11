#!/bin/bash

# Filename to be modified
FILE="Vzorek.C"

# Change the range of generated numbers (from 1-100 to 1-200)
echo "Changing the range of generated numbers to 1-200..."
sed -i 's#rand() % 100 + 1#rand() % 200 + 1#g' "$FILE"

# Add median calculation to analyze_data
echo "Adding median calculation to analyze_data..."
sed -i '/double avg = (data.empty()) ? 0 : static_cast<double>(total) \/ data.size();/a\
    std::vector<int> sorted_data = data;\
    std::sort(sorted_data.begin(), sorted_data.end());\
    double median = (sorted_data.size() % 2 == 0) ? \
        (sorted_data[sorted_data.size()/2 - 1] + sorted_data[sorted_data.size()/2]) / 2.0 \
        : sorted_data[sorted_data.size()/2];\
    std::cout << "Median: " << median << std::endl;' "$FILE"

echo "Modifications have been made."
