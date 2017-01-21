from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from helpers import *
import datetime     #added

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():    #THIS IS "GET" automatically
    
    id= session['user_id']
    
    db.execute("CREATE table IF NOT EXISTS portafolio(id INTEGER PRIMARY KEY AUTOINCREMENT, my_id INTEGER, stock TEXT, name TEXT, shares INTEGER, price REAL, date TEXT)");

    # select current id
    p = db.execute("SELECT stock,name,SUM(shares) AS shares,price, SUM(SHARES)*price AS total FROM  portafolio WHERE my_id=:id GROUP BY stock HAVING SUM(shares)>0", id= session['user_id'])

    overall_total= db.execute("SELECT SUM(shares * price) AS overall_total FROM portafolio WHERE my_id==:id", id= session['user_id'])
    
    
    if overall_total[0]['overall_total'] != None:
        overall_total[0]['overall_total'] = round(overall_total[0]['overall_total'],3)
    else:
        overall_total[0]['overall_total'] = 0
      
    
    cash_left = db.execute("SELECT cash from users WHERE id=:id", id=session['user_id'])
    cash_left[0]['cash'] = round(cash_left[0]['cash'],3)
        
    
    return render_template('index.html', p=p, overall_total=overall_total[0]['overall_total'], cash_left=cash_left[0]['cash'])

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template('buy.html')
    
    elif request.method == "POST":
        # check if it stock exists
        my_dict = lookup(request.form.get('stock'))
        if my_dict == None:
            return apology("Stock symbol is invalid.")
        
        num_shares = int(request.form.get('shares'))
        if num_shares < 0:
            return apology("Can't buy a negative number of shares")
        
        spended = num_shares * my_dict['price']     # (1 * 133.50)
        date = datetime.datetime.now()
        result = db.execute("SELECT cash from users WHERE id=:id", id= session['user_id'])
        
        if result[0]['cash'] < spended:  # if 10000 < (1 * 133.50)
            return apology("You don't have enough cash")
        
        # add a log to the table 'portafolio' 
        db.execute("INSERT INTO portafolio(my_id, stock, name, shares, price, date) VALUES(:my_id,:stock,:name,:shares,:price,:date)", 
                       my_id=session['user_id'], name= my_dict['name'], stock= my_dict['symbol'], 
                       shares=num_shares, price=my_dict['price'], date=date.strftime('%Y/%m/%d  %I:%M%p'));  
     
        # update cash from 'users' table
        
        db.execute("UPDATE users SET cash=cash-:spended WHERE id=:id", spended=spended,id= session['user_id']);
            
        return redirect(url_for('index'))


@app.route("/history")
@login_required
def history():
    p = db.execute("SELECT * from portafolio WHERE my_id=:id", id= session['user_id'])

    return render_template('history.html', p=p)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template('quote.html')
        
    elif request.method == "POST":
        # retrieve stock quote
        my_dict = lookup(request.form.get('stock'))
    
        if my_dict == None:
            return apology("Stock symbol is invalid.")
        
        return render_template("display.html", my_dict = my_dict)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "GET":
        return render_template("register.html") 
        
    elif request.method == "POST":
        if request.form.get('username')== '':
            return apology("Missing username")
        elif request.form.get('password') == '':   
            return apology("Missing password") 
    
        if request.form.get('password') != request.form.get('confirm_password'):
           return apology("Passwords don't match!") 
        
        hash = pwd_context.encrypt(request.form.get('password'))  
        result = db.execute('INSERT INTO users (username,hash) values(:username, :hash)',
                  username=request.form.get('username'), hash=hash)
        
        if not result:
            return apology("Username already exists!")
        
        #session["user_id"] = rows[0]["id"]     
        return redirect(url_for("index"))
        


   

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    
    if request.method == "GET":
        return render_template('sell.html')
    
    elif request.method == "POST":
        # check if it stock exists
        my_dict = lookup(request.form.get('stock'))
        if my_dict == None:
            return apology("Stock symbol is invalid.")
        
        num_shares = int(request.form.get('shares'))
        if num_shares < 0:
            return apology("Can't sell a negative number of shares")
        
        p = db.execute("SELECT stock,SUM(shares) AS shares from portafolio WHERE my_id=:id AND stock=:stock GROUP BY stock", 
                    id= session['user_id'], stock=my_dict['symbol'])
        if num_shares > p[0]['shares']:
            return apology("Selling too many shares!")
            
        spended = num_shares * my_dict['price']      #spended = 2 * 137.4
        date = datetime.datetime.now()
        
        # add a log to the table 'portafolio' 
        db.execute("INSERT INTO portafolio(my_id, stock, name, shares, price, date) VALUES(:my_id,:stock,:name,:shares,:price,:date)", 
                       my_id=session['user_id'], name= my_dict['name'], stock= my_dict['symbol'], 
                       shares=-num_shares, price=my_dict['price'], date=date.strftime('%Y/%m/%d  %I:%M%p'));  
        
        db.execute("UPDATE users SET cash=cash+:spended WHERE id=:id", spended=spended,id= session['user_id']);

    return redirect(url_for('index'))


@app.route("/account", methods=["GET","POST"])
@login_required
def account():
    if request.method == "GET":
        p = db.execute("SELECT cash,username FROM users WHERE id=:id", id= session['user_id'])
        return render_template('account.html', cash= round(p[0]['cash'],3), username= p[0]['username'])
        
    elif request.method == "POST":
        money = float(request.form.get('add_money'))
        db.execute("UPDATE users SET cash=cash+:money WHERE id=:id", money=money, id=session['user_id'])
        return redirect(url_for('account'))
    
            