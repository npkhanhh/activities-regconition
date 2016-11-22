#include "sift-feature.h"

cv::Rect rois[] = {Rect(Point(109, 35), Point(199, 240)),
                    Rect(Point(150, 27), Point(245, 240)),
                    Rect(Point(80, 35), Point(180, 235)),
                    Rect(Point(124, 44), Point(211, 229)),
                    Rect(Point(121, 26), Point(247, 240)),
                    Rect(Point(120, 35), Point(200, 240)),
                    Rect(Point(114, 32), Point(212, 240)),
                    Rect(Point(114, 32), Point(212, 240)),
                    Rect(Point(123, 81), Point(222, 240)),
                    Rect(Point(119, 97), Point(220, 240)),
                    Rect(Point(114, 55), Point(211, 230)),
                    Rect(Point(133, 40), Point(211, 228)),
                    Rect(Point(156, 31), Point(238, 234)),
                    Rect(Point(158, 32), Point(247, 236)),
                    Rect(Point(140, 38), Point(233, 232)),
                    Rect(Point(114, 100), Point(235, 240)),
                    Rect(Point(50, 22), Point(278, 240)),
                    };

  std::vector<cv::Point3f> convert3dtoPoint(Mat Depth)
  {
	  std::vector<cv::Point3f> Image3D;
	  cv::Point3f temp;
	  int rows = Depth.rows;
	  int cols = Depth.cols;
	  for (int i = 0; i < rows; i++)
	  {
		  short int* data = Depth.ptr<short int>(i);
		  for (int j = 0; j < cols; j++)
		  {
			  temp.x = j;
			  temp.y = i;
			  temp.z = data[j];
			  //cout<<temp<<endl;
			  Image3D.push_back(temp);
		  }
	  }
	  return Image3D;
  }

  Mat convertMat(std::vector<cv::Point2f> Object2D, std::vector<cv::Point3f> Object3D, Mat src)
  {
	  /*Mat dx(height, width, CV_32FC1);
	  Mat dy(height, width, CV_32FC1);
	  float* datax = dx.ptr<float>(0);
	  float* datay = dy.ptr<float>(0);
	  for (int i = 0; i < Object2D.size(); i++)
	  {
		  *(datax) = Object2D[i].x;

		  datax++;
		  *(datay) = Object2D[i].y;
		  datay++;
	  }
	  cv::normalize(dx, dx, 0, width-1,NORM_MINMAX,CV_32FC1);
	  cv::normalize(dy, dy, 0, height-1,NORM_MINMAX,CV_32FC1);
	  datax = dx.ptr<float>(0);
	  datay = dy.ptr<float>(0);
	  for (int i = 0; i < Object2D.size(); i++)
	  {
		  Object2D[i].x = *(datax++);
		  Object2D[i].y = *(datay++);
	  }*/
	  float max_x,min_x, max_y, min_y;
	  max_x = min_x = Object2D[0].x;
	  max_y = min_y = Object2D[0].y;
	  for (int i = 1; i < Object2D.size(); i++)
	  {
		 if (max_x < Object2D[i].x)
			 max_x = Object2D[i].x;
		 else
			 if(min_x > Object2D[i].x)
				 min_x = Object2D[i].x;
		 if (max_y < Object2D[i].y)
			 max_y = Object2D[i].y;
		 else
			 if(min_y > Object2D[i].y)
				 min_y = Object2D[i].y;
	  }
	  TWindow2D W;
	  TViewport2D V;
	  V.t = 0;
	  V.l = 0;
	  V.r = 320-1;
	  V.b = 240-1;
	  W.t = min_y;
	  W.l = min_x;
	  W.r = max_x;
	  W.b = max_y;
	  for (int i = 0; i < Object2D.size(); i++)
	  {
		  Object2D[i].x = (Object2D[i].x-(W.r+W.l)/2)*(V.r-V.l)/(W.r-W.l)+(V.r+V.l)/2;
		  Object2D[i].y = (Object2D[i].y-(W.t+W.b)/2)*(V.t-V.b)/(W.t-W.b)+(V.t+V.b)/2;
	  }
	  Mat image(height, width, CV_8UC1);
	  for (int i = 0; i < Object2D.size(); i++)
	  {
		  image.ptr<uchar>(Object2D[i].y)[(int)(Object2D[i].x)] = src.ptr<uchar>(Object3D[i].y)[(int)(Object3D[i].x)];
	  }
	  return image;
  }

  void writefeature(Mat sift,std::vector< float> descriptorsValues,std::vector< float> value3d, string filename)
  {
    FILE *fout;

    fout=fopen(filename.c_str(),"ab");


    fwrite(descriptorsValues.data(), descriptorsValues.size()*4,1,fout);
    //cout << "Descriptor value: " << descriptorsValues.data() << endl;
    //for(int i = 0;i<descriptorsValues.size();++i)
    //    out << descriptorsValues[i] << ",";
    fwrite(value3d.data(), value3d.size()*4,1,fout);

    //cout << "Value3d: " << value3d.data() << endl;
    //for(int i=0;i<value3d.size();++i)
    //    out << value3d[i] << ",";

    float temp[128];
	for (int i = 0; i < sift.rows; i++)
	{
		for (int j = 0; j < sift.cols; j++)
		{
			temp[j] = sift.at<float>(i, j)/512.f;
			//out << temp[j] << ",";
		}
		//cout << endl;
		//cout << "size: " << sizeof(temp) << endl;
		fwrite(temp, sizeof(temp), 1, fout);
	}

	fclose(fout);
  }

  void frame(string filein,string filedepth, string fileout, int i)
  {
      Mat depth = cv::imread(filedepth, CV_LOAD_IMAGE_ANYCOLOR);
      Mat croppedDepth = depth(rois[i]);
      //imshow("anh", img);
//
//    Mat dep(height, width, CV_16UC1);
//    //cv::normalize(img, dep, 0, 65535, NORM_MINMAX, CV_16UC1);
      Mat src = cv::imread(filein, CV_LOAD_IMAGE_ANYCOLOR);
      Mat croppedSrc = src(rois[i]);
      //cv::imshow("anh", src);
      //cv::waitKey();
//    Mat gray;
//    cv::cvtColor(src, gray, CV_RGB2GRAY);
//    std::vector<cv::Point3f> Object3D = convert3dtoPoint(img);
//    // configure
//    cv::Mat intrisicMat(3, 3, cv::DataType<double>::type); // Intrisic matrix
//    intrisicMat.at<double>(0, 0) = 1.6415318549788924e+003;
//    intrisicMat.at<double>(1, 0) = 0;
//    intrisicMat.at<double>(2, 0) = 0;
//
//    intrisicMat.at<double>(0, 1) = 0;
//    intrisicMat.at<double>(1, 1) = 1.7067753507885654e+003;
//    intrisicMat.at<double>(2, 1) = 0;
//
//    intrisicMat.at<double>(0, 2) = 5.3262822453148601e+002;
//    intrisicMat.at<double>(1, 2) = 3.8095355839052968e+002;
//    intrisicMat.at<double>(2, 2) = 1;
//
//    cv::Mat rVec(3, 1, cv::DataType<double>::type); // Rotation vector
//    rVec.at<double>(0) = -3.9277902400761393e-002;
//    rVec.at<double>(1) = 3.7803824407602084e-002;
//    rVec.at<double>(2) = 2.6445674487856268e-002;
//
//    cv::Mat tVec(3, 1, cv::DataType<double>::type); // Translation vector
//    tVec.at<double>(0) = 2.1158489381208221e+000;
//    tVec.at<double>(1) = -7.6847683212704716e+000;
//    tVec.at<double>(2) = 2.6169795190294256e+001;
//
//    cv::Mat distCoeffs(5, 1, cv::DataType<double>::type);   // Distortion vector
//    distCoeffs.at<double>(0) = -7.9134632415085826e-001;
//    distCoeffs.at<double>(1) = 1.5623584435644169e+000;
//    distCoeffs.at<double>(2) = -3.3916502741726508e-002;
//    distCoeffs.at<double>(3) = -1.3921577146136694e-002;
//    distCoeffs.at<double>(4) = 1.1430734623697941e+002;
//  //////////////////////////END//////////////////////////////////////////////
//
//  std::vector<cv::Point2f> Object2D;
//  cv::projectPoints(Object3D, rVec, tVec, intrisicMat, distCoeffs, Object2D);
//  Mat image2D = convertMat(Object2D, Object3D, gray);
    Mat out;
    cv::SiftFeatureDetector detector(50);
//    cv::Ptr<Feature2D> f2d = SIFT::create("");
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(croppedSrc, keypoints);

    while(keypoints.size()>50)
        keypoints.pop_back();
    Mat mask = mask.zeros(src.size(), CV_8U);
    //SIFT s;
    //s.operator()(src, cv::KERNEL_SMOOTH, keypoints, out, true);
    detector.operator()(croppedSrc, mask, keypoints, out, true);
    cv::Mat row = cv::Mat::zeros(1, 128, CV_32FC1);
    while(out.rows<50)
        out.push_back(row);

    Mat out_float;
    out_float.create(cvSize(out.cols, out.rows), CV_32FC1);
    cv::Mat output;

    //cv::drawKeypoints(croppedSrc, keypoints, output);
    //cv::imshow("anh", output);
    //cv::waitKey(0);

    Mat img_gray, depth_gray;
    img_gray.create(cvSize(src.cols, src.rows), CV_8UC1);
    //depth_gray.create(cvSize(src.cols, src.rows), CV_8UC1);
    cv::cvtColor(src, img_gray, CV_RGB2GRAY);
    //cv::imshow("anh", img_gray);
    //cv::waitKey();
    // Add results to image and save.
    //cv::resize(img_gray,img_gray,cvSize(320, 240));
    cv::HOGDescriptor d(cvSize(320, 240), cv::Size(32, 24), cv::Size(32, 24), cv::Size(16, 12), 8);
    //HOGDescriptor d(Size(32, 16), Size(8, 8), Size(4, 4), Size(4, 4), 9);
    std::vector< float> descriptorsValues, descriptorsValues_grey, value3d;
    std::vector< cv::Point> locations, local3d;

    d.compute(img_gray, descriptorsValues, cv::Size(0, 0), cv::Size(0, 0), locations);

    d.compute(depth, descriptorsValues_grey, cv::Size(0, 0), cv::Size(0, 0), locations);
    if(descriptorsValues.size() != 3200)
        cout << i << " " << "fucked2" << endl;

    if(descriptorsValues_grey.size() != 3200)
        cout << i << " " << "fucked3" << endl;
    //d.compute(image2D, value3d, cv::Size(0, 0), cv::Size(0, 0), local3d);
    writefeature(out, descriptorsValues, descriptorsValues_grey, fileout);
    //std::cout << descriptorsValues.size()<<" "<<value3d.size();
    //imshow("sau", image2D);
    //cv::waitKey(0);
  }

