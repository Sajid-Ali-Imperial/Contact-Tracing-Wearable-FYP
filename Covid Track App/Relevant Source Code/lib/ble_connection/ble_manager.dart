import 'package:contact_tracing_app/event_manager.dart';
import 'package:flutter_blue/flutter_blue.dart';


class BleManager {

  BleManager._privateConstructor();
  static final BleManager _instance = BleManager._privateConstructor();
  static BleManager get instance => _instance;
  
  final FlutterBlue flutterBlue = FlutterBlue.instance;
  static List<BluetoothDevice> _deviceList = new List<BluetoothDevice>();
  int get discoveredDeviceNum {return _deviceList.length;}

  BluetoothDevice _connectedDevice;
  BluetoothDevice get connectedDevice => _connectedDevice;
  List<BluetoothService> _connectedServices;

  //final String _deviceName = "DWM1001_ex_12a";
  final String _deviceName = "Custom BLE Example";
  String get deviceName => _deviceName;

  /// Scan for nearby BLE devices.
  /// Call this function to initiate the scanning procedure.
  /// No device can be connected to without first scanning
  /// for it. Each discovered device is added to list which can
  /// later be used for establishing a connection.
  Future scanForDevices({int scanDuration=3}) async
  {
    print("CALLED START SCAN");
    _deviceList = new List<BluetoothDevice>();  // empty scanned list
    await disconnectDevice();                   // disconnect from any devices
    flutterBlue.connectedDevices.asStream().listen((List<BluetoothDevice> devices) {  // add connected device to list
      for (BluetoothDevice device in devices) {
        if (device.name == _deviceName && !_deviceList.contains(device)) {
        _deviceList.add(device);
        _connectedDevice = device;
    }
    }});
    flutterBlue.scanResults.listen((List<ScanResult> results) {   // add un-connected devices to list
      for (ScanResult result in results) {
        if (result.device.name == _deviceName && !_deviceList.contains(result.device)) {
            print('FOUND DEVICE!');
            print("Result RSSI is: " + result.rssi.toString());
            _deviceList.add(result.device);
        }
    }});

    await flutterBlue.startScan(timeout: Duration(seconds: scanDuration)); // begin scan
    await flutterBlue.stopScan();

  }

  /// Scan for nearby BLE devices and pair witht the closest one.
  /// Call this function to initiate the scanning procedure.
  /// No device can be connected to without first scanning
  /// for it. Each discovered device is added to list which can
  /// later be used for establishing a connection.
  Future<int> scanAndPairWithDevice({int scanDuration=3}) async
  {
    await scanForDevices(scanDuration: scanDuration);
    if(_deviceList.length > 0){
      return await connectToDevice(_deviceList[0]);
    }
    return 1;
  }

  /// Scan for nearby BLE devices.
  /// Call this function to initiate the scanning procedure.
  /// No device can be connected to without first scanning
  /// for it. Each discovered device is added to list which can
  /// later be used for establishing a connection.
  Future<int> connectToDevice(BluetoothDevice device) async
  {
    if(_connectedDevice != null)
    {
      await disconnectDevice(); 
    }
    return await _pairDevice(device);
    
  }

  /// Attempt to pair with any discovered device.
  /// Calling this function will also populate the services list
  /// enabling the application to access characteristics and
  /// services. If for what every reason a connection cannot be
  /// established, this function will return an int<1> indicating
  /// that something went wrong. 
  Future<int> _pairDevice(BluetoothDevice device) async
  {
    try
    {
      print('Connecting to: ' + device.name);
      await device.connect(timeout: Duration(seconds: 3), autoConnect: false).then((value) => print('Successfully Connected'));

    } catch(e)
    {
      return 1;
    }

    _connectedDevice = device;
    _connectedServices = await _connectedDevice.discoverServices();
    print('Number of services discovered: ' + _connectedServices.length.toString());
    for(BluetoothService service in _connectedServices)
    {
      print("Service UUID: " + service.uuid.toString());
      for(BluetoothCharacteristic characteristic in service.characteristics)
      {
        print("Characteristic ID: " + characteristic.uuid.toString());
        // if(characteristic.properties.read)
        // {
        //   await characteristic.read().then((value) => print('Read value is: ' + String.fromCharCodes(value)));
        // }
      }
      print("\n");
    } 
    return 0;  
  }


  /// Terminate any active BLE connection.
  /// Call this before scanning for devices to ensure that existing 
  /// connections are terminated. Also, call this when the user is 
  /// done downloading data. This enables the device to re-enter 
  /// active mode since while thisconnection is active it cannot 
  /// scan for other nearby devices.
  Future disconnectDevice() async
  {
    if(_connectedDevice != null)
    {
      await _connectedDevice.disconnect();
      _connectedDevice = null;
    }
  }

  /// Read data from a BLE connection.
  /// Pass a valid serviceID and characteristicID to the function.
  /// If both are valid and a read operation is available on the
  /// characteristic, it will be asynchronously read and the result
  /// will be returned.
  Future<List<int>> readData(Guid serviceID, Guid characteristicID) async
  {
    if(_connectedDevice != null)
    {
      // get the service
      var service = _connectedServices.firstWhere((s) => s.uuid == serviceID);
      var characteristic = service.characteristics.firstWhere((c) => c.uuid == characteristicID);
      if(characteristic.properties.read)
      {
        return await characteristic.read();
      }
    }
    return null;
  }

  /// Write data to a BLE connection
  /// Pass a valid serviceID and characteristicID to the function.
  /// If both are valid and a write operation is available on the
  /// characteristic, it will be asynchonously written. Not sure if
  /// an ack is send to confirm reception of the data. CBT
  Future writeData(Guid serviceID, Guid characteristicID, List<int> value) async
  {
    if(_connectedDevice != null)
    {
      // get the service
      var service = _connectedServices.firstWhere((s) => s.uuid == serviceID);
      var characteristic = service.characteristics.firstWhere((c) => c.uuid == characteristicID);
      if(characteristic.properties.write)
      {
        await characteristic.write(value);
      }
    }
  }

}