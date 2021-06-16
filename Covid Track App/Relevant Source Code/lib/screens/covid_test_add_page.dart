import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:contact_tracing_app/widgets/device_widgets.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:contact_tracing_app/event_manager.dart';


class CovidTestPageAdd extends StatefulWidget {
  @override
  _CovidTestPageAddState createState() => _CovidTestPageAddState();
}

class _CovidTestPageAddState extends State<CovidTestPageAdd> {

  double iconSF = 0.27;
  bool positiveRslt = true;
  double buttonGap = 30;
  double buttonWidth;

  // @override
  // void initState() { 
  //   buttonWidth = ((MediaQuery.of(context).size.width * 0.85) - buttonGap) / 2.0;
  //   super.initState();  
  // }

  String getDateTime()
  {
    var now = new DateTime.now();
    var formatter = new DateFormat('dd/MM/yyyy');
    String formattedDate = formatter.format(now);
    return formattedDate;
  }

  void onPositiveButtonPress()
  {
    setState(() {
      positiveRslt = true;
    });
  }

  void onNegativeButtonPress()
  {
    setState(() {
      positiveRslt = false;
    });
  }

  void onSubmit()
  {

  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        SizedBox(height: 70),
        Container(
          width: MediaQuery.of(context).size.width * 0.85,
          height: MediaQuery.of(context).size.width * 0.85 * 0.18,
          decoration: BoxDecoration(
            // borderRadius: BorderRadius.only(
            //   topLeft: Radius.circular(8.0),
            //   topRight: Radius.circular(8.0),
            // ),
            border: Border(
              top: BorderSide(width: 3.0, color: Colors.black),
              bottom: BorderSide(width: 1.5, color: Colors.black),
              left: BorderSide(width: 3.0, color: Colors.black),
              right: BorderSide(width: 3.0, color: Colors.black),
            )
          ),
          child: Center(
            child: Text(
              'TEST DATE',
              style: TextStyle(
                color: Colors.black,
                fontSize: 30.0,
                fontWeight: FontWeight.w600,
                letterSpacing: 5
              ),
            ),
          )
        ),
        Container(
          width: MediaQuery.of(context).size.width * 0.85,
          height: MediaQuery.of(context).size.width * 0.85 * 0.18,
          decoration: BoxDecoration(
            // borderRadius: BorderRadius.only(
            //   bottomLeft: Radius.circular(8.0),
            //   bottomRight: Radius.circular(8.0),
            // ),
            color: primaryColour,
            border: Border(
              top: BorderSide(width: 1.5, color: Colors.black),
              bottom: BorderSide(width: 3.0, color: Colors.black),
              left: BorderSide(width: 3.0, color: Colors.black),
              right: BorderSide(width: 3.0, color: Colors.black),
            ),
          ),
          child: Center(
            child: Text(
              getDateTime(),
              style: TextStyle(
                color: Colors.white,
                fontSize: 30.0,
                fontWeight: FontWeight.w600,
                letterSpacing: 5
              ),
            ),
          )
        ),
        SizedBox(height: 40,),
        Padding(
          padding: EdgeInsets.fromLTRB(80, 0, 80, 0),
          child: Center(
            child: Text(
                'Your report will be anonymous and your identity will be protected',
                textAlign: TextAlign.center,
                style: TextStyle(
                  color: h2_FontColour,
                  fontSize: 15.0,
                  fontWeight: FontWeight.w400,
                  //letterSpacing: 1.0
                ),
              ),
          ),
        ),
        SizedBox(height: 40,),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            GenericButton(
              backgroundColor: positiveRslt ? primaryColour : Color(0xFFE1E1E1),
              text: 'POSITIVE',
              width: buttonWidth,
              height: 45,
              callback: onPositiveButtonPress,
            ),
            SizedBox(width: buttonGap),
            GenericButton(
              backgroundColor: !positiveRslt ? primaryColour : Color(0xFFE1E1E1),
              text: 'NEGATIVE',
              width: buttonWidth,
              height: 45,
              callback: onNegativeButtonPress,
            ),
          ],
        ),
        Expanded(
        child: Align(
          alignment: FractionalOffset.bottomCenter,
            child: Padding(
              padding: EdgeInsets.only(bottom: 40.0),
                child: //Your widget here,
                GenericButton(
                  backgroundColor: greenButtonColour,
                  text: 'SUBMIT',
                  width: MediaQuery.of(context).size.width * 0.85,
                  height: 45,
                  callback: onSubmit,
                ),
          ),
        ),
      ),
      ],
    );
  }
}