int sift_feature()
  {
	  string folderin = "data1";
	  string folderout = "data1";
	  string label = folderin +"/activityLabel.txt";
	  fstream f;
	  f.open(label,ios::in);
	  std::vector<std::string> folder;
	  string line;
	  getline(f,line);
	  ofstream fileout;
      fileout.open("vectorsize.txt", ofstream::out);
	  while (line != "END")
	  {
		  string element;
		  stringstream lineStream(line);
		  getline(lineStream, element,',');
		  while (element != "")
		  {
			  folder.push_back(element);
			  getline(lineStream, element,',');
			  getline(lineStream, element,',');
		  }
		  getline(f,line);
	  }
	  DIR*    dir;
      dirent* pdir;
	  string temp;
      std::vector<std::string> files;
      int count1 = 0;
	  for (int i = 0; i < folder.size(); i++)
	  {
		  temp = folderin+"/"+folder[i];
		  dir = opendir(temp.c_str());
		  files.clear();
		  while (pdir = readdir(dir)) {
			  string temp1 = temp + "/" + pdir->d_name;
			  //cout << pdir->d_name << endl;
			  files.push_back(temp1);
		  }
		  ostringstream number;
		  number << i;
		  string num = number.str();
          cout << "Writing to " << num+"_"+folder[i] << endl;
		  int numberOfFile = files.size()/2-1;
          cout << files.size() << endl;
		  for (int j = 1; j <= numberOfFile; j++)
		  {
			  ostringstream number2;  //temp as in temporary
			  number2<<j;
			  string num2 = number2.str();
			  //cout << "Frame: " << num2 << endl;
			  //cout << temp + "/RGB_" + num2 + ".png"  << endl;
			  frame(temp + "/RGB_" + num2 + ".png", temp + "/Depth_" + num2 + ".png", num+"_"+folder[i]+".bin", i);
		  }
		  //fstream filenameout;

		  count1+= numberOfFile/5;

		  fileout << "current total: " << count1 << endl;
		  fileout << "Number of frame: " << numberOfFile << endl;
		  fileout << "vector Size: " << numberOfFile * 12800 << " ";
		  FILE *fout;
          string filename = num+"_"+folder[i]+".bin";
          //fileout << filename << endl;
          fout=fopen(filename.c_str(),"rb");
		  fseek(fout, 0L, SEEK_END);
		  fileout << ftell(fout) << endl;
		  fclose(fout);

	  }
	  fileout.close();
	  return 0;
  }
