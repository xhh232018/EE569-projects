// textureseg.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;
int findmin(double x, double y, double z, double w,double t,double q) {
	double min;
	int flag;
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
	if (t < min) {
		min = t;
		flag = 5;
	}
	if (q < min) {
		min = q;
		flag = 6;
	}
	return flag;
}
vector<double> dccomponent(vector<unsigned char> pix, int imageHeight, int imageWidth) {
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
			pix_db[r*imageWidth + c] = pix_db[r*imageWidth + c] - avg_temp;
		}
	}
	return pix_db;
}
vector<vector<double>> cal_eng(vector<vector<double>> ext_db, int imageHeight, int imageWidth, int N_LW) {
	vector<vector<double>> avg_temp, sum_temp;
	avg_temp.resize(imageHeight);
	sum_temp.resize(imageHeight);
	for (int i = 0; i < imageHeight; i++) {
		avg_temp[i].resize(imageWidth);
		sum_temp[i].resize(imageWidth);
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			for (int m = -N_LW / 2; m <=N_LW / 2; m++) {
				for (int n = -N_LW / 2; n <= N_LW / 2; n++) {
					sum_temp[r][c] += pow(ext_db[r+m+ N_LW / 2][c + n + N_LW / 2],2);
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			avg_temp[r][c] = sum_temp[r][c] / (N_LW*N_LW);
		}
	}
	return  avg_temp;
}
vector<vector<double>> tensorproduct(double x[3], double y[3]) {
	int sz = 3;
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

vector<vector<double>> extension_2D(vector<vector<double>> db, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> extimg1;
	extimg1.resize(imageHeight + 2 * floor(N1 / 2));
	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg1[i].resize(imageWidth + 2 * floor(N1 / 2));
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r + N1 / 2][c + N1 / 2] = db[r][c];
		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r][c + N1 / 2] = db[(N1 / 2 - r)][c];
			extimg1[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = db[(imageHeight - 1 - N1 / 2 + r)][c];
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

vector<vector<double>> convolution(vector<vector<double>> db_ext, vector<vector<double>> L_F, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> pixel_fl;
	pixel_fl.resize(imageHeight);
	for (int i = 0; i < imageHeight; i++) {
		pixel_fl[i].resize(imageWidth);
	}
	double temp = 0.0;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			for (int m = -N1 / 2; m <= N1 / 2; m++) {
				for (int n = -N1 / 2; n <= N1 / 2; n++) {
					temp += db_ext[i + N1 / 2 + m][j + N1 / 2 + n] * L_F[m + N1 / 2][n + N1 / 2];
				}
			}
			pixel_fl[i][j] = temp;
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
	int N_f = 9;
	int N = 3;
	int N_LW = 17;
	double L3[3] = { 1.0 / 6, 2.0 / 6, 1.0 / 6 };
	double W3[3] = { -1.0 / 2,  0, 1.0 / 2 };
	double S3[3] = { -1.0 / 2, 2.0 / 2, -1.0 / 2 };
	vector<unsigned char> pix;
	vector<unsigned char> pix_out;
	vector<vector<double>> extimg;
	vector<vector<double>>avg_eng;
	vector<double> pix_db;
	vector<vector<vector<double>>> fvecarr;
	vector<vector<vector<double>>> engarr;
	vector<vector<vector<double>>> law_filter;
	vector<vector<vector<double>>> norm_eng;
	vector<vector<double>> extimg_eng;
	unsigned char temp, temp1, temp2, temp3, temp4;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	// Check if image is grayscale or color
	if (argc < 4) {
		BytesPerPixel = 1;// default is grey image
		imageHeight = 256;
		imageWidth = 256;
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5) {
			imageWidth = atoi(argv[4]);
			imageHeight = atoi(argv[5]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_out.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_db.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	extimg_eng.resize(imageHeight + 2 * floor(N_LW / 2));
	fvecarr.resize(N_f);
	norm_eng.resize(N_f);
	law_filter.resize(N_f);
	engarr.resize(N_f);

	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
	}

	for (int i = 0; i < imageHeight + 2 * floor(N_LW / 2); i++) {
		extimg_eng[i].resize(imageWidth + 2 * floor(N_LW / 2));
	}

	for (int i = 0; i < N_f; i++) {
		law_filter[i].resize(N);
		for (int j = 0; j < N; j++) {
			law_filter[i][j].resize(N);
		}
	}

	for (int i = 0; i < N_f; i++) {
		fvecarr[i].resize(imageHeight);
		engarr[i].resize(imageHeight);
		norm_eng[i].resize(imageHeight);
		for (int j = 0; j < imageHeight; j++) {
			fvecarr[i][j].resize(imageWidth);
			engarr[i][j].resize(imageWidth);
			norm_eng[i][j].resize(imageWidth);
		}
	}

	law_filter[0] = tensorproduct(L3, L3);
	law_filter[1] = tensorproduct(L3, W3);
	law_filter[2] = tensorproduct(L3, S3);

	law_filter[3] = tensorproduct(S3, S3);
	law_filter[4] = tensorproduct(S3, W3);
	law_filter[5] = tensorproduct(S3, L3);

	law_filter[6] = tensorproduct(W3, W3);
	law_filter[7] = tensorproduct(W3, L3);
	law_filter[8] = tensorproduct(W3, S3);

	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	cout << "loading data success!";

	pix_db = dccomponent(pix, imageHeight, imageWidth);
	extimg = extension(pix_db, imageHeight, imageWidth, BytesPerPixel, N);

	for (int i = 0; i < N_f; i++) {
		fvecarr[i] = convolution(extimg, law_filter[i], imageHeight, imageWidth, BytesPerPixel, N);//convolve后的image
		extimg_eng = extension_2D(fvecarr[i], imageHeight, imageWidth, BytesPerPixel, N_LW);
		engarr[i] = cal_eng(extimg_eng, imageHeight, imageWidth, N_LW);
	}
	//normalization
	for (int k = 0; k < N_f; k++) {
		for (int i = 0; i < imageHeight; i++) {
			for (int j = 0; j < imageWidth; j++) {
				norm_eng[k][i][j] = engarr[k][i][j] / (engarr[0][i][j]);
			}
		}
	}

	//6clusters 6 reference points 
	vector<double>k1, k2, k3, k4, k5, k6;
	for (int i = 1; i < N_f; i++) {
		k1.push_back(norm_eng[i][89][102]);
	}
	for (int i = 1; i < N_f; i++) {
		k2.push_back(norm_eng[i][90][294]);
	}
	for (int i = 1; i < N_f; i++) {
		k3.push_back(norm_eng[i][78][484]);
	}
	for (int i = 1; i < N_f; i++) {
		k4.push_back(norm_eng[i][224][300]);
	}
	for (int i = 1; i < N_f; i++) {
		k5.push_back(norm_eng[i][357][67]);
	}
	for (int i = 1; i < N_f; i++) {
		k6.push_back(norm_eng[i][362][485]);
	}
	vector<double> d_1, d_2, d_3, d_4, d_5, d_6;
	vector<pair<int, int>> s_1, s_2, s_3, s_4, s_5, s_6;
	double d1_temp, d2_temp, d3_temp, d4_temp, d5_temp,d6_temp;
	double weight_tp = 0;
	int flag;
	d1_temp = 0;
	d2_temp = 0;
	d3_temp = 0;
	d4_temp = 0;
	d5_temp = 0;
	d6_temp = 0;
	//k_means training
	
	for (int n = 0; n < 50; n++) {
		for (int i = 0; i < imageHeight; i++) {
			for (int j = 0; j < imageWidth; j++) {
				for (int k = 1; k < N_f; k++) {
					d1_temp += pow(norm_eng[k][i][j] - k1[k - 1], 2);
					d2_temp += pow(norm_eng[k][i][j] - k2[k - 1], 2);
					d3_temp += pow(norm_eng[k][i][j] - k3[k - 1], 2);
					d4_temp += pow(norm_eng[k][i][j] - k4[k - 1], 2);
					d5_temp += pow(norm_eng[k][i][j] - k5[k - 1], 2);
					d6_temp += pow(norm_eng[k][i][j] - k6[k - 1], 2);
				}
				d_1.push_back(d1_temp);
				d_2.push_back(d2_temp);
				d_3.push_back(d3_temp);
				d_4.push_back(d4_temp);
				d_5.push_back(d5_temp);
				d_6.push_back(d6_temp);
				d1_temp = 0;
				d2_temp = 0;
				d3_temp = 0;
				d4_temp = 0;
				d5_temp = 0;
				d6_temp = 0;
			}
		}
		for (int i = 0; i < imageHeight; i++) {
			for (int j = 0; j < imageWidth; j++) {
				flag = findmin(d_1[i*imageWidth + j], d_2[i*imageWidth + j], d_3[i*imageWidth + j], d_4[i*imageWidth + j], d_5[i*imageWidth + j], d_6[i*imageWidth + j]);
				switch (flag)
				{
				case 1:
					s_1.push_back(make_pair(i, j));
					break;

				case 2:
					s_2.push_back(make_pair(i, j));
					break;
				case 3:
					s_3.push_back(make_pair(i, j));
					break;
				case 4:
					s_4.push_back(make_pair(i, j));
					break;
				case 5:
					s_5.push_back(make_pair(i, j));
					break;
				case 6:
					s_6.push_back(make_pair(i, j));
					break;
				}
			}
		}
		//updating centeroid

		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_1.size(); i++) {
				weight_tp += norm_eng[d][s_1[i].first][s_1[i].second];
			}
			k1[d - 1] = weight_tp / s_1.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_1.clear();
		}

		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_2.size(); i++) {
				weight_tp += norm_eng[d][s_2[i].first][s_2[i].second];
			}
			k2[d - 1] = weight_tp / s_2.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_2.clear();
		}

		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_3.size(); i++) {
				weight_tp += norm_eng[d][s_3[i].first][s_3[i].second];
			}
			k3[d - 1] = weight_tp / s_3.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_3.clear();
		}
		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_4.size(); i++) {
				weight_tp += norm_eng[d][s_4[i].first][s_4[i].second];
			}
			k4[d - 1] = weight_tp / s_4.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_4.clear();
		}

		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_5.size(); i++) {
				weight_tp += norm_eng[d][s_5[i].first][s_5[i].second];
			}
			k5[d - 1] = weight_tp / s_5.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_5.clear();
		}
		for (int d = 1; d < N_f; d++) {
			for (int i = 0; i < s_6.size(); i++) {
				weight_tp += norm_eng[d][s_6[i].first][s_6[i].second];
			}
			k6[d - 1] = weight_tp / s_6.size();
			weight_tp = 0;
		}
		if (n != 49) {
			s_6.clear();
		}

	}

	//coloring

		for (int i = 0; i < s_1.size(); i++) {
			pix_out[s_1[i].first*imageWidth + s_1[i].second] = 0;
		}
		
	
		for (int i = 0; i < s_2.size(); i++) {
			pix_out[s_2[i].first*imageWidth + s_2[i].second] = 51;

		}
		

	
		for (int i = 0; i < s_3.size(); i++) {
			pix_out[s_3[i].first*imageWidth + s_3[i].second] = 102;

		}
		
	
		for (int i = 0; i < s_4.size(); i++) {
			pix_out[s_4[i].first*imageWidth + s_4[i].second] = 153;

		}
		

	
		for (int i = 0; i < s_5.size(); i++) {
			pix_out[s_5[i].first*imageWidth + s_5[i].second] = 204;

		}
		

		for (int i = 0; i < s_6.size(); i++) {
			pix_out[s_6[i].first*imageWidth + s_6[i].second] = 255;

		}
		

	FILE* outputFile;
 	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_out[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);

	return 0;
}