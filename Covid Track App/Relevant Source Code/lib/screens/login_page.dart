import 'package:contact_tracing_app/screens/tab_template.dart';
import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:contact_tracing_app/firebase_backend/authentication_service.dart';

final double _inputFieldWidth = 300.0;
final double _inputFieldHeight =  40.0;

class LoginPage extends StatefulWidget {
  @override
  _LoginPageState createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {

  String _password = 'randomPassword';
  String _username = 'anonymous@gmail.com';
  bool pressedButton = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: backgroundColour,
      body: Column(
        children: <Widget>[
          Center(
            child: Container(
              //alignment: Alignment.centerRight,
              width: MediaQuery.of(context).size.width,
              height: MediaQuery.of(context).size.height * 0.66,
              //color: Colors.white,
              decoration: BoxDecoration(
                color: Colors.white,
                boxShadow: [
                  BoxShadow(
                    color: Colors.black12,
                    offset: Offset(0.0, 3.0),
                    blurRadius: 4.0,
                  ),
                ]
              ),
              child: SingleChildScrollView(
                child: Column(
                  mainAxisAlignment:  MainAxisAlignment.end,
                  children: <Widget>[
                    SizedBox(height: MediaQuery.of(context).size.height * 0.66*0.47),
                    Text(
                      appName,
                      style: TextStyle(
                        color: Colors.black,
                        fontSize: 32.0,
                        fontWeight: FontWeight.bold,
                        letterSpacing: 1.1
                      ),
                    ),
                    SizedBox(height: MediaQuery.of(context).size.height * 0.66*0.12),
                    Container(
                      width: _inputFieldWidth,
                      height: _inputFieldHeight,
                      child: TextFormField(
                        obscureText: true,
                        textAlignVertical: TextAlignVertical.bottom,
                        validator: (val) => val.isEmpty ? 'Must fill in field' : null,
                        decoration: _inputFieldDecoration(_username, Icons.mail),
                        onChanged: (val) {
                          _username = val;
                        },
                      ),
                    ),
                    SizedBox(height: MediaQuery.of(context).size.height * 0.66*0.05),
                    Container(
                      width: _inputFieldWidth,
                      height: _inputFieldHeight,
                      child: TextFormField(
                        obscureText: true,
                        textAlignVertical: TextAlignVertical.bottom,
                        validator: (val) => val.isEmpty ? 'Must fill in field' : null,
                        decoration: _inputFieldDecoration(_password, Icons.lock),
                        onChanged: (val) {
                          _password = val;
                        },
                      ),
                    ),
                    SizedBox(height: MediaQuery.of(context).size.height * 0.66*0.05),
                    Container(
                      child: Text(
                        'Forget Password?',
                        style: TextStyle(
                          color: textBoxColour,
                          fontSize: 15.0,
                          fontWeight: FontWeight.w700,
                          letterSpacing: 2
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ),
          SizedBox(height: MediaQuery.of(context).size.height * 0.33*0.4),
          Container(
            width: _inputFieldWidth,
            height: _inputFieldHeight,
            child: FlatButton(
              color: primaryColour,
              child: pressedButton
              ? SpinKitRing(
                color: Colors.white,
                size: 30.0,
              )
              : Text(
                'LOGIN',
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 16.0,
                  fontWeight: FontWeight.bold,
                  letterSpacing: 5
                ),
              ),
              onPressed: () async{
                setState(() {
                  pressedButton = true;
                });
                dynamic result = await AuthenticateService.instance.signInWithEmailAndPassword(_username, _password);
                if(result != null){
                  print('Logged in successfully');
                }
              },
            ),
          ),
        ],
      ),
    );
  }
}


// Input Field styling information
InputDecoration _inputFieldDecoration(String hintText, IconData fieldIcon) => 
    InputDecoration(
      //contentPadding: const EdgeInsets.symmetric(vertical: 8.0, horizontal: 50.0),
      enabledBorder: OutlineInputBorder(
        //borderRadius: BorderRadius.all(Radius.circular(20.0)),
        borderSide: BorderSide(color: textBoxColour, width: 1.5),
      ),
      hintText: hintText,
      hintStyle: TextStyle(
        color: textBoxColour,
      ),
      prefixIcon: Icon(
        fieldIcon,
        color: textBoxColour,
      ),
    );   



