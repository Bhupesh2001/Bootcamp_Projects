// Mat src = imread("ss.png", IMREAD_UNCHANGED);

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;
int main()
{

    int theta = 0,angle;
    Mat src, frame, frameRotated;
    src = imread("image.jpg", IMREAD_UNCHANGED);
    cout << "Enter the angle to be rotated : ";
    cin >> angle;
    theta = angle;

    int diagonal = (int)sqrt(src.cols * src.cols + src.rows * src.rows);
    int newWidth = diagonal;
    int newHeight = diagonal;

    int offsetX = (newWidth - src.cols) / 2;
    int offsetY = (newHeight - src.rows) / 2;
    Mat targetMat(newWidth, newHeight, src.type());
    Point2f src_center(targetMat.cols / 2.0F, targetMat.rows / 2.0F);



        src.copyTo(frame);
        double radians = theta * 3.14 / 180.0;
        double sin = abs(std::sin(radians));
        double cos = abs(std::cos(radians));

        frame.copyTo(targetMat.rowRange(offsetY, offsetY + frame.rows).colRange(offsetX, offsetX + frame.cols));
        Mat rot_mat = getRotationMatrix2D(src_center, theta, 1.0);
        warpAffine(targetMat, frameRotated, rot_mat, targetMat.size());
        Rect bound_Rect(frame.cols, frame.rows, 0, 0);

        int x1 = offsetX;
        int x2 = offsetX + frame.cols;
        int x3 = offsetX;
        int x4 = offsetX + frame.cols;

        int y1 = offsetY;
        int y2 = offsetY;
        int y3 = offsetY + frame.rows;
        int y4 = offsetY + frame.rows;

        Mat co_Ordinate = (Mat_<double>(3, 4) << x1, x2, x3, x4,
            y1, y2, y3, y4,
            1, 1, 1, 1);
        Mat RotCo_Ordinate = rot_mat * co_Ordinate;

        for (int i = 0; i < 4; i++) {
            if (RotCo_Ordinate.at<double>(0, i) < bound_Rect.x)
                bound_Rect.x = (int)RotCo_Ordinate.at<double>(0, i);
            if (RotCo_Ordinate.at<double>(1, i) < bound_Rect.y)
                bound_Rect.y = RotCo_Ordinate.at<double>(1, i);
        }

        for (int i = 0; i < 4; i++) {
            if (RotCo_Ordinate.at<double>(0, i) > bound_Rect.width)
                bound_Rect.width = (int)RotCo_Ordinate.at<double>(0, i);
            if (RotCo_Ordinate.at<double>(1, i) > bound_Rect.height)
                bound_Rect.height = RotCo_Ordinate.at<double>(1, i);
        }

        bound_Rect.width = bound_Rect.width - bound_Rect.x;
        bound_Rect.height = bound_Rect.height - bound_Rect.y;

        Mat cropedResult;
        Mat ROI = frameRotated(bound_Rect);
        ROI.copyTo(cropedResult);

        char k;
        namedWindow("Result", WINDOW_NORMAL);
        imshow("Result", cropedResult);
        cout << "Press 'S' to save else any key to EXIT : ";
        k = waitKey();
        if (k == 'S' || k=='s')
            imwrite("rotated.jpg", cropedResult);
        
    return 0;
}