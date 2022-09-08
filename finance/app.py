import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# For displaying dates of buy & sell actions
import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# export API_KEY=pk_7b5b94b83b1a4c159b191c3466b46e8b

# Root page
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Check if user is logged in
    if session["user_id"] != None:
        # Get user's stocks
        stocks = db.execute("SELECT DISTINCT symbol FROM history WHERE person_id = :id", id=session["user_id"])
        # List of symbols, list of names & a list of the prices of shares
        symbols = []
        stock_names = []
        stock_prices = []
        # Keeps track of how many stocks the user has
        stock_count = 0
        # Total holding value
        total = 0
        # Get the price of 1 share for each stock
        for counter in range(len(stocks)):
            # Iterate through each symbol
            stock = stocks[counter]["symbol"]
            # Check if symbol is valid
            if not lookup(stock):
                # Return apology if invalid
                return apology("invalid symbol")
            else:
                # Add the symbol, name & price of each stock to their corresponding lists
                quote = lookup(stock)
                stock_symbol = quote["symbol"]
                stock_price = quote["price"]
                stock_name = quote["name"]
                symbols.append(stock_symbol)
                stock_names.append(stock_name)
                stock_prices.append(stock_price)
                stock_count += 1
        # Get total number of shares & total value of each holding for each stock
        each_share = []
        each_holding = []
        # Iterate through stocks
        for counter in range(len(stocks)):
            # Get symbol
            symbol = stocks[counter]["symbol"]
            # Look for shares & holding under this symbol
            shares = db.execute("SELECT sum(shares) FROM history WHERE symbol = :symbol AND person_id = :id",
                                symbol=symbol, id=session["user_id"])
            holding = db.execute("SELECT sum(price) FROM history WHERE symbol = :symbol AND person_id = :id",
                                 symbol=symbol, id=session["user_id"])
            # Append to respective lists
            each_share.append(shares[0]["sum(shares)"])
            each_holding.append(holding[0]["sum(price)"])
        # Total $$$
        for counter in range(len(each_holding)):
            total += each_holding[counter]
        # Check user's funds
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        user_funds = cash[0]["cash"]
        # Add to total
        total += user_funds
        # Render
        return render_template("index.html", stocks=symbols, names=stock_names, shares=each_share, price=stock_prices, holding=each_holding, count=stock_count, cash=user_funds, total=total)

    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Get sysmbol
        symbol = request.form.get("symbol")
        # Check if shares requested is an integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Invalid request", 400)
        # Check for empty fields
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("empty field")
        # Check symbol validity
        if not lookup(symbol):
            return apology("invalid symbol")
        # Check for valid number of shares
        if shares <= 0:
            return apology("invalid number of shares", 400)
        # Get user's wallet
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        user_funds = cash[0]["cash"]
        # Lookup quote of symbol
        quote = lookup(symbol)
        # Calculate price, price of 1 share * number of shares the user wants
        price = quote["price"] * shares
        # If price exceeds user's funds, cancel request
        if price > user_funds:
            return apology("insufficient funds", 403)
        # Purchase the shares & record transaction
        else:
            user_funds -= price
            # make a new SQL table with a history for purchases
            db.execute("INSERT INTO history (symbol,shares,price,person_id) VALUES (?,?,?,?)",
                       symbol, shares, price, session["user_id"])
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=user_funds, id=session["user_id"])
            # Get current time
            now = datetime.datetime.now()
            date = now.strftime("%d-%m-%Y %H:%M:%S")
            # insert into action history
            db.execute("INSERT INTO actions (symbol,shares,price,action_type,time_of,person_id) VALUES (?,?,?,?,?,?)",
                       symbol, shares, price, "buy", date, session["user_id"])
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Put all data into arrays so i can easily iterate over them in jinja
    symbols = db.execute("SELECT symbol FROM actions WHERE person_id = :id", id=session["user_id"])
    shares = db.execute("SELECT shares FROM actions WHERE person_id = :id", id=session["user_id"])
    price = db.execute("SELECT price FROM actions WHERE person_id = :id", id=session["user_id"])
    action = db.execute("SELECT action_type FROM actions WHERE person_id = :id", id=session["user_id"])
    time = db.execute("SELECT time_of FROM actions WHERE person_id = :id", id=session["user_id"])
    count = db.execute("SELECT COUNT(symbol) FROM actions WHERE person_id = :id", id=session["user_id"])
    count = count[0]["COUNT(symbol)"]
    stock_symbol = []
    stock_shares = []
    stock_price = []
    stock_action = []
    stock_time = []
    for counter in range(len(symbols)):
        stock_symbol.append(symbols[counter]["symbol"])
        stock_shares.append(shares[counter]["shares"])
        stock_price.append(int(price[counter]["price"]))
        stock_action.append(action[counter]["action_type"])
        stock_time.append(time[counter]["time_of"])
    # Render
    return render_template("history.html", symbol=stock_symbol, shares=stock_shares, price=stock_price, action=stock_action, time=stock_time, count=count)


