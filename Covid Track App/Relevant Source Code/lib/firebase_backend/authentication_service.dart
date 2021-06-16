import 'package:firebase_auth/firebase_auth.dart';


/// Contains instance of `FirebaseAuth`.
/// 
/// Subscribe the `user` stream to get updates on authentication state.
/// Also contains all Sign In/Out/Up.
class AuthenticateService{

  final FirebaseAuth _auth = FirebaseAuth.instance;

  // Create static instance
  static AuthenticateService _instance = new AuthenticateService._();
  static AuthenticateService get instance => _instance;

  AuthenticateService._();

  /// Auth change from backend.
  /// Called when authentication state changes,
  /// i.e. when the user logs in/out.
  Stream<FirebaseUser> get user {
    return _auth.onAuthStateChanged;
  }

  /// Depreciated - use currentUser;
  Future<FirebaseUser> userData() async => await _auth.currentUser();

  // sign in anon
  Future signInAnon() async {
    try {
      AuthResult result = await _auth.signInAnonymously();
      FirebaseUser user = result.user;
      return user;
    } catch(error) {
      print(error.toString());
      return null;
    }
  }
  
  // sign in with email and password
  Future signInWithEmailAndPassword(String email, String password) async {
    try {
      AuthResult result = await _auth.signInWithEmailAndPassword(email: email, password: password );
      FirebaseUser user = result.user;
      return user;
    } catch(error) {
      print(error.toString());
      return null;
    }
  }


  // register with email and password
  Future registerWithEmailAndPassword(String email, String password) async {
    try {
      AuthResult result = await _auth.createUserWithEmailAndPassword(email: email, password: password);
      FirebaseUser user = result.user;
      return user;
    } catch(error) {
      print(error.toString());
      return null;
    }
  }

  // sign out
  Future signOut() async{
    try{
      return await _auth.signOut();
    } catch(error) {
      print(error.toString());
      return null;
    }
  }
}