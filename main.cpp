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
    string filenumber;

    HOGDescriptor hog(
                Size(64,64),//winSize
                Size(16,16),//blocksize
                Size(8,8),//blockStride
                Size(8,8),//cellSize
                9,//nbins
                1,//derivAper
                -1,//winSigma
                0,//histogramNormType
                0.2,//L2HysThresh
                0,//gammal correction
                64,//nlevels
                false);//signed or unsigned gradients
    //hog.winSize = Size(64,64);
    vector<float> descriptors;

    bool success = true;
    filenumber = "000002";
    tFileName fname = ut.setFileName(filenumber);
    vector<tGroundtruth> gt = ut.loadGroundtruth(fname.filenamelabel,success);
    vector<Rect> roi;
    Point2i p1,p2;
    Mat inputImg;
    Mat gray;

    inputImg = imread(fname.filenameimage);
    roi = ut.setROI(gt);


    cvtColor(inputImg(roi[0]),gray,COLOR_BGR2GRAY);
        //resizing
    resize(gray,gray,Size(64,64));
    hog.compute(gray,descriptors);
    cout<<descriptors.size()<<"rect center:"<<roi[0].width<<endl;

//        //rectangle(inputImg,r,Scalar(0,255,0),2,1,0);

//    }


    imshow("bound",gray);

    waitKey(5000);
    return 0;
}
