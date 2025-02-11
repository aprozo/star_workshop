# Script Descriptions

This repository contains several shell scripts (`.sh` files) related to the presentation. Below is an explanation of what each script does and how to use it.  

Before executing a script, make it executable with:  
```bash
chmod +x script.sh
```

To execute the script:
```bash
./script.sh
```

## "for" loop
### `1_forLoop.sh`
**Description:**  
This script calculates and prints the square of each number in the array.

## First bash example program
### `2_makeDirectories.sh`
**Description:**
This script creates a directory named `workshop`, adds a file called `readme.txt`, and appends the numbers 1, 2, and 3 to it. It then lists the directory contents, saves them in `list.txt`, prints both files, and finally removes the workshop directory.

## "while" loop
### `3_whileLoop.sh`
**Description:**  
This script prints the numbers from 1 to 5.

## "if-else" loop
### `4_ifElse.sh`
**Description:**  
This script compares the variable `count` to `number` and prints whether `count` is equal to, less than, or greater than `number`.

## How to run the macro in root through bash
### `5_runRootMacro.sh`
**Description:** 
Defines a function to run a ROOT macro with or without an input file and executes it for two different macros.

**Requirements:**
Ensure you have the following files before running:

* gausInput.root
* drawGaussianFromFile.cxx
* drawGaussian.cxx

## Automating Script Execution with Multiple Parameters
### `6_conditions.sh`
**Description:** 
If a .cxx script requires different input conditions, this script automates parameter changes and execution.

**Requirements:**
Ensure you have particles.cxx.


## Automating Script Execution with Multiple Parameters
### `7_checkJobStatus.sh`
**Description:** 
This script checks job statuses, removes stalled jobs if needed, and merges output ROOT files when all jobs are completed. 







# Some helpful sources:

- Advanced git Course from CERN
  https://indico.cern.ch/event/1334738/contributions/5814285/attachments/2833252/4959870/git-good.pdf
