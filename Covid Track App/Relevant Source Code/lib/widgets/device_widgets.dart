import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:dotted_border/dotted_border.dart';
import 'package:contact_tracing_app/event_manager.dart';

// class DeviceStatusWidget extends StatelessWidget {

//   Color light_blue = Color(0xFF598ede);
//   Color light_blue_text = Color(0xFF99b9ea);

//   String _deviceName;
//   String _macAddress;
//   bool _bondingState;
//   double _batteryLevel = 10.0;

//   DeviceStatusWidget(String deviceName, String macAddres, int batteryLevel){
//     this._deviceName = deviceName;
//     this._macAddress = macAddres;
//     this._bondingState = true;
//     this._batteryLevel = batteryLevel.toDouble();
//   }

//   @override
//   Widget build(BuildContext context) {
//     return Column(
//       children: <Widget>[
//         Container(
//           width: MediaQuery.of(context).size.width * 0.85,
//           height: widget_height,
//           decoration: BoxDecoration(
//             color: primaryColour,
//             borderRadius: _bondingState ? BorderRadius.only(
//               topLeft: Radius.circular(8.0),
//               topRight: Radius.circular(8.0),
//             ) : BorderRadius.all(Radius.circular(8.0)),
//           ),
//           child: Row(
//             children: <Widget>[
//               SizedBox(width: 0.125*widget_height),
//               Stack(
//                 alignment: Alignment.center,
//                 children: <Widget>[
//                   Container(
//                     width: widget_height *0.75,
//                     height: widget_height *0.75,
//                     decoration: BoxDecoration(
//                       color: light_blue,
//                       shape: BoxShape.circle,
//                     ),
//                   ),
//                   Text(
//                     '?',
//                     style: TextStyle(
//                       color: Colors.white,
//                       fontSize: 56.0,
//                       fontWeight: FontWeight.bold,
//                     ),
//                   ),
//                 ],
//               ),
//               SizedBox(width: 0.125*widget_height),
//               Padding(
//                 padding: EdgeInsets.fromLTRB(0, 0.125*widget_height, 0, 0.125*widget_height),
//                 child: 
//                   Column(
//                     crossAxisAlignment: CrossAxisAlignment.start,
//                     children: <Widget>[
//                       SizedBox(height: 0.125*widget_height),
//                       Text(
//                         _deviceName,
//                         textAlign: TextAlign.left,
//                         style: TextStyle(
//                           color: Colors.white,
//                           fontSize: 15.0,
//                           fontWeight: FontWeight.w600,
//                         ),
//                       ),
//                       SizedBox(height: 5),
//                       Text(
//                         _macAddress,
//                         style: TextStyle(
//                           color: light_blue_text,
//                           fontSize: 11.0,
//                           fontWeight: FontWeight.normal,
//                         ),
//                       ),
//                       SizedBox(height: 3),
//                       Text(
//                         _bondingState ? 'BONDED' : 'NOT BONDED',
//                         textAlign: TextAlign.left,
//                         style: TextStyle(
//                           color: light_blue_text,
//                           fontSize: 11.0,
//                           fontWeight: FontWeight.normal,
//                         ),
//                       ),
//                     ],
//                   ),
//               ),
//               SizedBox(width: 0.3*widget_height),
//               Column(
//                 mainAxisAlignment: MainAxisAlignment.center,
//                 children: <Widget>[
//                   IconButton(
//                     icon: Image.asset('assets/ble_icon.png'),
//                     iconSize: 30,
//                     onPressed: () {},
//                   ),
//                   SizedBox(height: 2),
//                   Text(
//                     _bondingState ? 'UNLINK' : 'LINK',
//                     textAlign: TextAlign.left,
//                     style: TextStyle(
//                       color: _bondingState ? Color(0xFFFF6262) : Color(0xFF46FF7A),
//                       fontSize: 11.0,
//                       fontWeight: FontWeight.normal,
//                     ),
//                   ),
//                 ],
//               ),
//             ],
//           ),  
//         ),
//         !_bondingState ? Container() :
//         Container(
//           width: MediaQuery.of(context).size.width * 0.85,
//           height: widget_height * 0.65,
//           decoration: BoxDecoration(
//             color: Color(0xFFd6e5fb),
//             borderRadius: BorderRadius.only(
//               bottomLeft: Radius.circular(8.0),
//               bottomRight: Radius.circular(8.0),
//             ),
//           ),
//           child: Padding(
//             padding: EdgeInsets.fromLTRB(widget_height*0.125, 0, widget_height*0.125, 0),
//             child: Column(
//               crossAxisAlignment: CrossAxisAlignment.start,
//               children: <Widget>[
//                 SizedBox(height: widget_height * 0.6 * 0.15),
//                 Text(
//                   'BATTERY',
//                   textAlign: TextAlign.left,
//                   style: TextStyle(
//                     color: primaryColour,
//                     fontSize: 12.0,
//                     fontWeight: FontWeight.w600,
//                   ),
//                 ),
//                 SizedBox(height: widget_height * 0.6 * 0.1),
//                 Stack(
//                   alignment: Alignment.center,
//                   children: <Widget>[
//                     Container(
//                       height: widget_height * 0.6 * 0.3,
//                       //width: MediaQuery.of(context).size.width * 0.85 * 0.9,
//                       child: LinearProgressIndicator(
//                         value: _batteryLevel / 100.0,
//                         semanticsLabel: 'Linear progress indicator',
//                         valueColor: AlwaysStoppedAnimation<Color>(primaryColour),
//                       ),
//                     ),
//                     Text(
//                       _batteryLevel.toInt().toString() + '%',
//                       textAlign: TextAlign.left,
//                       style: TextStyle(
//                         color: Colors.white,
//                         fontSize: 12.0,
//                         fontWeight: FontWeight.w600,
//                       ),
//                     ),
//                   ],
//                 ),
//               ],
//             ),
//           ),
          
