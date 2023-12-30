# What is Boris?
Boris is a code generator that takes code written in "Boris" and produces C files from it.

# Features
Syntactic Sugar:
Boris performs automatic semicolon insertion for most cases.
Boris does not require parentheses around if, else if, and else statements along with while or for loops
Boris adds a walrus operator that uses the keyword auto to perform variable type deduction and initialization (added as of C23). By default, variables initialized using the walrus operator will be const, and can be made mutable by adding the word "mut" right before the ":=" operator
Boris creates separate header (.h) and source (.c) files for each file imported by the program (using the "import" keyword) and stores which files have been included so far in order to avoid multiple include errors.

# Usage
Boris has to be added to path manually, and is meant to be used from the directory in which the .boris files are located.
As of right now, boris files cannot import files in other directories other than their current residing directory.
To generate the corresponding C files from Boris code, use the command "boris compile {file_name}". Boris will recursuvely translate any files imported by the file

# Future Features
Support for importing files in other directories is planned to be added.
Configuration for Boris will also be added, where the user can add the C compiler commands they wish to use.
A "build" flag will be added which will generate the executable by calling the C compiler listed in the user's configuration.
