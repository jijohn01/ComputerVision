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
tFileName util::setFileName(string filenum){
    tFileName file;
    file.filenameimage = filenum+".png";
    file.filenamelabel = filenum+".txt";
    return file;
}

vector<Rect> util::setROI(vector<tGroundtruth> &gt){
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
        if(h>w)
            r= Rect(center.x-(w/2),center.y-(w/2),h,h);
        else
            r = Rect(center.x-(w/2),center.y-(w/2),w,w);

        roi.push_back(r);

    }

    return roi;
}
