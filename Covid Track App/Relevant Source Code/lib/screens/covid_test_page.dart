import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/widgets/device_widgets.dart';
import 'package:contact_tracing_app/screens/tab_template.dart';

class CovidTestPage extends StatefulWidget {
  @override
  _CovidTestPageState createState() => _CovidTestPageState();
}

class _CovidTestPageState extends State<CovidTestPage> {

  double iconSF = 0.27;

  void newTestCallback()
  {
    setState(() {
      EventManager.instance.triggerEvent("onPageChanged", PageNav.CovidAddTest);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.fromLTRB(80, 0, 80, 0),
      child: Column(
        children: <Widget>[
          SizedBox(height: 70),
          Center(
            child: Container(
              height: 877 * iconSF,
              width: 356 * iconSF,
              child: FlatButton(
                onPressed: null,
                padding: EdgeInsets.all(0.0),
                child: Image(image: AssetImage('assets/CovidTestIcon.png')),
              ),
            ), 
          ),
          SizedBox(height: 40,),
          Text(
            'Help your community by reporting your diagnosis',
            textAlign: TextAlign.center,
            style: TextStyle(
              color: h2_FontColour,
              fontSize: 20.0,
              fontWeight: FontWeight.w500,
              //letterSpacing: 1.0
            ),
          ),
          SizedBox(height: 30,),
          Text(
            'Your report will be anonymous and your identity will be protected',
            textAlign: TextAlign.center,
            style: TextStyle(
              color: h2_FontColour,
              fontSize: 15.0,
              fontWeight: FontWeight.w400,
              //letterSpacing: 1.0
            ),
          ),
          SizedBox(height: 30,),
          GenericButtonWithIcon(
              Icon(Icons.add, color: Colors.white),
              text: 'NEW TEST',
              callback: this.newTestCallback,
              padding: 5.0,
          ),
          
        ],
      )
    );
  }
}