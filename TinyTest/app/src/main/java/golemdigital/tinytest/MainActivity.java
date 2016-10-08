package golemdigital.tinytest;

import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

public class MainActivity extends AppCompatActivity {

    static private String TAG = "TinyDnn";

    private String document_path_;
    private String image_file_;
    private String net_file_;

    //----------------------------------------------------------------------------------------------
    //
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS: {

                    // Load native library after(!) OpenCV initialization
                    Log.i(TAG, "OpenCV loaded successfully");
                    System.loadLibrary("tiny_ndk");
                }
                break;
                default: {
                    super.onManagerConnected(status);
                }
                break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        document_path_ = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();

        image_file_ = document_path_ + "/TinyDnn/eight.jpg";
        net_file_ = document_path_ + "/TinyDnn/Lenet_model";
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }


    public void TestTinyDnn(View v)
    {
        Log.i(TAG, "Test Button pressed");
        //int classification = TestTinyNet(net_file_, image_file_);

        Tiny tiny = new Tiny(net_file_, image_file_);
        tiny.run();
        TextView text = (TextView)findViewById(R.id.textView);

        //String classification_str = "Classification is : " + Integer.toString(classification);

        //text.setText(classification_str);


        Log.i(TAG, "Classification done");
    }

    public static native int TestTinyNet(String net_path, String image_path);
}
