// texturecls.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;
int findmin(double x, double y, double z, double w){
		int min,flag;
		if (x <= y) {
			min = x;
			flag = 1;
		}
		else {
			min = y;
			flag = 2;
		}
		if (z < min) {
			min = z;
			flag = 3;
		}
		if (w < min) {
			min = w;
			flag = 4;
		}
		return flag;
}
vector<double> normalization(vector<unsigned char> pix,int imageHeight,int imageWidth){
	vector<double>pix_db;
	pix_db.resize(imageWidth*imageHeight);
	double sum_temp = 0;
	double avg_temp = 0;
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_db[r*imageWidth + c] = (double)pix[r*imageWidth + c];
			sum_temp += pix_db[r*imageWidth + c];
		}
	}
	avg_temp = sum_temp / (imageHeight*imageWidth);
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_db[r*imageWidth + c] = pix_db[r*imageWidth + c]-avg_temp;
		}
	}
	return pix_db;
}
double cal_eng(vector<double>db, int imageHeight, int imageWidth) {
	double avg_temp, sum_temp;
	avg_temp = sum_temp = 0;
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			sum_temp += pow(db[r*imageWidth + c],2);
		}
	}
	avg_temp = sum_temp / (imageHeight*imageWidth);
	return avg_temp;
}
vector<vector<double>> tensorproduct(double x[5], double y[5]) {
	int sz = 5;
	vector<vector<double>>temp;
	temp.resize(sz);
	for (int i = 0; i < sz; i++) {
		temp[i].resize(sz);
	}
	for (int r = 0; r < sz; r++) {
		for (int c = 0; c < sz; c++) {
			temp[r][c] = x[r] * y[c];
		}
	}
	return temp;

}


