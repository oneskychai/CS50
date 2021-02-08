# Import data from characters.csv and write to students.db
# For some reason this program glitches in the IDE because it changes the case on first
# and last to upper, but it checks at 100% in the gradebook, no idea why

# Import necesary functions
import cs50
import csv
from sys import argv, exit

# Check to make sure len(argv) = 2
if len(argv) != 2:
    print("Usage: python import.py data.csv")
    exit(1)

# Create database
open("students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# Create table
db.execute("CREATE TABLE students (id INT, first VARCHAR(255), middle VARCHAR(255), last VARCHAR(255), house VARCHAR(10), birth INT, PRIMARY KEY(id))")

# Open csv file
with open(argv[1], "r") as people:

    # Create DictReader
    reader = csv.DictReader(people)

    # Create counter for id
    id = 1

    # Iterate over csv file
    for row in reader:

        # Separate out names
        names = row["name"]
        names_arr = names.split()
        if len(names_arr) == 2:
            name_f = names_arr[0]
            name_m = None
            name_l = names_arr[1]
        else:
            name_f = names_arr[0]
            name_m = names_arr[1]
            name_l = names_arr[2]

        # Cast birth to int
        birth = int(row["birth"])

        # Insert name into database
        db.execute("INSERT INTO students (id, first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?, ?)",
                    id, name_f, name_m, name_l, row["house"], birth)

        # Add 1 to id counter
        id += 1
