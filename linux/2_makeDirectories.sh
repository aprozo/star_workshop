#!/bin/bash

# Define file and directory names
fileName="readme.txt"
directoryName="workshop"
listName="list.txt"

# Create the directory. The -p flag ensures no error if the directory already exists.
mkdir -p $directoryName

# Navigate into the newly created directory
cd $directoryName

# Create the file
touch $fileName

# Loop to append numbers 1 to 3 into the file
for i in {1..3}; do
    echo "$i" >> "$fileName"
done

# Display the content of the created file
echo "Content of the $fileName:"
cat $fileName

# List the contents of the current directory and save to list.txt
ls > $listName

# Display the content of list.txt
echo "Content of the $listName:"
cat $listName

# Return to the parent directory and remove the directory
cd ..
rm -r $directoryName
