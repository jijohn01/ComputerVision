//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/xfeatures2d.hpp>
//#include <opencv2/features2d.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
//#include <iostream>
//#include <vector>

//#include <algorithm>
//#include <stdio.h>
//#include <math.h>
//#include <numeric>
//#include <strings.h>
//#include <assert.h>

//#include "datamanager.h"
//#include "mail.h"

//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>

//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point_xy.hpp>
//#include <boost/geometry/geometries/polygon.hpp>
//#include <boost/geometry/geometries/adapted/c_array.hpp>

//using namespace std;
//using namespace cv;
//constructor
//DataManager::DataManager() :success(true){}
//setter and getter
//void DataManager::setLabelName(string file_num){labelName=file_num+".txt";}
//void DataManager::setImageName(string file_num){imageName=file_num+".png";}


//vector<tGroundtruth> DataManager::loadGroundtruth(string file_name,bool &success) {

//  // holds all ground truth (ignored ground truth is indicated by an index vector
//  vector<tGroundtruth> groundtruth;
//  FILE *fp = fopen(file_name.c_str(),"r");
//  if (!fp) {
//    success = false;
//    return groundtruth;
//  }
//  while (!feof(fp)) {
//    tGroundtruth g;
//    char str[255];
//    if (fscanf(fp, "%s %lf %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
//                   str, &g.truncation, &g.occlusion, &g.box.alpha,
//                   &g.box.x1,   &g.box.y1,     &g.box.x2,    &g.box.y2,
//                   &g.h,      &g.w,        &g.l,       &g.t1,
//                   &g.t2,      &g.t3,        &g.ry )==15) {
//      g.box.type = str;
//      groundtruth.push_back(g);
//    }
//  }
//  fclose(fp);
//  success = true;
//  return groundtruth;
//}

//void DataManager::imageRead(string file_name){
//    string filename = file_name;
//    filename += ".png";
//      image = imread(file_name);

//}

//void DataManager::setData(){
//    bool success = true;
//    gt = loadGroundtruth("000000.png",success);
//    image = imread("000000.png");
//}
