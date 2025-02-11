#!/bin/bash

# File to be modified
FILE="program.cpp"

# New function to be added to the code
NEW_FUNCTION="
void generate_and_log_random_size() {
    int random_size = rand() % 100 + 1;  // Generates a random size between 1 and 100
    std::cout << \"Random data size: \" << random_size << std::endl;
    std::vector<int> random_data = generate_data(random_size);
    log_data(random_data, \"Random data\");
}
"

# Add the new function declaration at the beginning of the file
sed -i '1i\
'"$NEW_FUNCTION" $FILE

# Modify the simulate_operation function to call the new function
sed -i '/void simulate_operation()/a \
    generate_and_log_random_size();' $FILE

echo "The code has been modified. The new function has been added, and simulate_operation has been updated."
