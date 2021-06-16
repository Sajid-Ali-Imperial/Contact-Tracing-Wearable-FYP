import threading
import firebase_admin
from datetime import datetime
from firebase_admin import credentials, firestore, storage
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
import database.contact_tracing_alg as ContactTracingAlg


# TODO:
# 1. Pull data from firebase
# 2. Store data in memory in an appropriate format
# 3. Automatically update and synchronise data with firebase


"""
The dictionaries contain all the covid_test data and interaction data.
"""
covid_test_data = {}
contact_data = {}
isolating_users = {}




"""
Use the 'ServiceAccountKey' to get certified to access firestore documents.
"""
_cred = credentials.Certificate("ServiceAccountKey.json")
_default_app = firebase_admin.initialize_app(_cred, {
    'storageBucket': 'ee4---contact-tracing-wearable.appspot.com'
})
_db = firestore.client()    # firestore reference
_bucket = storage.bucket()  # storage reference





def on_update_isolation_status():
    new_isolation_status = ContactTracingAlg.updateCovidStatus(covid_test_data, contact_data)   # find users that need to isolate themselves
    date = datetime.today().strftime('%d-%m-%Y')
    for user in new_isolation_status:
        user_data = read_firestore('users', user)
        if user_data != None:
            user_data['self_isolate'] = {'start_date': date, 'state': True}
            write_firestore('users', user, user_data)

"""
On startup, pull all the contract tracing data including covid_tests and closest interactions.
"""

# Create an Event for notifying main thread.
callback_done = threading.Event()

# Create a callback on_snapshot function to capture changes to the 'contacts' document
def on_contacts_update(doc_snapshot, changes, read_time):
    for doc in doc_snapshot:
        print(f'Received document snapshot: {doc.id}')
        contact_data = doc.to_dict()
    on_update_isolation_status()
    callback_done.set()

contact_doc_ref = _db.collection('contact_tracing').document('contacts')
contact_doc_watch = contact_doc_ref.on_snapshot(on_contacts_update)
doc_1 = contact_doc_ref.get()
if doc_1.exists:
    print('Got Contact Data')
    contact_data = doc_1.to_dict()
else:
    print("FAILED TO GET CONTACT DATA")

# Create a callback on_snapshot function to capture changes to the 'covid_tests' document
def on_covid_tests_update(doc_snapshot, changes, read_time):
    for doc in doc_snapshot:
        print(f'Received document snapshot: {doc.id}')
        covid_test_data = doc.to_dict()
    on_update_isolation_status()
    callback_done.set()

covid_tests_doc_ref = _db.collection('contact_tracing').document('covid_tests')
covid_tests_doc_watch = covid_tests_doc_ref.on_snapshot(on_covid_tests_update)
doc_2 = covid_tests_doc_ref.get()
if doc_2.exists:
    print('Got Covid Test Data')
    covid_test_data = doc_2.to_dict()
else:
    print("FAILED TO GET COVID_TESTS DATA")





"""
Firestore Functions:
Generic functions for accessing the Firestore Database including read and write operations.
"""
def write_firestore(collection, document, data):
    """ write to a firestore document by creating/overriding files.

    Arguments:
        collection: name of collected.
        document: name of document.
        data: a dictionary that will be converted to json format and saved.

    Returns:
        None.
    """
    _db.collection(collection).document(document).set(data)


def read_firestore(collection, document):
    """ Read a firestore document and return if it exists, else return None

    Arguments:
        collection: name of collected.
        document: name of document.

    Returns:
        dict or None
    """
    rslt = _db.collection(collection).document(document).get()
    if rslt.exists:
        return rslt.to_dict()
    return None










