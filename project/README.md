For my final project, I created a web application designed to help programmers gain a clearer view of what is happening
behind the curtain when they are using hash functions to distribute data into a hash table. Back in problem set 5, we
used a hash function to distribute the 143091 words of a dictionary into a hash table in order to run a spell check program.
I searched online for a good hash function and came across the djb2 algorithm. I encountered a lot of conversation discussing
the desirable distribution output of the function, as well as a lot of inquiry into the significance of the numbers chosen
as start point and multiplier, 5381 and 33 respectively.  I found myself rather curious to get a deeper glimpse into how
changing these values would change the outcome of the hashing. I was also very curious about the optimal number of buckets
to set in the hash table.

When it came time to create a final project for CS50, this curiosity was still present and alive in my mind. I thought it
would be interesting to write a Python program that would comprehensively analyze the distribution properties of a large
swath of variations on the djb2 algorithm. I soon realized that the program I had in mind would take something on the order
of 1000 years to run in the CS50 IDE. So, I decided to do something more practical and fun instead.

I created a web applicaiton where users can register an account, sign in, and try out different values for the start point,
the multiplier, and the number of buckets in their hash function. The program then simulates the hashing of the 143091 word
dicitonary, calculating the expected number of words per bucket, the variance of the number of words per bucket, the number
of empty buckets, as well as the most and least number of words in any one bucket. Each function analyzed is then stored in
a database, so that users can view a history of all of the functions they have analyzed. Users can also sort all of their
analyzed functions however they choose, as well as view a list with length of their choosing of all of the functions with the
lowest variance from all users.

I also wrote the aforementioned Python program on the side and ran some test batches of functions through it to get a lay of
the land, so to speak.  I soon realized that a variance of about 1 was as good as it was going to get for the number of words
in each bucket. So I limited my search to functions with a variance below 1 and stored these functions in a separate table.
I discovered that the algorithm I chose for problem set 5 had a variance of 3.592, and that by changing the number of buckets,
I could get the variance down to below 1. Using the Python program I wrote, the lowest variance I found for any function was
.9871, and I quickly found over 1000 functions with variance less than 1.

I called my web application '#HASHfun' as a fun play on words. I used the distribution code from CS50 Finance as a start point
and modified it to meet my needs. I had a lot of fun working out all the little details and I learned a lot along the way. Some
of the aspects that stand out for me include the following: using Jinja to combine HTML files that I realized had a lot of
redundancy, using JavaScript to disable and enable buttons based on whether or not all input fields have been entered, and
repeatedly refining the logic of the routes to create a smooth flow that accounts for various outlier conditions.