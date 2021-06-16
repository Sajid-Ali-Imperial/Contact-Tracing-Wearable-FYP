import 'dart:html';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/firebase_backend/database_services.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:intl/intl.dart';

// TODO:
// Listen to changes in positive_tests document.
// Interpret these changes to update covid status.


class ContactTracingAlg {

  
  /// Add a Positive Test Result
  /// Start by creating a map object that contains the cut-off date of infection along
  /// with all the uids that were used during that period.
  Future addPositiveTest(List<String> positiveIds) async
  {
    DateFormat formatter = DateFormat('dd-MM-yyyy');
    String formattedDate = formatter.format(DateTime.now());
    
    Map<String, List<String>> data = DatabaseService.instance.cachedPositiveCases;
    if(data.containsKey(formattedDate))
    {
      data[formattedDate] = data[formattedDate] + positiveIds;
    }
    else
    {
      data[formattedDate] = positiveIds;
    }   
    await DatabaseService.instance.uploadData('contact_tracing', 'positive_cases', data);
  }

  bool checkContact()
  {

  }

}