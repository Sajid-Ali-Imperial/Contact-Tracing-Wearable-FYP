
/// A global instance of this class exists to allow subscribers to listent to events.
/// Events can also be triggered from anywhere.
class EventManager {
  // Create static instance
  static EventManager _instance = new EventManager._();
  static EventManager get instance => _instance;

  // Internal constructor
  EventManager._();

  Map<String, List<Function>> _events = Map<String, List<Function>>();


  /// Pass the event name and the callback to be execute when this event occures.
  /// 
  /// If an event with this name does not exists, one will be created.
  void subscribe(String eventName, Function callback){
    if(_events.containsKey(eventName)){
      // prevent double subsription
      if(!_events[eventName].contains(callback)){
        _events[eventName].add(callback);
      }
    }
    else {
      _events[eventName] = List<Function>();
      _events[eventName].add(callback);
    }
  }

  /// Pass the event name and the callback to be removed from the list of triggered functions.
  void unsubscribe(String eventName, Function callback){
    if(_events.containsKey(eventName)){
      _events[eventName].remove(callback);
    }
  }

  /// Trigger the event with the name `eventName`.
  void triggerEvent(String eventName, dynamic passData){
    if(_events.containsKey(eventName)){
      for (var func in _events[eventName]) {
        if(passData == null){
          func.call();
        }
        else{
          func.call(passData);
        }
      }
    }
  }
}