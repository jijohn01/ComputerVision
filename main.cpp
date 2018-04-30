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
/*
vector< float > get_svm_detector( const Ptr< SVM >& svm )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction( 0, alpha, svidx );
   // CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
   // CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
   //            (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    vector< float > hog_detector( sv.cols + 1 );
    memcpy( &hog_detector[0], sv.ptr(), sv.cols*sizeof( hog_detector[0] ) );
    hog_detector[sv.cols] = (float)-rho;
    return hog_detector;
}*/
void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector)
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);
    cout << "alpha = " << alpha << endl;
    //CV_Assert(alpha.total() == 1 && svidx.total() == 1 && sv_total == 1);
    //CV_Assert((alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
        //(alpha.type() == CV_32F && alpha.at<float>(0) == 1.f));
    //CV_Assert(sv.type() == CV_32F);
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols * sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}

void getSVMParams(SVM *svm)
{
    cout << "Kernel type     : " << svm->getKernelType() << endl;
    cout << "Type            : " << svm->getType() << endl;
    cout << "C               : " << svm->getC() << endl;
    cout << "Degree          : " << svm->getDegree() << endl;
    cout << "Nu              : " << svm->getNu() << endl;
    cout << "Gamma           : " << svm->getGamma() << endl;
}

void SVMtrain(Mat &trainMat, vector<int> &trainLabels, Mat &testResponse, Mat &testMat,HOGDescriptor hog) {
    Ptr<SVM> svm = SVM::create();
    svm->setGamma(0.4);
    svm->setC(6);
//    svm->setP(0.01);
    svm->setKernel(SVM::LINEAR);
    svm->setType(SVM::C_SVC);
    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    cout<<"SVM Training..."<<endl;
    svm->train(td);
    //svm->trainAuto(td);
    cout<<"train complete!!"<<endl;
    svm->save("firetrain.yml");

//    svm->setCoef0( 0.0 );
//    svm->setDegree( 3 );
//    svm->setTermCriteria( TermCriteria( CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 1e-3 ) );
//    svm->setGamma( 0 );
//    svm->setKernel( SVM::LINEAR );
//    svm->setNu( 0.5 );
//    svm->setP( 0.1 ); // for EPSILON_SVR, epsilon in loss function?
//    svm->setC( 0.01 ); // From paper, soft classifier
//    svm->setType( SVM::C_SVC); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
//    svm->train( trainMat, ROW_SAMPLE, trainLabels );
    cout<<"save the yml file!!"<<endl;
    svm->predict(testMat, testResponse);
    //getSVMParams(svm);
    //hog.setSVMDetector(get_svm_detector(svm));
    //hog.save("hogfile.yml");

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

void printHOGParams(HOGDescriptor &hog)
{
    cout << "HOG descriptor size is " << hog.getDescriptorSize() << endl;
    cout << "hog.windowSize: " << hog.winSize << endl;
    cout << " cellsize " << hog.cellSize << endl;
    cout << " hog.nbins " << hog.nbins << endl;
    cout << " blockSize " << hog.blockSize << endl;
    cout << " blockStride " << hog.blockStride << endl;
    cout << " hog.nlevels " << hog.nlevels << endl;
    cout << " hog.winSigma " << hog.winSigma << endl;
    cout << " hog.free_coef  " << hog.free_coef << endl;
    cout << " hog.DEFAULT_NLEVELS " << hog.DEFAULT_NLEVELS << endl;

}


int main(int argc, char *argv[])
{
    //get Train data
    util ut;

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

    Mat inputImg;
    bool success = true;

    string filenumber;
    bool trainorvalid = true;


    /*********manipulation train data*********/
    vector<vector<float>> trainHog;
    vector<vector<float>> validHog;
    vector<int> trainLabel;//car=1,doncare = -1,people =0
    vector<int> validLabel;

    for(int j=0;j<1;j++){
    for(int i=0;i<7000;i++){
        filenumber = i2sFillZero(i,6);

        tFileName fname = ut.setFileName(filenumber,trainorvalid);

        vector<tGroundtruth> gt = ut.loadGroundtruth(fname.filenamelabel,success);
        inputImg = imread(fname.filenameimage);
        ut.stackHog(inputImg,hog,gt,trainHog,trainLabel);

        cout<<"train file number "<<i<<"complete"<<endl;

    }
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
    SVMtrain(trainMat, trainLabel, testResponse, validMat,hog);

    float count = 0;
    float accuracy = 0;
    SVMevaluate(testResponse, count, accuracy, validLabel);

    cout << "the accuracy is :" << accuracy << endl;
/************************test code******************/
    //HOGDescriptor hogi;
    Ptr<SVM> svm;
    svm = StatModel::load<SVM>("firetrain.yml");
    vector< float > hog_detector;
    get_svm_detector(svm, hog_detector);
    hog.setSVMDetector(hog_detector);
   // printHOGParams(hogi);
   // hogi.load("hogfile.yml");

    Mat testimg;
    tFileName testfile = ut.setFileName("000003",trainorvalid);
    testimg = imread(testfile.filenameimage);

    vector< Rect > detections;
    vector<double> foundWeights;
    hog.detectMultiScale(testimg, detections, foundWeights );
    //hogi.detectMultiScale(testimg, detections,foundWeights, Size(8, 8), Size(0, 0), 1.05, 2);
    for ( size_t j = 0; j < detections.size(); j++ )
    {
        Scalar color = Scalar( 0, foundWeights[j] * foundWeights[j] * 200, 0 );
        rectangle( testimg, detections[j], color,testimg.cols / 400 + 1  );
    }

    imshow( "test", testimg );

//rectangle(inputImg,r,Scalar(0,255,0),2,1,0);
//imshow("bound",gray);

    waitKey(9000);
    return 0;
}
