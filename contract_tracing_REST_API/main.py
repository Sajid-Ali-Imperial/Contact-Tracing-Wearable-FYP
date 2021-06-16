from os import path, listdir

from flask import Flask, jsonify, request
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
from api_services.contact_tracing import ContactTracingService
import firebase_database as fb


# TODO:
# 1. Implement request parsing.
# 2. Create outline for validate request.
# 3. Create outline for product request.

# Tutorial:
# https://www.youtube.com/watch?v=GMppyAPbLYk

app = Flask(__name__)
api = Api(app)



# api.add_resource(InfluencerValidation, "/influencer_validation/<string:name>")
api.add_resource(ContactTracingService, "/contact_tracing_service")



if __name__ == '__main__':
    app.run(debug=False)
