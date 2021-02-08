# Prints out the names of all students in a given house in alphabetical order
# For some reason this program glitches in the IDE because it changes the case on first
# and last to upper, but it checks at 100% in the gradebook, no idea why

# Import necessary functions
import cs50
from sys import argv, exit

# Check to make sure len(argv) = 2
if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Open database
open("students.db", "r")
db = cs50.SQL("sqlite:///students.db")

# Extract data for house input by user
roster = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

# Count number of students in house
count = len(roster)

# Print out roster
for i in range(count):
    if roster[i]["middle"] == None:
        print(f"{roster[i]['FIRST']} {roster[i]['LAST']}, born {roster[i]['birth']}")
    else:
        print(f"{roster[i]['FIRST']} {roster[i]['middle']} {roster[i]['LAST']}, born {roster[i]['birth']}")
