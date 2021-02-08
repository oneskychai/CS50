import os
import string

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///hashfun.db")

@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show users analyzed hash functions ordered by selected category"""

    id = session["user_id"]

    # User reached route via GET
    if request.method == "GET":

        rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY variance", id=id)

        # Render page
        if len(rows) == 0:
            flash("Analyze some functions!")
            return redirect("/analyze")
        else:
            return render_template("index.html", rows=rows, order="Variance ascending")

    # User reached route via POST
    else:
        order = request.form.get("order")
        if order == "start":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY start", id=id)
            order = "Start ascending"
        if order == "start DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY start DESC", id=id)
            order = "Start descending"
        if order == "multiplier":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY multiplier", id=id)
            order = "Multiplier ascending"
        if order == "multiplier DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY multiplier DESC", id=id)
            order = "Multiplier descending"
        if order == "buckets":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY buckets", id=id)
            order = "Buckets ascending"
        if order == "buckets DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY buckets DESC", id=id)
            order = "Buckets descending"
        if order == "variance":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY variance", id=id)
            order = "Variance ascending"
        if order == "variance DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY variance DESC", id=id)
            order = "Variance descending"
        if order == "empty":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY empty, variance", id=id)
            order = "Empties ascending"
        if order == "empty DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY empty DESC, variance", id=id)
            order = "Empties descending"
        if order == "max":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY max, variance", id=id)
            order = "Most ascending"
        if order == "max DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY max DESC, variance", id=id)
            order = "Most descending"
        if order == "min":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY min, variance", id=id)
            order = "Least ascending"
        if order == "min DESC":
            rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY min DESC, variance", id=id)
            order = "Least descending"
        return render_template("index.html", rows=rows, order=order)

@app.route("/background")
@login_required
def background():
    """Explain how it works"""

    # Render page
    return render_template("background.html")

@app.route("/analyze", methods=["GET", "POST"])
@login_required
def analyze():
    """Analyze hash function"""

    # User reached route via POST
    if request.method == "POST":

        # Make sure user input all values
        if not request.form.get("start"):
            return apology("must enter start number", 403)
        if not request.form.get("multiplier"):
            return apology("must enter multiplier", 403)
        if not request.form.get("buckets"):
            return apology("must enter number of buckets", 403)

        # Get input from user
        start = int(request.form.get("start"))
        multiplier = int(request.form.get("multiplier"))
        buckets = int(request.form.get("buckets"))

        # Check to see if function has already been analyzed
        rows = db.execute("SELECT * FROM functions JOIN users ON functions.user_id = users.id WHERE start = :start AND multiplier = :multiplier AND buckets = :buckets",
                            start=start, multiplier=multiplier, buckets=buckets)
        if len(rows) != 0:
            row = rows[0]

            # If analyzed by user change username to 'you'
            if row["user_id"] == session["user_id"]:
                row["username"] = "you"

            return render_template("analysis.html", row=row, analyzed=True)

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

        # Check to see if function is the best one yet
        rows = db.execute("SELECT variance FROM functions ORDER BY variance LIMIT 1")
        if len(rows) != 0:
            if variance < rows[0]["variance"]:
                flash("CONGRATULATIONS! This is the best hash function yet!")

        # Write function to functions table
        db.execute("INSERT INTO functions (user_id, start, multiplier, buckets, variance, empty, max, min) VALUES (:user_id, :start, :multiplier, :buckets, :variance, :empty, :max, :min)",
                    user_id=session["user_id"], start=start, multiplier=multiplier, buckets=buckets, variance=variance, empty=empty, max=max, min=min)

        # Render analysis.html
        return render_template("analysis.html", start=start, multiplier=multiplier, buckets=buckets, variance=variance, empty=empty, max=max, min=min, analyzed=False)

    # Else user reached route via GET
    else:
       return render_template("analyze.html")

@app.route("/history")
@login_required
def history():
    """Show users history of analyzed functions"""

    # Get analyzed functions for current user
    id = session["user_id"]
    rows = db.execute("SELECT * FROM functions WHERE user_id = :id ORDER BY timestamp", id=id)

    if len(rows) == 0:
        flash("You haven't analyzed any functions yet!")
        return redirect("/analyze")

    # Return user to history.html
    return render_template("history.html", rows=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET
    else:

        # Check to see if any registered users
        rows = db.execute("SELECT id FROM users LIMIT 1")
        if len(rows) == 0:
            return redirect("/register")

        return render_template("login.html")


@app.route("/newpassword", methods=["GET", "POST"])
@login_required
def newpassword():
    """Change users password"""

    # User reached route via POST
    if request.method == "POST":

        id = session["user_id"]

        # Ensure old password was entered
        if not request.form.get("old_password"):
            return apology("must enter old password", 403)

        # Ensure old password is correct
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=id)
        if not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("old password incorrect", 403)

        # Ensure new password entered
        if not request.form.get("new_password"):
            return apology("must enter new password", 403)

        # Ensure new password meets criteria
        password = request.form.get("new_password")
        specials = string.punctuation
        spec_chars = list(map(lambda char: char in specials, password))
        alphas = 0
        nums = 0
        for char in password:
            if char.isalpha():
                alphas += 1
            elif char.isdigit():
                nums += 1
        if not any(spec_chars) or alphas == 0 or nums == 0 or len(password) < 8:
            return apology("new password does not fit criteria", 403)

        # Ensure password confirmation entered and matches password
        if not request.form.get("confirm_new"):
            return apology("must confirm new password", 403)
        elif request.form.get("confirm_new") != password:
            return apology("new passwords do not match", 403)
        else:
            # Update user in database
            hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
            db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash, id=id)

            # Redirect user to index page
            flash('Password changed successfully!')
            return redirect("/")

    # User reached route via GET
    else:
        return render_template("newpassword.html")

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/best", methods=["GET", "POST"])
@login_required
def best():
    """Display best hash functions from all users"""

    # User reached route via POST
    if request.method == "POST":

        # Make sure number was entered
        if not request.form.get("number"):
            return apology("must enter number", 403)

        number = int(request.form.get("number"))
        rows = db.execute("SELECT * FROM functions JOIN users ON functions.user_id = users.id ORDER BY variance LIMIT :number", number=number)
        if len(rows) < number:
            number = len(rows)
        post = True
        return render_template("best.html", rows=rows, number=number, post=post)

    # Else user reached via GET
    else:
        rows = db.execute("SELECT id FROM functions LIMIT 1")
        if len(rows) == 0:
            flash("No functions analyzed yet! Be the first to check it out!")
            return redirect("/analyze")
        post = False
        return render_template("best.html", post=post)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure username is not already taken
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) != 0:
            return apology("username is already taken", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password is at least 8 characters with at least 1 digit, 1 letter, and 1 special character
        password = request.form.get("password")
        specials = string.punctuation
        spec_chars = list(map(lambda char: char in specials, password))
        alphas = 0
        nums = 0
        for char in password:
            if char.isalpha():
                alphas += 1
            elif char.isdigit():
                nums += 1
        if not any(spec_chars) or alphas == 0 or nums == 0 or len(password) < 8:
            return apology("Password does not fit criteria", 403)

        # Ensure password confirmation entered and matches password
        if not request.form.get("confirmation"):
            return apology("must confirm password", 403)
        elif request.form.get("confirmation") != password:
            return apology("passwords do not match", 403)
        else:
            # Add user to the database
            hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
            username = request.form.get("username")
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)

            # Remember user
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
            session["user_id"] = rows[0]["id"]

            # Redirect user to background page
            flash('You have registered successfully!')
            return redirect("/background")

    # Else user reached route via GET
    else:
        # Check to see if any registered users
        rows = db.execute("SELECT id FROM users LIMIT 1")
        if len(rows) == 0:
            flash("Be the first to register!")
        return render_template("register.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
