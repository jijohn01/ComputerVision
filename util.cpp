#include "util.h"

util::util()
{

}
//setFunctions
vector<tGroundtruth> util::loadGroundtruth(string file_name,bool &success) {

  // holds all ground truth (ignored ground truth is indicated by an index vector
  vector<tGroundtruth> groundtruth;
  FILE *fp = fopen(file_name.c_str(),"r");
  if (!fp) {
    success = false;
    return groundtruth;
  }
  while (!feof(fp)) {
    tGroundtruth g;
    char str[255];
    if (fscanf(fp, "%s %lf %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                   str, &g.truncation, &g.occlusion, &g.box.alpha,
                   &g.box.x1,   &g.box.y1,     &g.box.x2,    &g.box.y2,
                   &g.h,      &g.w,        &g.l,       &g.t1,
                   &g.t2,      &g.t3,        &g.ry )==15) {
      g.box.type = str;
      groundtruth.push_back(g);
    }
  }
  fclose(fp);
  success = true;
  return groundtruth;
}
tFileName util::setFileName(string filenum,bool trainorvalid){
    tFileName file;
    if(trainorvalid){
        file.filenameimage = "./training/image_2/"+filenum+".png";
        file.filenamelabel = "./training/label_2/"+filenum+".txt";
        return file;
    }
    else{
        file.filenameimage = "./testing/image_2/"+filenum+".png";
        file.filenamelabel = "./testing/label_2/"+filenum+".txt";
        return file;
    }
}

vector<Rect> util::setROI(vector<tGroundtruth> &gt,Mat &img){
    vector<Rect> roi;

//    Point2i p1,p2;
    for(unsigned int i=0;i < gt.size();i++){
        Point2i center;
        int h,w;
        Rect r;
        //roi center
        center.x = ((int)gt[i].box.x1 + (int)gt[i].box.x2)/2;
        center.y = ((int)gt[i].box.y1 + (int)gt[i].box.y2)/2;
        h = ((int)gt[i].box.y2 - (int)gt[i].box.y1);
        w = ((int)gt[i].box.x2 - (int)gt[i].box.x1);
        //square rect
        if(h>w){
            //예외처리
            if(center.x<(h/2)||(int)gt[i].box.x1+h>img.size().width){r = Rect((int)gt[i].box.x1,center.y-(h/2),w,w);}
            else {r = Rect(center.x-(w/2),center.y-(h/2),h,h);}
        }
        else{
            if(center.y<(w/2)||(int)gt[i].box.y1+w>img.size().height){r = Rect(center.x-(w/2),(int)gt[i].box.y1,h,h);}
            else {r = Rect(center.x-(w/2),center.y-(h/2),w,w);}
        }

        roi.push_back(r);

    }

    return roi;
}

void util::stackHog(Mat &image,HOGDescriptor &hog,vector<tGroundtruth> &gt,vector<vector<float>> &hogset,vector<int> &label)
{
    vector<Rect> roi = util::setROI(gt,image);
    vector<float> descriptors;

    for(unsigned int i = 0;i<roi.size();i++){
        Mat gray;
        cvtColor(image(roi[i]),gray,COLOR_BGR2GRAY);
    //resizing
        resize(gray,gray,Size(32,32));

        hog.compute(gray,descriptors);
//        cout<<"rect width:"<<roi[i].width<<endl;

        //stack training hog and label
        hogset.push_back(descriptors);
        if(gt[i].box.type=="DontCare"){
            label.push_back(-1);
        }
        else if(gt[i].box.type=="Person_sitting"||gt[i].box.type=="Pedestrian"){
            label.push_back(-1);
        }
        else{
            label.push_back(1);
        }
    }
}

void util::ConvertVectortoMatrix(vector<vector<float> > &ipHOG, Mat & opMat)
{

    int descriptor_size = ipHOG[0].size();
    for (int i = 0; i<ipHOG.size(); i++) {
        for (int j = 0; j<descriptor_size; j++) {
            opMat.at<float>(i, j) = ipHOG[i][j];
        }
    }
}
