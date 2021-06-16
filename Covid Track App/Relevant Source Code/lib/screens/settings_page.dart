import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/widgets/device_widgets.dart';
import 'package:contact_tracing_app/firebase_backend/authentication_service.dart';


class SettingsPage extends StatefulWidget {
  @override
  _SettingsPageState createState() => _SettingsPageState();
}

class _SettingsPageState extends State<SettingsPage> {


  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        SizedBox(height: 20,),
        Center(
          child: GenericButton(
              text: 'SIGN OUT',
              callback: (){AuthenticateService.instance.signOut();}
            ),
        ),
      ],
    );
  }
}