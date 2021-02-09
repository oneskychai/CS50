# Prints two pyramids of height input by user

# Import necessary function
from cs50 import get_int

# Get height between 1 and 8 inclusive
while True:
    n = get_int("Height: ")
    if n > 0 and n < 9:
        break

# Print pyramids
for i in range(n):
    print(" " * (n - i - 1) + "#" * (i + 1) + "  " + "#" * (i + 1))
