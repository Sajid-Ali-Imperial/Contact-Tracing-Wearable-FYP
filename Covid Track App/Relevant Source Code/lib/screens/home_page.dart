import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:contact_tracing_app/widgets/device_widgets.dart';
import 'package:contact_tracing_app/firebase_backend/authentication_service.dart';
import 'package:contact_tracing_app/firebase_backend/database_services.dart';
import 'package:contact_tracing_app/device_manager/device_manager.dart';
import 'package:flutter_blue/flutter_blue.dart';


class HomePage extends StatefulWidget {
  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  bool connected = false;
  bool _cameIntoContact = false;
  int batteryLevel = 0;
  
  @override
  void initState() { 
    EventManager.instance.subscribe('connectedToDevice', onConnectedToDevice);
    //EventManager.instance.subscribe('failedToConnect', onConnectedToDevice);
    EventManager.instance.subscribe('onBatteryLevelUpdate', updateBatteryLevel);
    EventManager.instance.subscribe('onDisconnected', onDisconnectedDevice);
    super.initState(); 
  }

  @override
  void dispose() {
    EventManager.instance.unsubscribe('connectedToDevice', onConnectedToDevice);
    //EventManager.instance.unsubscribe('failedToConnect', onConnectedToDevice);
    EventManager.instance.unsubscribe('onBatteryLevelUpdate', updateBatteryLevel);
    EventManager.instance.subscribe('onDisconnected', onDisconnectedDevice);
    super.dispose();
  }

  void updateBatteryLevel(int newBatteryLevel){
    setState(() {
      batteryLevel = newBatteryLevel;
    });
  }

  void onConnectedToDevice()
  {
    setState(() {
      connected = true;
    });
  }

  void onDisconnectedDevice()
  {
    setState(() {
      connected = false;
    });
  }

  void moveToLinkPage()
  {
    setState(() {
      EventManager.instance.triggerEvent('onPageChanged', 2);
    });
  }

  void printUUID() async
  {
    Guid s = Guid('0000180f-0000-1000-8000-00805f9b34fb');
    Guid c = Guid('00002a19-0000-1000-8000-00805f9b34fb');
    print(s);
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        children: <Widget>[
          SizedBox(height: 30,),
          _cameIntoContact ? SelfIsolateWidget() : ContinueSocialDistancing(),
          SizedBox(height: 60),
          Center(
              //child: connected ? DeviceStatusWidget(DeviceManager.instance.deviceName, DeviceManager.instance.macAddress, batteryLevel) : EmptyConnection()
            child: connected ? DeviceStatusWidget() : EmptyConnection()
          ),
          SizedBox(height: 20),
          connected 
          ? Container()
          : GenericButton(
              backgroundColor: greenButtonColour, 
              text: "CONNECT",
              callback: moveToLinkPage,
              //callback: printUUID,
          ),
        ],
      ),
    );
  }
}






class SelfIsolateWidget extends StatelessWidget {

  SelfIsolateWidget(){}

  @override
  Widget build(BuildContext context) {
    return Stack(
      //alignment: Alignment.center,
      children: <Widget>[
        Center(
          child:Image(
            height: 270,
            width: 270,
            image: AssetImage('assets/red_circles.png'),
          ),
        ),
        Column(
          children: <Widget>[
            SizedBox(height: 35,),
            Padding(
              padding: EdgeInsets.fromLTRB(50, 0, 50, 0),
              child: Text(
                'You came into contact with someone who tested positive for COVID-19',
                textAlign: TextAlign.center,
                style: TextStyle(
                  color: h2_FontColour,
                  fontSize: 18.0,
                  fontWeight: FontWeight.w600,
                  //letterSpacing: 1.0
                ),
              ),
            ),
            SizedBox(height: 80,),
            Text(
              'Please Self Isolate!',
              textAlign: TextAlign.center,
              style: TextStyle(
                color: Colors.red,
                fontSize: 22.0,
                fontWeight: FontWeight.bold,
                //letterSpacing: 1.0
              ),
            ),
          ],
        ),
      ]
    );
  }
}


class ContinueSocialDistancing extends StatelessWidget {

  ContinueSocialDistancing(){}

  @override
  Widget build(BuildContext context) {
    return Stack(
      //alignment: Alignment.center,
      children: <Widget>[
        Center(
          child: Image(
            height: 270,
            width: 270,
            image: AssetImage('assets/green_circles.png'),
          ),
        ),
          Column(
            children: <Widget>[
              SizedBox(height: 35,),
              Padding(
                padding: EdgeInsets.fromLTRB(50, 0, 50, 0),
                child: Text(
                  'You have not come into contact with someone who tested positive for COVID-19',
                  textAlign: TextAlign.center,
                  style: TextStyle(
                    color: h2_FontColour,
                    fontSize: 18.0,
                    fontWeight: FontWeight.w600,
                    //letterSpacing: 1.0
                  ),
                ),
              ),
              SizedBox(height: 80,),
              Text(
                'Continue Social Distancing',
                textAlign: TextAlign.center,
                style: TextStyle(
                  color: Color(0xFF2FE357),
                  fontSize: 22.0,
                  fontWeight: FontWeight.bold,
                  //letterSpacing: 1.0
                ),
              ),
            ],
          ),
      ]
    );
  }
}