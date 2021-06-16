import 'dart:async';
import 'package:contact_tracing_app/ble_connection/ble_manager.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/firebase_backend/database_services.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:intl/intl.dart';

// TODO:
// Create a stream to access the battery level
// Create function to read data from device
// Create function to initialise connection with device (and register)


/// A Static instance of this class is reponsible for managing connections with the device.
/// All interactions with the physical device are done through this class. This includes BLE
/// connectivity, data transfer, connecting, and disconnecting
class DeviceManager {

  /// Get instance if it exists, else create a new one.
  // Create static instance
  static DeviceManager _instance = new DeviceManager();
  static DeviceManager get instance => _instance;

  // Define constant parameters
  final int bleScanDuration = 3;
  final Guid batteryServiceID = Guid('0000180f-0000-1000-8000-00805f9b34fb');
  final Guid batteryCharID = Guid('00002a19-0000-1000-8000-00805f9b34fb');

  final Guid pullDataServiceID = Guid('12345678-1234-5678-1234-56789abcdef0');
  final Guid pullDataCharID = Guid('12345678-1234-5678-1234-56789abcdef3');

  final Guid hasDataServiceID = Guid('12345678-1234-5678-1234-56789abcdef0');
  final Guid hasDataCharID = Guid('12345678-1234-5678-1234-56789abcdef4');

  // define timers
  Timer batteryTimer;

  // define public accessors
  String _deviceName;
  String get deviceName => _deviceName;
  
  // Default constructor
  DeviceManager()
  {

  }

  Future searchForDevices() async
  {
    bool hasBLE = await BleManager.instance.flutterBlue.isAvailable;
    if(!hasBLE)
    {
      print("ERROR: NO BLE DETECTED ON DEVICE");
      EventManager.instance.triggerEvent('failedToConnect', null);
      return;
    }
    int err = await BleManager.instance.scanAndPairWithDevice(scanDuration: bleScanDuration);
    if(err == 1)
    {
      print("ERROR: CONNECTING TO DEVICE, OR COULD NOT FIND ONE");
      EventManager.instance.triggerEvent('failedToConnect', null);
      return;
    }

    //update public accessor variables
    _deviceName = BleManager.instance.deviceName;
    // begin listening to data from device
    const oneSec = const Duration(seconds:10);
    batteryTimer = new Timer.periodic(oneSec, (Timer t) async {
      var batteryLevelData = await BleManager.instance.readData(batteryServiceID, batteryCharID);
      print('Batery Data is:');
      print(batteryLevelData);
      EventManager.instance.triggerEvent('onBatteryLevelUpdate', batteryLevelData[0]);
      //update the battery stream

    });
    EventManager.instance.triggerEvent('connectedToDevice', null);
  }

  Future disconnectDevice() async
  {
    batteryTimer.cancel();
    await BleManager.instance.disconnectDevice();
    EventManager.instance.triggerEvent('onDisconnected', null);
  }

  Future<bool> hasData() async
  {
    var hasDataVal = await BleManager.instance.readData(hasDataServiceID, hasDataCharID);
    return hasDataVal[0] == 1 ? true : false;
  }

  Future pullData() async
  {
    var contactTracingData = await BleManager.instance.readData(pullDataServiceID, pullDataCharID);
  }

}