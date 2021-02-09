# Determines if a number input by user is valid Amex, Visa, or Mastercard

# Import necessary library functions
from cs50 import get_string
from sys import exit

# Prompt user for number and obtain number length
number = get_string("Number: ")
n = len(number)

# Check for proper number length
if n < 13 or n > 16 or n == 14:
    print("INVALID")
    exit(0)

# Create list and populate with digits from number
num = []
for c in number:
    num.append(c)

# Cast each digit in list to an int
for i in range(n):
    num[i] = int(num[i])

# Set sum to 0
sum = 0

# Calculate sum for Luhn's algorithm
for i in range(n):
    if i % 2 == 0:
        sum += num[n - i - 1]
    elif num[n - i - 1] < 5:
        sum += 2 * num[n - i - 1]
    else:
        sum += (2 * num[n - i - 1]) % 10 + 1

# Check to see if sum is a multiple of 10
if sum % 10 != 0:
    print("INVALID")
    exit(0)

# Check number against card criteria
if (n == 13 or n == 16) and num[0] == 4:
    print("VISA")
    exit(0)
elif (n == 15 and num[0] == 3 and (num[1] == 4 or num[1] == 7)):
    print("AMEX")
    exit(0)
elif (n == 16 and num[0] == 5 and num[1] > 0 and num[1] < 6):
    print("MASTERCARD")
    exit(0)
else:
    print("INVALID")
