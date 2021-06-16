import numpy as np
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
from os import path
import firebase_database as fb


"""
Define endpoint class which processes contact tracing queries and posts.
"""

contact_trace_get_ars = reqparse.RequestParser()
contact_trace_get_ars.add_argument("uid", type=str, help="Influencer or product name must be sent.", required=True)

successfully_return_data = {"Success": "Accessed REST API"}

class ContactTracingService(Resource):
    def get(self):
        # parse arguments
        args = contact_trace_get_ars.parse_args()
        return successfully_return_data


""" 
Add the updated contact tracing data to database and update the account which need to
socially isolate themselves because of close contact with someone who tested positive.
The 'data' has the following form:
    {
        date#1: {uid#1: closest, uid#2: closest, ... },
        date#2: {uid#3: closest, uid#1: closest, ... },
        date#3: {uid#2: closest, uid#5: closest, ... },
    }
"""
def updateContactTracing(uid, data):
    # get the existing data for this user
    new_dict = {}
    if uid in fb.contact_data.keys():
        new_dict = fb.contact_data[uid]
    for date in data.keys():
        new_dict[date] = data[date]
    # update the database
    fb.write_firestore('contact_tracing', 'contacts', new_dict)
