package golemdigital.tinytest;

public class Tiny implements Runnable {

    private String net_path_;
    private String img_path_;

    private int classification = -10;

    Tiny(String net, String img)
    {
        net_path_ = net;
        img_path_ = img;
    }

    public int getClassification()
    {
        return classification;
    }

    @Override
    public void run() {
        classification = TestTinyNet(net_path_, img_path_);
    }

    public static native int TestTinyNet(String net_path, String image_path);
}
