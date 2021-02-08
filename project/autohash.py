# Analyze large batches of hash functions and write the ones with lowest variance to autohash table

# Import SQL from CS50
from cs50 import SQL

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///hashfun.db")

# Set parameters
for buckets in range(142971, 143092):
    for start in range(5381, 5382):
        for multiplier in range(33, 34):
            # Create array to count hash values output by function
            hash_count = []
            for i in range(buckets):
                hash_count.append(0)

            # Open dictionary and read lines
            with open("dictionary.txt", "r") as dictionary:
                for line in dictionary:
                    word = line.rstrip()

                    # Calculate hash value for each word and add 1 to hash_count table
                    hash = start
                    for char in word:
                        hash = hash * multiplier + ord(char)
                    hash = hash % buckets
                    hash_count[hash] += 1

            # Analyze hash_count for variance, empties, max, and min
            avg = 143091 / buckets
            variance = 0
            empty = 0
            max = hash_count[0]
            min = hash_count[0]
            for i in hash_count:
                if min != 0:
                    if i < min:
                        min = i
                if i > max:
                    max = i
                if i == 0:
                    empty += 1
                variance = variance + (i - avg) ** 2
            variance = variance / buckets

            # If variance < 1 write function to autohash table
            if variance < 1:
                db.execute("INSERT INTO autohash (start, multiplier, buckets, variance, empty, max, min) VALUES (:start, :multiplier, :buckets, :variance, :empty, :max, :min)",
                            start=start, multiplier=multiplier, buckets=buckets, variance=variance, empty=empty, max=max, min=min)