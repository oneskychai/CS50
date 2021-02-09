# Says hello to name input by user

# Import necessary function
from cs50 import get_string

# Get name and say hello, "name"
answer = get_string("What is your name?\n")
print(f"hello, {answer}")
