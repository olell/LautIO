"""
LautIO development test server based on flask
"""

from flask import Flask
from flask import send_from_directory
from flask import jsonify

from flask_sockets import Sockets

from json import load as load_json
from json import loads as load_json_string

app = Flask(__name__, static_folder=None)
sockets = Sockets(app)

@app.route("/static/<path:path>")
def serve_static(path):
    print(path)
    return send_from_directory("data/web/", path)

@app.route("/")
def serve_index():
    return send_from_directory("data/web/html", "index.html")

@app.route("/about")
def serve_about():
    return send_from_directory("data/web/html", "about.html")

@app.route("/dsp/get_structure")
def serve_dsp_structure():
    with open("data/dsp/structure.json", "r") as target:
        structure = load_json(target)
    return jsonify(structure)

@sockets.route("/ws")
def websocket_route(ws):
    while not ws.closed:
        message = ws.receive()
        data = load_json_string(message)

        print(data)

        if (data["command"] == "test"):
            ws.send("{\"status\": \"ok\", \"message\": \"test command received!\"}")
            


if __name__ == "__main__":
    from gevent import pywsgi
    from geventwebsocket.handler import WebSocketHandler
    server = pywsgi.WSGIServer(('0.0.0.0', 80), app, handler_class=WebSocketHandler)
    server.serve_forever()