//         ),
//       ],
//     );
    
//   }
// }

class DeviceStatusWidget extends StatelessWidget {

  Color light_blue = Color(0xFF598ede);
  Color light_blue_text = Color(0xFF99b9ea);
  bool _loadingCircle = false;
  double _batteryLevel = 10.0;

  DeviceStatusWidget({int batteryLevel=25}){
    this._batteryLevel = batteryLevel.toDouble();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        Container(
          width: MediaQuery.of(context).size.width * 0.85,
          height: 110,
          decoration: BoxDecoration(
            color: Colors.white,
            borderRadius: BorderRadius.only(
              topLeft: Radius.circular(8.0),
              topRight: Radius.circular(8.0),
            ),
            boxShadow: [
              BoxShadow(
                color: Colors.black12,
                offset: Offset(0.0, 0.0),
                blurRadius: 4.0,
              ),
            ]
          ),
          child: Row(
            children: <Widget>[
              SizedBox(width: 0.125*widget_height),
              Padding(
                padding: EdgeInsets.fromLTRB(0, 0.125*widget_height, 0, 0.125*widget_height),
                child: 
                  Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: <Widget>[
                      SizedBox(height: 0.125*widget_height),
                      Text(
                        'CONNECTED DEVICE',
                        textAlign: TextAlign.left,
                        style: TextStyle(
                          color: primaryColour,
                          fontSize: 15.0,
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                      SizedBox(height: 5),
                      Text(
                        'BATTERY',
                        textAlign: TextAlign.left,
                        style: TextStyle(
                          color: light_blue_text,
                          fontSize: 12.0,
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                      SizedBox(height: widget_height * 0.6 * 0.1),
                      Stack(
                        alignment: Alignment.center,
                        children: <Widget>[
                          Container(
                            height: widget_height * 0.6 * 0.5,
                            width: MediaQuery.of(context).size.width * 0.85 * 0.9,
                            child: LinearProgressIndicator(
                              value: _batteryLevel / 100.0,
                              semanticsLabel: 'Linear progress indicator',
                              valueColor: AlwaysStoppedAnimation<Color>(primaryColour),
                              backgroundColor: Color(0xFFC9DEFF),
                            ),
                          ),
                          Text(
                            _batteryLevel.toInt().toString() + '%',
                            textAlign: TextAlign.left,
                            style: TextStyle(
                              color: Colors.white,
                              fontSize: 12.0,
                              fontWeight: FontWeight.w600,
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
              ),
            ],
          ),  
        ),
        Container(
          width: MediaQuery.of(context).size.width * 0.85,
          height: 40,
          child:FlatButton(
            color: primaryColour,
            shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.only(
                  bottomLeft: Radius.circular(8.0),
                  bottomRight: Radius.circular(8.0),
                ),
            ),
            child: _loadingCircle
              ? SpinKitRing(
                color: Colors.white,
                size: 30.0,
              )
              : Text(
                'PULL DATA',
                textAlign: TextAlign.center,
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 14.0,
                  fontWeight: FontWeight.w600,
                  letterSpacing: 5
              ),
            ),
            onPressed: () {
              // add on pressed funcitonality
            },
          ),
          
        ),
      ],
    );
    
  }
}

class EmptyConnection extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return Container(
      width: MediaQuery.of(context).size.width * 0.85,
      height: widget_height * 1.65,
      child: DottedBorder(
        color: textBoxColour,
        strokeWidth: 3,
        dashPattern: [8, 4],
        child: Center(
          child: Text(
            'NO DEVICE CONNECTED',
            style: TextStyle(
              color: textBoxColour,
              fontSize: 20,
              fontWeight: FontWeight.w600
            ),
          ),
        )    
      ),
    );
  }
}



class GenericButton extends StatelessWidget {

  Color _backgroundColor;
  Color _textColor;
  String _text;
  Function _callback;
  bool _loadingCircle;
  double _height;
  double _width;

  GenericButton({Color backgroundColor=primaryColour, Color textColor=Colors.white, 
                String text="Button", Function callback=null, bool loadingCircle=false,
                double height=40, double width=180 }){
    this._backgroundColor = backgroundColor;
    this._textColor = textColor;
    this._text = text;
    this._callback = callback;
    this._loadingCircle = loadingCircle;
    this._height = height;
    this._width = width;
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: _width,
      height: _height,
      child: FlatButton(
        color: _backgroundColor,
        shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(5.0),
        ),
        child: _loadingCircle
          ? SpinKitRing(
            color: Colors.white,
            size: 30.0,
          )
          : Text(
            _text,
            textAlign: TextAlign.center,
            style: TextStyle(
              color: _textColor,
              fontSize: 14.0,
              fontWeight: FontWeight.w600,
              letterSpacing: 5
          ),
        ),
        onPressed: () async{
          if(_callback != null)
          {
            _callback.call();
          }
        },
      ),
    );
  }
}

class GenericButtonWithIcon extends StatelessWidget {

