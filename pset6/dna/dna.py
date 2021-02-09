# Searches DNA sequences for STRs and looks for a match in a database

# Import necessary functions
from sys import argv, exit
import csv
import re

# Check to make sure len(argv) = 3
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# Open DNA sequence file and store in memory
sequence_file = open(argv[2])
dna_seq = sequence_file.read()
sequence_file.close()

# Open database file and read to memory
data_file = open(argv[1])
reader = csv.DictReader(data_file)

# Save fieldnames to set and convert to list of strs
strs = reader.fieldnames
str_list = list(strs)

# Remove 'name' from beginning of str_list
str_list.pop(0)

# Create list of dictionaries to store data
data_list = []
for row in reader:
    data_list.append(row)

# Close data file
data_file.close()

# Determine number of STRs in data
# Determine number of people in data
str_num = len(str_list)
people_num = len(data_list)

# Convert numbers in data to integers
for i in range(people_num):
    for j in range(str_num):
        data_list[i][str_list[j]] = int(data_list[i][str_list[j]])

# Search for strs in dna_seq

# Create list to store values found
str_found = []

# Loop through strs
for i in range(str_num):

    # Set str_count to 0
    str_count = 0

    # Determine length of str and length of dna_seq
    str_len = len(str_list[i])
    seq_len = len(dna_seq)

    # Loop through all positions of dna_seq
    for j in range(seq_len - str_len + 1):

        # Set tmp_count to 0
        tmp_count = 0

        # Check to see if str starts at index
        if re.search(str_list[i], dna_seq[j:j + str_len]):
            tmp_count += 1

            # Check for subsequent appearances
            a = j + str_len
            b = j + 2 * str_len
            while True:
                if re.search(str_list[i], dna_seq[a:b]):
                    tmp_count += 1
                    a += str_len
                    b += str_len
                else:
                    break

            # Adjust str_count to highest value found so far
            if tmp_count > str_count:
                str_count = tmp_count

    # Add str_count found to str_found list
    str_found.append(str_count)

# Compare str_found to database
# Loop through people
for i in range(people_num):

    # Loop through strs
    for j in range(str_num):

        # Compare values, break loop if no match
        if data_list[i][str_list[j]] != str_found[j]:
            break

        # If all strs match, print name of match and exit
        elif j == str_num - 1:
            print(f"{data_list[i]['name']}")
            exit(0)

# If no matches found in data_list, print 'No match'
print("No match")
exit(0)
