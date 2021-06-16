import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/widgets/device_widgets.dart';
import 'package:contact_tracing_app/screens/tab_template.dart';
import 'package:contact_tracing_app/ble_connection/ble_manager.dart';
import 'package:contact_tracing_app/device_manager/device_manager.dart';

class LinkPage extends StatefulWidget {
  @override
  _LinkPageState createState() => _LinkPageState();
}

class _LinkPageState extends State<LinkPage> {

  double iconSF = 0.27;
  bool _scanning = false;
  bool _connected = false;

  @override
  void initState() { 
    EventManager.instance.subscribe('connectedToDevice', onSuccessfullyConnected);
    EventManager.instance.subscribe('failedToConnect', onFailedToConnect);
    super.initState(); 
  }

  @override
  void dispose() {
    EventManager.instance.unsubscribe('connectedToDevice', onSuccessfullyConnected);
    EventManager.instance.unsubscribe('failedToConnect', onFailedToConnect);
    super.dispose();
  }

  void discoverDevices()
  {
    setState(() {
      DeviceManager.instance.searchForDevices();
      _scanning = true;
    });
  }

  void onFailedToConnect()
  {
    setState(() {
      _scanning = false;
      _connected = false;
    });
  }

  void onSuccessfullyConnected()
  {
    setState(() {
      _scanning = false;
      _connected = true;
    });
  }

  void disconnect() async
  {
    await DeviceManager.instance.disconnectDevice();
    print("Completed Disconnect");
    setState(() {
      _connected = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.fromLTRB(80, 0, 80, 0),
      child: Column(
        children: <Widget>[
          SizedBox(height: 50),
          Center(
            child: Container(
              height: 843 * iconSF,
              width: 658 * iconSF,
              child: FlatButton(
                onPressed: null,
                padding: EdgeInsets.all(0.0),
                child: Image(image: AssetImage('assets/DeviceIcon.png')),
              ),
            ), 
          ),
          SizedBox(height: 40,),
          Text(
            'Turn on the device and place it nearby for a one time pairing procedure.',
            textAlign: TextAlign.center,
            style: TextStyle(
              color: h2_FontColour,
              //color: Color(0xFF540351),
              fontSize: 20.0,
              fontWeight: FontWeight.w500,
              //letterSpacing: 1.0
            ),
          ),
          SizedBox(height: 30,),
          Text(
            'Your data will be anonymous and your identity will be protected',
            textAlign: TextAlign.center,
            style: TextStyle(
              color: h2_FontColour,
              fontSize: 15.0,
              fontWeight: FontWeight.w400,
              //letterSpacing: 1.0
            ),
          ),
          SizedBox(height: 30,),
          !_connected
          ? GenericButton(
              text: 'DISCOVER',
              callback: this.discoverDevices,
              loadingCircle: _scanning,
          )
          :
          GenericButton(
              text: 'DISCONNECT',
              callback: this.disconnect,
              backgroundColor: Colors.red,
          )
        ],
      )
    );
  }
}