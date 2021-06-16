import 'package:flutter/material.dart';
import 'package:contact_tracing_app/constants.dart';
import 'package:contact_tracing_app/event_manager.dart';
import 'package:contact_tracing_app/screens/home_page.dart';
import 'package:contact_tracing_app/screens/covid_test_page.dart';
import 'package:contact_tracing_app/screens/covid_test_add_page.dart';
import 'package:contact_tracing_app/screens/link_device_page.dart';
import 'package:contact_tracing_app/screens/settings_page.dart';


class PageNav {
  static const Home = 0; 
  static const CovidTest = 1;
  static const LinkDevice = 2;
  static const Settings = 3;
  static const CovidAddTest = 4;
  static const LinkDeviceSearch = 5;

  static const BottomNav = [Home, CovidTest, LinkDevice, Settings];
  static const PageNames = {
    Home: appName,
    CovidTest: 'Covid-19 Test Result',
    CovidAddTest: 'Covid-19 Test Result',
    LinkDevice: 'Link Device',
    LinkDeviceSearch: 'Link Device',
    Settings: 'Settings',
  };


  int _currentIndex;
  int get currentIndex => _currentIndex;
  Map<int, int> _pageTracker = Map<int, int>();
  PageNav({int startIndex=0}){
    _currentIndex = startIndex;
  }

  int onUpdatePage(int newPage){
    if(BottomNav.contains(newPage)){
      // check if any associativty in tracker
      if(_pageTracker.containsKey(newPage)){
        _currentIndex = _pageTracker[newPage];
      }
      else{
        _currentIndex = newPage;
      }
    }
    else{
      _pageTracker[_currentIndex] = newPage;
      _currentIndex = newPage;
    }
    return _currentIndex;
  }

  int onBackButtonPressed(){
    // remove current page from history
    var keys = _pageTracker.keys.toList();
    for (var key in keys) {
      if(_pageTracker[key] == _currentIndex){
        _pageTracker.remove(key);
        _currentIndex = key;
      }
    }
    return _currentIndex;
  }
  
}

class TabTemplate extends StatefulWidget {
  @override
  _TabTemplateState createState() => _TabTemplateState();
}

class _TabTemplateState extends State<TabTemplate>{

  // track the current tab index
  int _currentIndex = 0;
  int _bottomNavIndex = 0;
  List<int> pageHistory = List<int>();
  PageNav nav = PageNav();

  @override
  void initState() {
    EventManager.instance.subscribe("onPageChanged", onPressedNavButton);
    super.initState();
  }

  @override
  void dispose() {
    EventManager.instance.unsubscribe("onPageChanged", onPressedNavButton);
    super.dispose();
  }
  /// call this method to change the current page selected.
  void onPressedNavButton(int index){
    setState(() {
      _currentIndex = nav.onUpdatePage(index);
      if(PageNav.BottomNav.contains(index)){
        _bottomNavIndex = index;
      }
    });
  }

  void onBackButton(){
    setState(() {
      _currentIndex = nav.onBackButtonPressed();
      if(PageNav.BottomNav.contains(_currentIndex)){
        _bottomNavIndex = _currentIndex;
      }
    });
    
  }


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: backgroundColour,
      bottomNavigationBar: BottomNavigationBar(
        selectedItemColor: primaryColour,
        type: BottomNavigationBarType.fixed,
        currentIndex: _bottomNavIndex,
        onTap: (index){
          onPressedNavButton(index);
        },
        items: [
          BottomNavigationBarItem(
            icon: Icon(Icons.home),
            title: Text('Home'),
            backgroundColor: Colors.white,
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.local_hospital),
            title: Text('Covid Test'),
            backgroundColor: Colors.white,
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.link),
            title: Text('Link Device'),
            backgroundColor: Colors.white,
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.settings),
            title: Text('Settings'),
            backgroundColor: Colors.white,
          ),
        ],
      ),
      body: SafeArea(
        child: Column(
          children: <Widget>[
            Container(
              width: MediaQuery.of(context).size.width,
              height: 70,
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
              child: Row(
                children: <Widget>[
                  Visibility(
                    maintainSize: true,
                    maintainAnimation: true,
                    maintainState: true,
                    visible: !PageNav.BottomNav.contains(_currentIndex),
                    child: IconButton(
                      icon: Icon(Icons.arrow_back),
                      onPressed: (){
                        onBackButton();
                      },
                      color: Color(0xFFC4C4C4),
                      iconSize: 35.0, 
                    ),
                  ),
                  //SizedBox(width:30.0),
                  Text(
                    PageNav.PageNames[_currentIndex],
                    style: TextStyle(
                      color: Colors.black,
                      fontWeight: FontWeight.bold,
                      fontSize: 25.0,
                    ),
                  ),
                ],
              ),
            ),
            SizedBox(height: 5.0),
            Expanded(
              child: Container(
                width: MediaQuery.of(context).size.width,
                child: IndexedStack(
                  children: <Widget>[
                    HomePage(),
                    CovidTestPage(),
                    LinkPage(),
                    SettingsPage(),
                    CovidTestPageAdd(),
                  ],
                  index: _currentIndex,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}