  Color _backgroundColor;
  Color _textColor;
  String _text;
  Function _callback;
  bool _loadingCircle;
  Icon _icon;
  double _padding; 

  GenericButtonWithIcon(Icon icon, {Color backgroundColor=primaryColour, Color textColor=Colors.white, 
                          String text="Button", Function callback=null, bool loadingCircle=false, double padding=0}){
    this._backgroundColor = backgroundColor;
    this._textColor = textColor;
    this._text = text;
    this._callback = callback;
    this._loadingCircle = loadingCircle;
    this._icon = icon;
    this._padding = padding;
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 180,
      height: 40,
      child: FlatButton(
        color: _backgroundColor,
        shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8.0),
        ),
        child: _loadingCircle
          ? SpinKitRing(
            color: Colors.white,
            size: 30.0,
          )
          : Row(
            children: <Widget>[
              _icon,
              SizedBox(width: _padding),
              Text(
                _text,
                textAlign: TextAlign.center,
                style: TextStyle(
                  color: _textColor,
                  fontSize: 14.0,
                  fontWeight: FontWeight.w600,
                  letterSpacing: 5
                ),
              ),
            ],
          ),   
        onPressed: () async{
          if(_callback != null)
          {
            _callback.call();
          }
        },
      ),
    );

  }

}