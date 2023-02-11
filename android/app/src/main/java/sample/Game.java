package sample;

public class Game extends android.app.NativeActivity {
    static {

        System.loadLibrary("c++_shared");
        System.loadLibrary("oboe");
        System.loadLibrary("game");

    }
}
