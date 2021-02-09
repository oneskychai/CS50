# Determines approximate grade level necessary to comprehend text

# Import necessary function
from cs50 import get_string

# Get text from user
text = get_string("Text: ")

# Set letter count to 0
# Set word count to 1
# Set sentence count to 0
let_count = 0
word_count = 1
sent_count = 0

# Count letters, words, and sentences in text
for c in text:
    if c.isalpha():
        let_count += 1
    if c == " ":
        word_count += 1
    if c == "." or c == "?" or c == "!":
        sent_count += 1

# Calculate Coleman-Liau index and round to nearest integer
L = 100 * let_count / word_count
S = 100 * sent_count / word_count
index = .0588 * L - .296 * S - 15.8
index = round(index)

# Output grade level
if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {index}")
