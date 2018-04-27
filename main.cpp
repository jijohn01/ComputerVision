#include "util.cpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>

#include <iostream>
#include <vector>

#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <numeric>
#include <strings.h>
#include <assert.h>
#include <dirent.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>

#include "mail.h"
#include "util.h"
using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    util ut;
    string filename;

    HOGDescriptor hog(Size(32,32),Size(8,8),Size(4,4),Size(4,4),9,1);
    //hog.winSize = Size(64,64);
    vector<float> descriptors;
    vector<Point> location;

    bool success = true;
    filename = "000002";
    vector<tGroundtruth> gt = ut.loadGroundtruth("000002.txt",success);
    //cout<<gt[0].box.x1<<gt[0].box.x2 <<endl;
    Point2i p1,p2;
    Mat inputImg;
    Mat gray;

    inputImg = imread("000002.png");
    for(unsigned int i=0;i < gt.size()-1;i++){

        p1.x = (int)gt[i].box.x1;
        p1.y = (int)gt[i].box.y1;
        p2.x = (int)gt[i].box.x2;
        p2.y = (int)gt[i].box.y2;

        Rect r = Rect(p1,p2);
        cvtColor(inputImg(r),gray,COLOR_BGR2GRAY);
        //resizing
        //resize(gray,gray,Size(64,64));
//        hog.compute(gray,descriptors,Size(0,0),Size(0,0),location);

        //rectangle(inputImg,r,Scalar(0,255,0),2,1,0);
        imshow("bound",gray);

    }


    //imshow("bound",gray);

    waitKey(5000);
    return 0;
}