@app.route("/clear", methods=["GET", "POST"])
def clear():
    """Clear History"""
    if request.method == "POST":
        db.execute("DELETE FROM actions WHERE person_id = :id", id=session["user_id"])
    return redirect("/history")


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
    if request.method == "POST":
        # Get symbol and look up quote
        quote = request.form.get("symbol")
        quoted = quote
        # Check if it's a valid symbol
        if not lookup(quote):
            return apology("invalid symbol")
        # If it is -> display the quote to the user
        else:
            quoted = lookup(quote)
            return render_template("quoted.html", quoted=quoted)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Check if any of the fields are blank
        if not request.form.get("username"):
            return apology("must proide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)
        # Check if user doesn't exist
        row = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(row) != 0:
            return apology("user already exists", 400)
        # Check if passwords match
        passw = request.form.get("password")
        passw_confirm = request.form.get("confirmation")
        if passw != passw_confirm:
            return apology("passwords do not match", 400)
        # Get username and hash password
        user = request.form.get("username")
        passw_hash = generate_password_hash(passw)
        # Add user to database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", user, passw_hash)
        row = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        session["user_id"] = row[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # get user's previously bought stocks
        prev_buy_symbol = db.execute("SELECT symbol FROM history WHERE person_id = :id", id=session["user_id"])
        # get the total count of these symbols
        prev_buy_count = db.execute("SELECT COUNT(symbol) FROM history WHERE person_id = :id", id=session["user_id"])
        prev_buy_count = prev_buy_count[0]["COUNT(symbol)"]
        # append every symbol to stocks_bought
        stocks_bought = []
        for i in range(prev_buy_count):
            stocks_bought.append(prev_buy_symbol[i]["symbol"])
        # Get symbol & no. of shares user entered into form
        symbol = request.form.get("symbol")
        # Check if shares is an integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Invalid request", 400)
        # VALIDITY STUFF
        # Check for empty fields
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("empty field")
        # Check symbol validity
        if not lookup(symbol):
            return apology("invalid symbol")
        # Check if user even bought this stock at all
        if symbol not in stocks_bought:
            return apology("you have not bought that stock")
        # Check for valid number of shares & check if user also has that many shares
        share_count = db.execute("SELECT shares FROM history WHERE symbol = :symbol", symbol=symbol)
        share_count = share_count[0]["shares"]
        if shares <= 0 or shares > share_count:
            return apology("invalid number of shares", 400)
        # Get user's wallet
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        user_funds = cash[0]["cash"]
        # Lookup quote of symbol
        quote = lookup(symbol)
        # Calculate price, price of 1 share * number of shares the user wants to sell
        price = quote["price"] * shares
        # Sell
        user_funds += price
        share_count -= shares
        # update user wallet
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=user_funds, id=session["user_id"])
        # update shares
        # if count of shares > 0, just update the number of shares
        if share_count > 0:
            db.execute("UPDATE history SET shares = :share_count WHERE symbol = :symbol", share_count=share_count, symbol=symbol)
        # if less than 0, just delete the record completely
        else:
            db.execute("DELETE FROM history WHERE symbol = :symbol", symbol=symbol)
        # Get current time
        now = datetime.datetime.now()
        date = now.strftime("%d-%m-%Y %H:%M:%S")
        # insert into action history
        db.execute("INSERT INTO actions (symbol,shares,price,action_type,time_of,person_id) VALUES (?,?,?,?,?,?)",
                   symbol, shares, price, "sell", date, session["user_id"])

        return redirect("/")
    else:
        return render_template("sell.html")
