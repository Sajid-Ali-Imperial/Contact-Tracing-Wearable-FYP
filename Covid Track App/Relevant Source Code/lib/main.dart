import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:contact_tracing_app/screens/login_page.dart';
import 'package:contact_tracing_app/screens/tab_template.dart';
import 'package:contact_tracing_app/firebase_backend/authentication_service.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return StreamProvider<FirebaseUser>.value(
      value: AuthenticateService.instance.user,
      initialData: null,
      child: MaterialApp(
        title: 'Flutter Demo',
        theme: ThemeData(
          //fontFamily: 'Caros',
          fontFamily: 'Roboto',
        ),
        home: AuthenticationParent(),
        //home: LoginPage()
      ),
    );
  }
}

class AuthenticationParent extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    // get user stream value from parent
    final user = Provider.of<FirebaseUser>(context);
    if(user == null){
      return LoginPage();
    }
    else{
      //DatabaseService(uid: user.uid).updateUserData();
      print(user);
      return TabTemplate();
    }
  }
}


