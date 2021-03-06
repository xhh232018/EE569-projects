// SIFT.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<math.h>
#include<opencv2\features2d.hpp>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\xfeatures2d.hpp>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
using namespace xfeatures2d;
vector<int> histogram(Mat desc, Mat voca) {
	vector<double> Euc_dist;
	double tempD = 0.0; double temp = 0; int index = 0;
	Euc_dist.resize(voca.rows);
	vector<int> histCount;
	histCount.resize(voca.rows);
	for (int i = 0; i < voca.rows; i++) {
		histCount[i] = 0;
	}
	for (int i = 0; i < desc.rows; i++) {
		for (int k = 0; k < voca.rows; k++) {
			for (int j = 0; j < desc.cols; j++) {
				tempD += ((desc.at<float>(i, j) - voca.at<float>(k, j))*(desc.at<float>(i, j) - voca.at<float>(k, j)));
			}
			Euc_dist[k] = (sqrt(tempD));
			tempD = 0.0;
		}
		temp = Euc_dist[0]; index = 0;

		for (int k = 1; k<voca.rows; k++) {
			if (temp>Euc_dist[k]) {
				temp = Euc_dist[k];
				index = k;
				//	cout<<index<<endl;
			}
		}	//cout<<"temp:"<<temp<<endl;
		histCount[index] += 1;
		//	cout << "The Texture" << i << " :is mapped to following index:" << mappedIndex[i] << endl;

	}
	return histCount;
}
int main(int argc, char* argv[])
{
	String inputFile1 = argv[1];
	String inputFile2 = argv[2];
	String inputFile3 = argv[3];
	String inputFile4 = argv[4];
	Mat truck, car,f_1,f_2;
	truck = imread(inputFile1,IMREAD_GRAYSCALE);
	car = imread(inputFile2,IMREAD_GRAYSCALE);
	f_1 = imread(inputFile3, IMREAD_GRAYSCALE);
	f_2 = imread(inputFile4, IMREAD_GRAYSCALE);
	Ptr<SIFT> sift_detector = SIFT::create();
	Ptr<SURF> surf_detector = SURF::create();

	vector<KeyPoint>kp_1, kp_2,kp_3,kp_4,kp_5,kp_6,kp_7,kp_8;
	double t = getTickCount();
	sift_detector->detect(truck, kp_1);
	sift_detector->detect(car, kp_2);
	sift_detector->detect(f_1, kp_3);
	sift_detector->detect(f_2, kp_4);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "t is " << t << "s" << endl;

	double t1 = getTickCount();
	surf_detector->detect(truck, kp_5);
	surf_detector->detect(car, kp_6);
	surf_detector->detect(f_1, kp_7);
	surf_detector->detect(f_2, kp_8);
	t1 = ((double)getTickCount() - t1) / getTickFrequency();
	cout << "t1 is " << t1 << "s" << endl;
    

	Mat img_1, img_2,img_3,img_4, img_5, img_6, img_7, img_8;
	drawKeypoints(truck, kp_1, img_1, Scalar::all(-1), 0);
	drawKeypoints(car, kp_2, img_2, Scalar::all(-1), 0);
	drawKeypoints(f_1, kp_3, img_3, Scalar::all(-1), 0);
	drawKeypoints(f_2, kp_4, img_4, Scalar::all(-1), 0);
	drawKeypoints(truck, kp_5, img_5, Scalar::all(-1), 0);
	drawKeypoints(car, kp_6, img_6,Scalar::all(-1), 0);
	drawKeypoints(f_1, kp_7, img_7, Scalar::all(-1), 0);
	drawKeypoints(f_2, kp_8, img_8, Scalar::all(-1), 0);

	imshow("SIFT_KEYPOINTS1",img_1);
	imshow("SIFT_KEYPOINTS2", img_2);
	imshow("SIFT_KEYPOINTS3", img_3);
	imshow("SIFT_KEYPOINTS4", img_4);

	imshow("SURF_KEYPOINTS1", img_5);
	imshow("SURF_KEYPOINTS2", img_6);
	imshow("SURF_KEYPOINTS3", img_7);
	imshow("SURF_KEYPOINTS4", img_8);

	BFMatcher SURF_matcher, SIFT_matcher;
	Mat si_descriptor1, si_descriptor2, si_descriptor3, si_descriptor4, sf_descriptor1, sf_descriptor2, sf_descriptor3, sf_descriptor4, simatch1, sfmatch1, simatch2, sfmatch2, simatch3, sfmatch3;
	sift_detector->compute(img_3, kp_3, si_descriptor1); //F_1
	sift_detector->compute(img_4, kp_4, si_descriptor2); //F_2
	sift_detector->compute(img_1, kp_1, si_descriptor3); //TRUCK
	sift_detector->compute(img_2, kp_2, si_descriptor4); //CAR

	surf_detector->compute(img_3, kp_3, sf_descriptor1);
	surf_detector->compute(img_4, kp_4, sf_descriptor2);
	surf_detector->compute(img_1, kp_1, sf_descriptor3);
	surf_detector->compute(img_2, kp_2, sf_descriptor4);


	vector<DMatch> sf_matches1, si_matches1, sf_matches2, si_matches2, sf_matches3, si_matches3;
	SIFT_matcher.match(si_descriptor1, si_descriptor2, si_matches1);//F_1 VS F_1
	SURF_matcher.match(sf_descriptor1, sf_descriptor2, sf_matches1);

	SIFT_matcher.match(si_descriptor1, si_descriptor3, si_matches2);//F_1 VS TRUCK
	SURF_matcher.match(sf_descriptor1, sf_descriptor3, sf_matches2);

	SIFT_matcher.match(si_descriptor1, si_descriptor4, si_matches3);//F_1 VS CAR
	SURF_matcher.match(sf_descriptor1, sf_descriptor4, sf_matches3);

	drawMatches(img_3, kp_3, img_4, kp_4, si_matches1, simatch1, Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);
	drawMatches(img_3, kp_3, img_4, kp_4, sf_matches1, sfmatch1,Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

	drawMatches(img_3, kp_3, img_1, kp_1, si_matches2, simatch2, Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);
	drawMatches(img_3, kp_3, img_1, kp_1, sf_matches2, sfmatch2, Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

	drawMatches(img_3, kp_3, img_2, kp_2, si_matches3, simatch3, Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);
	drawMatches(img_3, kp_3, img_2, kp_2, sf_matches3, sfmatch3, Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

	imshow("SIFT MATCHING1", simatch1);
	imshow("SURF MATCHING1", sfmatch1);

	imshow("SIFT MATCHING2", simatch2);
	imshow("SURF MATCHING2", sfmatch2);

	imshow("SIFT MATCHING3", simatch3);
	imshow("SURF MATCHING3", sfmatch3);
	
	/*imwrite("SIFT MATCHING1.jpg", simatch1);
	imwrite("SURF MATCHING1.jpg", sfmatch1);

	imwrite("SIFT MATCHING2.jpg", simatch2);
	imwrite("SURF MATCHING2.jpg", sfmatch2);

	imwrite("SIFT MATCHING3.jpg", simatch3);
	imwrite("SURF MATCHING3.jpg", sfmatch3);*/
	BOWKMeansTrainer BGWDS(8);
	BGWDS.add(si_descriptor1);
	BGWDS.add(si_descriptor3);
	BGWDS.add(si_descriptor4);
	Mat vocabulary = BGWDS.cluster();
	vector<int> hist1, hist2, hist3, hist4;
	hist1.resize(vocabulary.rows);
	hist2.resize(vocabulary.rows);
	hist3.resize(vocabulary.rows);
	hist4.resize(vocabulary.rows);
    
	hist1 = histogram(si_descriptor1, vocabulary);
	hist2 = histogram(si_descriptor2, vocabulary);
	hist3 = histogram(si_descriptor3, vocabulary);
	hist4 = histogram(si_descriptor4, vocabulary);
	
	FILE *fp = fopen("hist1.txt", "w");
	for (int i = 0; i<hist1.size(); i++)
	{
		fprintf(fp, "%d\n", hist1[i]);
	}
	fclose(fp);

	FILE *fp1 = fopen("hist2.txt", "w");
	for (int i = 0; i<hist2.size(); i++)
	{
		fprintf(fp1, "%d\n", hist2[i]);
	}
	fclose(fp1);

	FILE *fp2 = fopen("hist3.txt", "w");
	for (int i = 0; i<hist3.size(); i++)
	{
		fprintf(fp2, "%d\n", hist3[i]);
	}
	fclose(fp2);

	FILE *fp3 = fopen("hist4.txt", "w");
	for (int i = 0; i<hist4.size(); i++)
	{
		fprintf(fp3, "%d\n", hist4[i]);
	}
	fclose(fp3);
	cout << "分类完毕";
	waitKey(0);
	
    return 0;
}