vector<vector<double>> extension(vector<double> db, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> extimg1;
	extimg1.resize(imageHeight + 2 * floor(N1 / 2));
	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg1[i].resize(imageWidth + 2 * floor(N1 / 2));
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r + N1 / 2][c + N1 / 2] = db[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r][c + N1 / 2] = db[(N1 / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg1[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = db[(imageHeight - 1 - N1 / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N1 / 2); r++) {
		for (int c = 0; c < N1 / 2; c++) {
			extimg1[r][c] = extimg1[r][N1 - 1 - c];
			extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];
		}
	}
	return extimg1;
}
vector<double> convolution(vector<vector<double>> db_ext, vector<vector<double>> L_F, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<double> pixel_fl;
	pixel_fl.resize(imageHeight*imageWidth*BytesPerPixel);
	double temp = 0.0;
	for (int i = 0; i < imageHeight; i++){
		for (int j = 0; j < imageWidth; j++) {
			for (int m = -N1/2; m <=N1/2 ; m++) {
			    for (int n = -N1 / 2; n <= N1 / 2; n++) {
					temp += db_ext[i + N1 / 2 + m][j + N1 / 2+ n] * L_F[m + N1 / 2][n + N1 / 2];
				}
			}
			pixel_fl[i*imageWidth + j] = temp;
			temp = 0;
		}
	}
	return pixel_fl;
}
int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int nbpp = 1;
	double sum = 0;
	double avg;
	int N_f = 9;
	int N = 5;
	double E5[5] = {-1.0 / 6, -2.0 / 6, 0, 2.0 / 6, 1.0 / 6};
	double S5[5] = { -1.0 / 4,  0, 2.0 / 4, 0 , -1.0 / 4 };
	double W5[5] = { -1.0 / 6, 2.0 / 6, 0, -2.0 / 6, 1.0 / 6 };

	vector<vector<unsigned char>> pixel_data; //原始数据 8-bit图像
	vector<vector<double>>pixel_db;          // 减掉均值后的double型图像
	vector<double>pixel_cov;                // 经过filter后的图像
	vector<string> inputFileName;
	vector<vector<vector<double>>> law_filter; //滤波器库
	vector<vector<vector<double>>> ext_img;   // 12个double型图像的拓展图
	vector<vector<double>>avg_eng;           //feature vector 存着12个图的9个mask的平均能量
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName1 = argv[1];
	inputFileName.push_back(inputFileName1);

	string inputFileName2 = argv[2];
	inputFileName.push_back(inputFileName2);

	string inputFileName3 = argv[3];
	inputFileName.push_back(inputFileName3);

	string inputFileName4 = argv[4];
	inputFileName.push_back(inputFileName4);

	string inputFileName5 = argv[5];
	inputFileName.push_back(inputFileName5);

	string inputFileName6 = argv[6];
	inputFileName.push_back(inputFileName6);

	string inputFileName7 = argv[7];
	inputFileName.push_back(inputFileName7);

	string inputFileName8 = argv[8];
	inputFileName.push_back(inputFileName8);

	string inputFileName9 = argv[9];
	inputFileName.push_back(inputFileName9);

	string inputFileName10 = argv[10];
	inputFileName.push_back(inputFileName10);

	string inputFileName11 = argv[11];
	inputFileName.push_back(inputFileName11);

	string inputFileName12 = argv[12];
	inputFileName.push_back(inputFileName12);

	// Check if image is grayscale or color
	if (argc < 4) {
		BytesPerPixel = 1;// default is grey image
		imageHeight = 256;
		imageWidth = 256;
	}
	else {
		BytesPerPixel = atoi(argv[13]);
		// Check if size is specified
		if (argc >= 5) {
			imageWidth = atoi(argv[14]);
			imageHeight = atoi(argv[15]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pixel_data.resize(12);
	pixel_db.resize(12);
	pixel_cov.resize(imageHeight*imageWidth);
	avg_eng.resize(12);
	ext_img.resize(12);
	law_filter.resize(N_f);

	for (int i = 0; i < N_f; i++) {
		law_filter[i].resize(N);
		for (int j = 0; j < N; j++) {
			law_filter[i][j].resize(N);
		}
	}

	for (int i = 0; i < 12; i++) {
		avg_eng[i].resize(9);
	}

	for (int i = 0; i < 12; i++) {
		ext_img[i].resize(imageHeight + 2 * floor(N / 2));
		for (int j = 0; j <imageHeight + 2 * floor(N / 2); j++) {
			ext_img[i][j].resize(imageWidth + 2 * floor(N / 2));
		}
	}
	for (int i = 0; i < 12; i++) {
		pixel_data[i].resize(imageWidth*imageHeight*BytesPerPixel, 0);
		pixel_db[i].resize(imageWidth*imageHeight*BytesPerPixel, 0);
		
	}
	
	law_filter[0] = tensorproduct(E5, E5);
	law_filter[1] = tensorproduct(E5, S5);
	law_filter[2] = tensorproduct(E5, W5);

	law_filter[3] = tensorproduct(S5, S5);
	law_filter[4] = tensorproduct(S5, E5);
	law_filter[5] = tensorproduct(S5, W5);

	law_filter[6] = tensorproduct(W5, W5);
	law_filter[7] = tensorproduct(W5, E5);
	law_filter[8] = tensorproduct(W5, S5);

	FILE* inputFile;
	for (int i = 0; i < 12; i++) {
		if (!(inputFile = fopen(inputFileName[i].c_str(), "rb"))) {
			cout << "Cannot open inputfile: " << inputFile << endl;
			exit(1);
		}
		fread(&pixel_data[i][0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	}
	fclose(inputFile);
	cout << "loading data success!";
	
	for (int i = 0; i < 12; i++) {
		pixel_db[i] = normalization(pixel_data[i], imageHeight, imageWidth); 
	}
	//img_extension
	for (int i = 0; i < 12; i++) {
		ext_img[i] = extension(pixel_db[i], imageHeight, imageWidth, BytesPerPixel, N);
	}
	//convolution & calculate avg engergy
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 9; j++) {
			pixel_cov = convolution(ext_img[i], law_filter[j], imageHeight, imageWidth, BytesPerPixel, N);
			avg = cal_eng(pixel_cov, imageHeight, imageWidth);
			avg_eng[i][j] = avg;
		}
	}
	cout<< "calculation finished";
	//kmeans

	vector<vector<double>> k_vec;
	vector<double> D_1, D_2, D_3, D_4;
	k_vec.resize(4);
	D_1.resize(12);
	D_2.resize(12);
	D_3.resize(12);
	D_4.resize(12);
	for (int i = 0; i < 4; i++) {
		k_vec[i].resize(9);
	}
	k_vec[0] = { 300,200,70,120,200,42,16,53,32};
	k_vec[1] = { 50, 35,10,20,25,7,2,7,6};
	k_vec[2] = { 20,9,3,4,10,1,0.6,3,1.2};
	k_vec[3] = {2,2,1,2,2,1,1,1,1};
	cout << "k_vec ini finished" << endl;
	
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 12; i++) {
			 
		}
	}
	//欧氏距离判决
	double d1, d2, d3, d4,temp1,temp2,temp3,temp4;
	int flag;
	vector<int> c1_index;
	vector<int> c2_index;
	vector<int> c3_index;
	vector<int> c4_index;
	vector<double>sum1, sum2, sum3, sum4; 
	sum1.resize(9);
	sum2.resize(9);
	sum3.resize(9);
	sum4.resize(9);
	for (int i = 0; i < 100; i++) {
		d1 = 0;
		d2 = 0;
		d3 = 0;
		d4 = 0;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 9; j++) {
				temp1 = pow(avg_eng[i][j] - k_vec[0][j], 2);
				temp2 = pow(avg_eng[i][j] - k_vec[1][j], 2);
				temp3 = pow(avg_eng[i][j] - k_vec[2][j], 2);
				temp4 = pow(avg_eng[i][j] - k_vec[3][j], 2);
				d1 += temp1;
				d2 += temp2;
				d3 += temp3;
				d4 += temp4;
			}
			D_1[i] = sqrt(d1);
			D_2[i] = sqrt(d2);
			D_3[i] = sqrt(d3);
			D_4[i] = sqrt(d4);
			d1 = 0;
			d2 = 0;
			d3 = 0;
			d4 = 0;
		}
		//classify
	
		

		for (int i = 0; i < 12; i++)
		{
			flag = findmin(D_1[i], D_2[i], D_3[i], D_4[i]);
			switch (flag)
			{
			case 1:
				c1_index.push_back(i);
				break;

			case 2:
				c2_index.push_back(i);
				break;
			case 3:
				c3_index.push_back(i);
				break;
			case 4:
				c4_index.push_back(i);
				break;
			}
		}
		//更新centeroids
		if (c1_index.size() != 0) {
			for (int i = 0; i < c1_index.size(); i++) {
				for (int j = 0; j < 9; j++) {
					sum1[j] += avg_eng[c1_index[i]][j] / c1_index.size();
				}
			}
			k_vec[0] = sum1;
			for (int i = 0; i < 9; i++) {
				sum1[i] = 0;
			}
		}


		if (c2_index.size() != 0) {
			for (int i = 0; i < c2_index.size(); i++) {
				for (int j = 0; j < 9; j++) {
					sum2[j] += avg_eng[c2_index[i]][j] / c2_index.size();
				}
			}
			k_vec[1] = sum2;
			for (int i = 0; i < 9; i++) {
				sum2[i] = 0;
			}
		}


		if (c3_index.size() != 0) {
			for (int i = 0; i < c3_index.size(); i++) {
				for (int j = 0; j < 9; j++) {
					sum3[j] += avg_eng[c3_index[i]][j] / c3_index.size();
				}
			}
			k_vec[2] = sum3;
			for (int i = 0; i < 9; i++) {
				sum3[i] = 0;
			}
		}


		if (c4_index.size() != 0) {
			for (int i = 0; i < c4_index.size(); i++) {
				for (int j = 0; j < 9; j++) {
					sum4[j] += avg_eng[c4_index[i]][j] / c4_index.size();
				}
			}
			k_vec[3] = sum4;
			for (int i = 0; i < 9; i++) {
				sum4[i] = 0;
			}
		}
		if (i != 99) {
			c1_index.clear();
			c2_index.clear();
			c3_index.clear();
			c4_index.clear();
		}
	}
	cout << "The index of images that belong to rock is: ";
	for (int i = 0; i < c1_index.size(); i++) {
		cout << c1_index[i]+1 << ",";
	}
	cout << endl;
	cout << "The number of images that belong to grass is: ";
	for (int i = 0; i < c2_index.size(); i++) {
		cout << c2_index[i]+1 << ",";
	}
	cout << endl;
	cout << "The number of images that belong to weave is: ";
	for (int i = 0; i < c3_index.size(); i++) {
		cout << c3_index[i] + 1 << ",";
	}
	cout << endl;
	cout << "The number of images that belong to rock is: ";
	for (int i = 0; i < c4_index.size(); i++) {
		cout << c4_index[i] + 1 << ",";
	}
	return 0;
}

