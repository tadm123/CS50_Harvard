import sqlite3
from flask import Flask,render_template, url_for, redirect, request


app = Flask(__name__)
conn = sqlite3.connect('shoes.db', check_same_thread=False)
c = conn.cursor()

@app.route("/")
def main():
        return render_template('main.html')

@app.route("/search/", methods=["GET", "POST"])
def search():
    name = request.form.get("name")
    size = request.form.get("size")

    c.execute("SELECT * FROM shoes WHERE name LIKE ? AND sizes LIKE ? ORDER BY price",
                    ('%'+name+'%','%'+size+'%'))
    p = c.fetchall()

    url = [p[i][0] for i in range(len(p))]
    names = [p[i][1] for i in range(len(p))]
    prices = [p[i][2] for i in range(len(p))]
    sizes = [p[i][3] for i in range(len(p))]
    shoe_type = [p[i][4] for i in range(len(p))]

    return render_template('search.html' , url=url, names=names, prices=prices,
                           sizes=sizes, shoe_type=shoe_type)


if __name__ == '__main__':
    app.run(debug=True)
