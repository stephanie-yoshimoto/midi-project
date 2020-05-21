from flask import Flask, jsonify, request

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        result_json = request.get_json()
        return jsonify({'sent': result_json}), 201
    else:
        # method is 'GET'
        return jsonify({'about': 'Hello World'})


@app.route('/multi/<int:num>', methods=['GET'])
def multiply(num):
    return jsonify({'result': num * 10})


# @app.route('/')
# def hello_world():
#     return 'Hello World!'


if __name__ == '__main__':
    app.run()
