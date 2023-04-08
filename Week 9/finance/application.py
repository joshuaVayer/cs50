import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session, jsonify
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
    user_id = session['user_id']
    # Query the DB
    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)
    shares = db.execute("SELECT symbol, SUM(quantity) AS shares_total FROM shares "
                        "WHERE user_id = ? GROUP BY symbol", user_id)

    # Get the infos for the front end (all operations and total of invested cash)
    wallet = []
    invested = 0
    if len(shares) != 0:
        for share in shares:
            if share["shares_total"] > 0:
                answer = lookup(share["symbol"])
                answer["shares_total"] = share["shares_total"]
                answer["total"] = share["shares_total"]*answer["price"]
                wallet.append(answer)
                invested += share["shares_total"] * answer["price"]

    print(wallet)
    total = user[0]["cash"] + invested
    return render_template('index.html', user=user[0]["cash"], wallet=wallet, invested=invested, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Check the inputs
        if not request.form.get("symbol"):
            return apology("symbol is a required input", 400)
        if not request.form.get("shares") or not request.form.get("shares").isnumeric() or  int(request.form.get("shares")) < 1:
            return apology("please re-submit with valid quantity", 400)

        # Send request to the API and check the answer
        answer = lookup(request.form.get("symbol"))
        if answer is None:
            return apology("symbol is does not exist", 400)

        # Store the required infos in variables
        total = int(request.form.get("shares")) * answer['price']
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        quantity = int(request.form.get("shares"))

        new_cash = 0
        # Buying operation block
        if request.form.get("type") == "buy":
            # Make sure user has enough money in his account
            if cash[0]['cash'] < total:
                return apology("sorry, you don't have enough cash", 400)
            # Update user's cash amount
            new_cash += cash[0]['cash'] - total

        # Selling operation block
        if request.form.get("type") == "sell":
            # Make sure user has enough stocks to sell, first query the DB
            share = db.execute("SELECT SUM(quantity) AS share FROM shares "
                               "WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, answer['symbol'])
            if quantity > share[0]["share"]:
                return apology("you are trying to sell more stocks than you have", 400)
            # Update the variables for a selling operation
            new_cash += cash[0]['cash'] + total
            total = -abs(total)
            quantity = -abs(quantity)

        # Save the transaction
        db.execute("INSERT INTO shares (user_id, price, symbol, quantity, total, created_at) "
                   "VALUES (?, ?, ?, ?, ?, ?)",
                   user_id,
                   answer['price'],
                   answer['symbol'],
                   quantity,
                   total,
                   datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    # Query the DB for all operations made by the user and store them in operations
    user_id = session["user_id"]
    operations = db.execute("SELECT * FROM shares WHERE user_id = ? ORDER BY created_at DESC", user_id)
    return render_template("history.html", operations=operations)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
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
    # POST method
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("no symbol submitted", 400)
        answer = lookup(request.form.get("symbol"))
        # Check the answer
        if answer is None:
            return apology("this symbol does not exist", 400)
        # Load the result to the user
        return render_template("quoted.html", answer=answer)
    # GET method
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Check credentials
        if not request.form.get("username"):
            return apology("must provide username", 400)
        if not request.form.get("password"):
            return apology("must provide password", 400)
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        username = request.form.get("username")
        password = generate_password_hash(request.form.get("password"))

        # Check is the username is not taken
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 0:
            return apology("sorry username already exist", 400)

        # Insert the new user into the DB
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, password)
        return redirect("/")

    # For simple GET request
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":

        # Check the inputs
        if not request.form.get("symbol"):
            return apology("symbol is a required input", 400)
        if not request.form.get("shares") or not request.form.get("shares").isnumeric() or  int(request.form.get("shares")) < 1:
            return apology("please re-submit with valid quantity", 400)

        # Send request to the API and check the answer
        answer = lookup(request.form.get("symbol"))
        if answer is None:
            return apology("symbol is does not exist", 400)

        # Store the required infos in variables
        total = int(request.form.get("shares")) * answer['price']
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        quantity = int(request.form.get("shares"))

        new_cash = 0
        # Buying operation block
        if request.form.get("type") == "buy":
            # Make sure user has enough money in his account
            if cash[0]['cash'] < total:
                return apology("sorry, you don't have enough cash", 400)
            # Update user's cash amount
            new_cash += cash[0]['cash'] - total

        # Selling operation block
        if request.form.get("type") == "sell":
            # Make sure user has enough stocks to sell, first query the DB
            share = db.execute("SELECT SUM(quantity) AS share FROM shares "
                               "WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, answer['symbol'])
            if quantity > share[0]["share"]:
                return apology("you are trying to sell more stocks than you have", 400)
            # Update the variables for a selling operation
            new_cash += cash[0]['cash'] + total
            total = -abs(total)
            quantity = -abs(quantity)

        # Save the transaction
        db.execute("INSERT INTO shares (user_id, price, symbol, quantity, total, created_at) "
                   "VALUES (?, ?, ?, ?, ?, ?)",
                   user_id,
                   answer['price'],
                   answer['symbol'],
                   quantity,
                   total,
                   datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        return redirect("/")

    symbols = db.execute("SELECT symbol, SUM(quantity) AS share FROM shares "
                         "WHERE user_id = ? GROUP BY symbol", user_id)
    currents = []
    for symbol in symbols:
        if symbol["share"] > 0:
            currents.append(symbol["symbol"])
    return render_template("sell.html", currents=currents)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
