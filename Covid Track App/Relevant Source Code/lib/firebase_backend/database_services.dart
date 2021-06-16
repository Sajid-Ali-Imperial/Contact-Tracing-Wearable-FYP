import 'dart:io';
import 'dart:async';
import 'dart:convert'; 
import 'package:http/http.dart' as http;
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:contact_tracing_app/firebase_backend/authentication_service.dart';

/// Use this class to interact with the backend database.
/// 
/// Capable to pushing and pulling data from firebase.
class DatabaseService{
  
  final String localURL = 'http://10.0.2.2:5000/thermoPatch/algorithm';

  // Store an instance of the document
  Map<String, dynamic> cachedPositiveCases = {};

  DatabaseService()
  {
    print('CALLED STATIC CONSTRUCTOR');
    CollectionReference reference = Firestore.instance.collection('contact_tracing');
    reference.snapshots().listen((querySnapshot) {
      querySnapshot.documentChanges.forEach((change) {
        // update local copy of positive cases
        if(change.document.documentID == 'positive_cases')
        {
          cachedPositiveCases = change.document.data;
        }
      });
    });
  }

  /// Get instance if it exists, else create a new one.
  // Create static instance
  static DatabaseService _instance = new DatabaseService();
  static DatabaseService get instance => _instance;

  // Collection reference
  final CollectionReference userDataReference = Firestore.instance.collection('users');

  

  /// Initialise the database with a new user
  Future<void> createNewUser(String uid) async
  {
    Map<String, dynamic> initialData = {'DoB': '01-09-1954', 'email': 'anon@gmail.com', 'first_name': 'John',
                                        'surname': 'Smith', 'password': 'random', 'self_isolate': {'start_date':'','state':false},};
    return await userDataReference.document(uid).setData(initialData);
  }

  /// Override the user data 
  Future<void> updateUserData(String uid, Map<String, dynamic> data) async{
    return await userDataReference.document(uid).setData(data);
  }


  // Future<void> queryAPI(Map<String, dynamic> data) async{

  //   // wait for result from API call
  //   final responce = await http.post(
  //     Uri.encodeFull(localURL),
  //     body: jsonEncode(data),
  //     headers: {'Content-Type': 'application/json'},
  //   );
  //   print('Status Code: ${responce.statusCode}');
  // }

  // Future<Map<String, dynamic>> fetchDocument(String collectionName, String documentName, Map<String) async
  // {
  //   await Firestore.instance.collection(collectionName).document(documentName).setData(data);
  // }

  Future uploadData(String collectionName, String documentName, Map<String, dynamic> data) async
  {
    await Firestore.instance.collection(collectionName).document(documentName).setData(data);
  }


}

