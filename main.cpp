#include "util.cpp"
#include "mail.h"
#include "util.h"

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


using namespace std;
using namespace cv;
using namespace cv::ml;

void getSVMParams(SVM *svm)
{
    cout << "Kernel type     : " << svm->getKernelType() << endl;
    cout << "Type            : " << svm->getType() << endl;
    cout << "C               : " << svm->getC() << endl;
    cout << "Degree          : " << svm->getDegree() << endl;
    cout << "Nu              : " << svm->getNu() << endl;
    cout << "Gamma           : " << svm->getGamma() << endl;
}

void SVMtrain(Mat &trainMat, vector<int> &trainLabels, Mat &testResponse, Mat &testMat) {
    Ptr<SVM> svm = SVM::create();
    svm->setGamma(0.70625);
    svm->setC(1);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);
    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    cout<<"SVM Training..."<<endl;
    svm->train(td);
    //svm->trainAuto(td);
    cout<<"train complete!!"<<endl;
    svm->save("firetrain.yml");
    cout<<"save the yml file!!"<<endl;
    svm->predict(testMat, testResponse);
    getSVMParams(svm);

    /*
    To acheive 100% rate.
    Descriptor Size : 576
Kernel type     : 2
Type            : 100
C               : 2.5
Degree          : 0
Nu              : 0
Gamma           : 0.03375
the accuracy is :100

    */

}

void SVMevaluate(Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels) {

    for (int i = 0; i<testResponse.rows; i++)
    {
        //cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if (testResponse.at<float>(i, 0) == testLabels[i]) {
            count = count + 1;
        }
    }
    accuracy = (count / testResponse.rows) * 100;
}


inline string i2sFillZero(int n, int len)
{
    string result(len--, '0');
    for (int val=(n<0)?-n:n; len>=0&&val!=0; --len,val/=10)
       result[len]='0'+val%10;
    if (len>=0&&n<0) result[0]='-';
    return result;
}

int main(int argc, char *argv[])
{
    //get Train data
    util ut;

    HOGDescriptor hog(
                Size(32,32),//winSize
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
    Mat inputImg;
    bool success = true;

    string filenumber;
    string vaildfileNum = "000000";
    bool trainorvalid = true;


    //manipulation train data
    vector<vector<float>> trainHog;
    vector<vector<float>> validHog;
    vector<int> trainLabel;//car=1,doncare = -1,people =0
    vector<int> validLabel;

    for(int i=0;i<7000;i++){
        filenumber = i2sFillZero(i,6);

        tFileName fname = ut.setFileName(filenumber,trainorvalid);

        vector<tGroundtruth> gt = ut.loadGroundtruth(fname.filenamelabel,success);
        inputImg = imread(fname.filenameimage);
        ut.stackHog(inputImg,hog,gt,trainHog,trainLabel);

        cout<<"train file number "<<i<<"complete"<<endl;

    }

    int descriptor_size = trainHog[0].size();
    cout<<"descriptor size : "<<descriptor_size<<endl;
    /*******validation ************/
    for(int i=7001;i<7200;i++){
        filenumber = i2sFillZero(i,6);

        tFileName fname = ut.setFileName(filenumber,trainorvalid);

        vector<tGroundtruth> gt = ut.loadGroundtruth(fname.filenamelabel,success);
        inputImg = imread(fname.filenameimage);
        ut.stackHog(inputImg,hog,gt,validHog,validLabel);

        cout<<"train file number "<<i<<"complete"<<endl;

    }

    cout<<"descriptor size(vaildHog) : "<<descriptor_size<<endl;

    /****************prepare SVM******************/

    Mat trainMat(trainHog.size(), descriptor_size, CV_32FC1);
    Mat validMat(validHog.size(),descriptor_size, CV_32FC1);
    cout<<"vector to Mat converting..."<<endl;
    ut.ConvertVectortoMatrix(trainHog,trainMat);
    ut.ConvertVectortoMatrix(validHog,validMat);
    cout<<"converted!!"<<endl;

    Mat testResponse;
    SVMtrain(trainMat, trainLabel, testResponse, validMat);

    float count = 0;
    float accuracy = 0;
    SVMevaluate(testResponse, count, accuracy, validLabel);

    cout << "the accuracy is :" << accuracy << endl;

//rectangle(inputImg,r,Scalar(0,255,0),2,1,0);
//imshow("bound",gray);

    waitKey(5000);
    return 0;
}
