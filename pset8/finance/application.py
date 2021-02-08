import os
import string

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Gather data from transactions table
    id = session["user_id"]
    rows = db.execute("SELECT symbol, name, SUM(shares), price FROM transactions WHERE user_id = :id GROUP BY symbol ORDER BY symbol", id=id)

    # Update prices for each stock
    # Delete row if SUM(shares) == 0
    n = len(rows)
    zeros = []
    for i in range(n):
        if rows[i]["SUM(shares)"] == 0:
            zeros.append(i)
            continue
        info = lookup(rows[i]["symbol"])
        rows[i]["price"] = info["price"]
    zeros.reverse()
    for i in zeros:
        rows.pop(i)

    # Get users cash amount from users table
    info = db.execute("SELECT cash FROM users WHERE id = :id", id=id)
    cash = info[0]["cash"]

    # Calculate total value of stocks and total assets
    assets = 0
    for row in rows:
        row["total"] = row["SUM(shares)"] * row["price"]
        assets += row["total"]

        #COnvert price and total to usd() format
        row["price"] = usd(row["price"])
        row["total"] = usd(row["total"])
    total_assets = cash + assets

    # Convert dollar amounts to usd() format
    cash = usd(cash)
    total_assets = usd(total_assets)

    # Render page
    return render_template("index.html", rows=rows, cash=cash, total_assets=total_assets)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Make sure a symbol was entered
        if not request.form.get("symbol"):
            return apology("must enter a ticker symbol", 403)

        # Lookup price of stock, make sure symbol is valid
        symbol = request.form.get("symbol")
        symbol = symbol.strip()
        quote = lookup(symbol)
        if quote == None:
            return apology("invalid symbol input", 403)

        # Get number of shares from form
        quant = int(request.form.get("shares"))

        # Access users cash amount from users table
        id = session["user_id"]
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=id)

        # Make sure user has enough cash for purchase
        price = quote["price"]
        cost = quant * price
        cash = rows[0]["cash"]
        if cost > cash:
            return apology("not enough cash on hand to purchase", 403)

        # Deduct sale amount from cash, update users table
        cash -= cost
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=id)

        # Log transaction in transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, name, shares, price) VALUES (:user_id, :symbol, :name, :shares, :price)",
                    user_id=id, symbol=quote["symbol"], name=quote["name"], shares=quant, price=price)

        # Redirect user to home page
        flash("Bought shares!")
        return redirect("/")

    # Else user reached route via GET
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get transactions for current user
    id = session["user_id"]
    rows = db.execute("SELECT symbol, shares, price, timestamp FROM transactions WHERE user_id = :id ORDER BY timestamp", id=id)

    # Change price in each row to usd() format
    for row in rows:
        row["price"] = usd(row["price"])

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
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":

        # Lookup quote for symbol input by user
        symbol = request.form.get("symbol")
        symbol = symbol.strip()
        quote = lookup(symbol)

        # Check to see if symbol is valid
        if quote == None:
            return apology("invalid symbol input", 403)

        else:
            return render_template("quoted.html", quote=quote)

    # Else user reached via GET
    else:
        return render_template("quote.html")

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

            # Redirect user to homepage
            flash('You have registered successfully!')
            return redirect("/")

    # Else user reached route via GET
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Make sure a symbol was entered
        if not request.form.get("symbol"):
            return apology("must enter a ticker symbol", 403)

        # Lookup price of stock
        quote = lookup(request.form.get("symbol"))

        # Get number of shares from form, convert to int
        quant = int(request.form.get("shares"))

        # Make sure user has enough shares to sell
        id = session["user_id"]
        symbol = quote["symbol"]
        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = :id AND symbol = :symbol GROUP BY symbol", id=id, symbol=symbol)
        if quant > rows[0]["SUM(shares)"]:
            return apology("you don't own enough shares for this sale", 403)

        # Access users cash amount from users table
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=id)
        cash = rows[0]["cash"]

        # Add sale amount to cash, update users table
        price = quote["price"]
        value = quant * price
        cash += value
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=id)

        # Log transaction in transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, name, shares, price) VALUES (:user_id, :symbol, :name, :shares, :price)",
                    user_id=id, symbol=quote["symbol"], name=quote["name"], shares=-quant, price=price)

        # Redirect user to home page
        flash("Sold shares!")
        return redirect("/")

    # Else user reached route via GET
    else:
        # Create list of owned stocks
        id = session["user_id"]
        owned = []
        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = :id GROUP BY symbol ORDER BY symbol", id=id)
        for row in rows:
            if row["SUM(shares)"] == 0:
                continue
            owned.append(row["symbol"])

        # Return sell.html
        return render_template("sell.html", owned=owned)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
