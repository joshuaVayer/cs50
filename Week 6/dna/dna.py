# 1 - Imports

import csv
import sys
from copy import deepcopy


# 3 - Main

def main():

    # Ensure that all required arguments are entered
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    persons = []
    # Read the CSV file into memory
    database = sys.argv[1]
    with open(database) as file:
        reader = csv.DictReader(file)
        # Implement the persons array
        for row in reader:
            persons.append(row)

    # Get the dna sequence
    dna_sequence = open(sys.argv[2], "r").read()

    # Get the strs that we will need to analyse
    strs = [*persons[0]]
    strs.remove("name")

    # Get the suspect position, first argument contains a dict with strs longuest runs in dna_sequence
    suspect = find_suspect(get_str(dna_sequence, strs), deepcopy(persons))

    # Look for the suspect and print his name or pint "No match" if we didn't find him
    if suspect != -1:
        print(persons[suspect]["name"])
    else:
        print("No match")



# 4 - Functions library

# Return the longuest runs of strs we're investigating
def get_str(dna, strs):

    counts = {}

    # Loop for every strs
    for STR in strs:

        # Variables declaration
        counts[STR] = 0
        prev = dna[0]
        run = 1
        p = 0

        # Loop on each charc of the sequence
        for i in range(len(dna) + 1):

            dna_split = dna[p : p + len(STR)]

            # Check if match the str and previous and jump to the next str to check if the next one is also one
            if dna_split == STR:
                if counts[STR] == 0: counts[STR] = 1
                if dna_split == prev: run += 1
                p += (len(STR) - 1)

            # Update the counter if the latest run was longer
            if dna_split != prev and run != 1:
                if counts[STR] < run: counts[STR] = run
                run = 1

            # Update the previous word and increment the counter
            prev = dna_split
            p += 1

    # Parse the counts to string, so that we can compare them with the persons array
    for count in counts:
        counts[count] = str(counts[count])


    return counts


# Return the suspect position in the persons array or -1 if no matches
def find_suspect(STR, persons):

    suspect = 0

    for person in persons:

        person.pop("name")

        if person == STR:
            return suspect

        suspect += 1

    return -1


# EXECUTION

if __name__ == "__main__":
    main()