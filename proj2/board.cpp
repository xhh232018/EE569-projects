#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
#include<string>
#include<queue>
using namespace std;
bool compare(unsigned char win[3][3], unsigned char s[3][3]) {
	int count = 0;
	if ((win[0][0] == s[0][0]) && (win[0][1] == s[0][1]) && (win[0][2] == s[0][2]))
	{
		count++;
	}
	if ((win[1][0] == s[1][0]) && (win[1][1] == s[1][1]) && (win[1][2] == s[1][2]))
	{
		count++;
	}
	if ((win[2][0] == s[2][0]) && (win[2][1] == s[2][1]) && (win[2][2] == s[2][2]))
	{
		count++;
	}
	if (count == 3) {
		return true;
	}
	else
	{
		return false;
	}
}
bool dcompare(unsigned char win[3][3], unsigned char d[3][3]) {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if ((win[i][j] == d[i][j]) || (d[i][j] == 2))
			{
				count++;
			}

		}
	}
	if (count == 9) {
		return true;
	}
	else
	{
		return false;
	}
}
unsigned char maskhit(unsigned char s1[4][3][3], unsigned char s2[4][3][3], unsigned char s3[8][3][3], unsigned char s4[4][3][3],
	unsigned char s5[8][3][3], unsigned char s6[10][3][3], unsigned char s7[4][3][3], unsigned char s8[4][3][3],
	unsigned char s9[8][3][3], unsigned char s10[4][3][3], int b, unsigned char m_window[3][3]) {
	unsigned char temp1 = 0;
	switch (b)
	{
	case 1:
		if (compare(m_window, s1[0]) || compare(m_window, s1[1]) || compare(m_window, s1[2]) || compare(m_window, s1[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 2:
		if (compare(m_window, s2[0]) || compare(m_window, s2[1]) || compare(m_window, s2[2]) || compare(m_window, s2[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 3:
		if (compare(m_window, s3[0]) || compare(m_window, s3[1]) || compare(m_window, s3[2]) || compare(m_window, s3[3]) ||
			compare(m_window, s3[4]) || compare(m_window, s3[5]) || compare(m_window, s3[6]) || compare(m_window, s3[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 4:
		if (compare(m_window, s4[1]) || compare(m_window, s4[2]) || compare(m_window, s4[3]) || compare(m_window, s4[0])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 5:
		if (compare(m_window, s5[0]) || compare(m_window, s5[1]) || compare(m_window, s5[2]) || compare(m_window, s5[3]) ||
			compare(m_window, s5[4]) || compare(m_window, s5[5]) || compare(m_window, s5[6]) || compare(m_window, s5[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 6:
		if (compare(m_window, s6[0]) || compare(m_window, s6[1]) || compare(m_window, s6[2]) || compare(m_window, s6[3]) ||
			compare(m_window, s6[4]) || compare(m_window, s6[5]) || compare(m_window, s6[6]) || compare(m_window, s6[7]) ||
			compare(m_window, s6[8]) || compare(m_window, s6[9])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 7:
		if (compare(m_window, s7[0]) || compare(m_window, s7[1]) || compare(m_window, s7[2]) || compare(m_window, s7[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 8:
		if (compare(m_window, s8[0]) || compare(m_window, s8[1]) || compare(m_window, s8[2]) || compare(m_window, s8[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 9:
		if (compare(m_window, s9[0]) || compare(m_window, s9[1]) || compare(m_window, s9[2]) || compare(m_window, s9[3]) ||
			compare(m_window, s9[4]) || compare(m_window, s9[5]) || compare(m_window, s9[6]) || compare(m_window, s9[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 10:
		if (compare(m_window, s10[0]) || compare(m_window, s10[1]) || compare(m_window, s10[2]) || compare(m_window, s10[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	}
	return temp1;
}

int main(int argc, char* argv[]) {
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int N = 3;
	int bond;
	int tempi;
	int temp_s = 0;
	int row, col;
	vector<unsigned char> pix;
	vector<vector<unsigned char> > pix_cp;
	vector<vector<unsigned char> > pix_bi;
	vector<vector<unsigned char> > intermid;
	vector<vector<unsigned char> > extimg;
	vector<unsigned char> pix_out;
	vector<vector<unsigned char> > pix_iter;
	vector<string> u_ptn;
	vector<pair<int, int> > index;
	vector<vector<unsigned char> > visited;
	vector<vector<pair<int,int> > > jig;
	vector<pair<int, int> >group;
	queue<pair<int, int> > q;
	int flag[16];
	vector<int>size;
	int N_ck = 41;
	unsigned char window[3][3];
	unsigned char s[3][3];
	unsigned char temp;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	string match;
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
	unsigned char s_1[4][3][3] =
	{ { { 0,0,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 1,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,0,1 } } };

	unsigned char s_2[4][3][3] =
	{ { { 0,0,0 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,0 } } };

	unsigned char s_3[8][3][3] =
	{ { { 0,0,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,1 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,0,1 } } };


	unsigned char s_4[4][3][3] =
	{ { { 0,0,1 },{ 0,1,1 },{ 0,0,1 } },
	{ { 1,1,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 1,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 1,1,1 } } };



	unsigned char s_5[8][3][3] =
	{ { { 1,1,0 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,0 },{ 0,1,1 },{ 0,0,1 } },
	{ { 0,1,1 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,1 },{ 0,1,1 },{ 0,1,0 } },{ { 0,1,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,1,1 } } };


	unsigned char s_6[10][3][3] =
	{ { { 1,1,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,1 },{ 0,1,1 },{ 0,0,1 } },
	{ { 1,1,1 },{ 1,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 1,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,1,1 } },
	{ { 0,0,0 },{ 0,1,1 },{ 1,1,1 } },
	{ { 0,0,1 },{ 0,1,1 },{ 0,1,1 } },{ { 1,1,0 },{ 0,1,1 },{ 0,0,1 } },
	{ { 0,1,1 },{ 1,1,0 },{ 1,0,0 } } };


	unsigned char s_7[4][3][3] =
	{ { { 1,1,1 },{ 0,1,1 },{ 0,0,1 } },
	{ { 1,1,1 },{ 1,1,0 },{ 1,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 1,1,1 } },
	{ { 0,0,1 },{ 0,1,1 },{ 1,1,1 } } };

	unsigned char s_8[4][3][3] = { { { 0,1,1 },{ 0,1,1 },{ 0,1,1 } },
	{ { 1,1,1 },{ 1,1,1 },{ 0,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 1,1,1 },{ 1,1,1 } } };

	unsigned char s_9[8][3][3] =
	{ { { 1,1,1 },{ 0,1,1 },{ 0,1,1 } },
	{ { 0,1,1 },{ 0,1,1 },{ 1,1,1 } },
	{ { 1,1,1 },{ 1,1,1 },{ 1,0,0 } },
	{ { 1,1,1 },{ 1,1,1 },{ 0,0,1 } },
	{ { 1,1,1 },{ 1,1,0 },{ 1,1,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 1,1,1 } },
	{ { 1,0,0 },{ 1,1,1 },{ 1,1,1 } },
	{ { 0,0,1 },{ 1,1,1 },{ 1,1,1 } } };


	unsigned char s_10[4][3][3] =
	{ { { 1,1,1 },{ 0,1,1 },{ 1,1,1 } },
	{ { 1,1,1 },{ 1,1,1 },{ 1,0,1 } },
	{ { 1,1,1 },{ 1,1,0 },{ 1,1,1 } },
	{ { 1,0,1 },{ 1,1,1 },{ 1,1,1 } } };

	unsigned char unc_nd[28][3][3] = {
		//without D
		// Spur
		{ { 0,0,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,0 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,0,0 } },

	//L cluster
	{ { 0,0,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,1 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,0,1 } },

	//4-Connected offest
	{ { 0,1,1 },{ 1,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,0 },{ 0,1,1 },{ 0,0,1 } },
	{ { 0,0,1 },{ 0,1,1 },{ 0,1,0 } },


	//Spur corner cluster
	{ { 0,1,1 },{ 0,1,1 },{ 1,0,0 } },{ { 0,0,1 },{ 0,1,1 },{ 1,0,0 } },{ { 0,1,1 },{ 0,1,0 },{ 1,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 0,0,1 } },{ { 1,0,0 },{ 1,1,0 },{ 0,0,1 } },{ { 1,1,0 },{ 0,1,0 },{ 0,0,1 } },
	{ { 0,0,1 },{ 1,1,0 },{ 1,1,0 } },{ { 0,0,1 },{ 1,1,0 },{ 1,0,0 } },{ { 0,0,1 },{ 0,1,0 },{ 1,1,0 } },
	{ { 1,0,0 },{ 0,1,1 },{ 0,1,1 } },{ { 1,0,0 },{ 0,1,0 },{ 0,1,1 } },{ { 1,0,0 },{ 0,1,1 },{ 0,0,1 } },


	};

	unsigned char unc_d[41][3][3] = {
		//With D
		//corner cluster
		{ { 1,1,2 },{ 1,1,2 },{ 2,2,2 } },

		//Tree branch
	{ { 2,1,0 },{ 1,1,1 },{ 2,0,0 } },
	{ { 0,1,2 },{ 1,1,1 },{ 0,0,2 } },
	{ { 0,0,2 },{ 1,1,1 },{ 0,1,2 } },
	{ { 2,0,0 },{ 1,1,1 },{ 2,1,0 } },
	{ { 2,1,2 },{ 1,1,0 },{ 0,1,0 } },
	{ { 0,1,0 },{ 1,1,0 },{ 2,1,2 } },
	{ { 0,1,0 },{ 0,1,1 },{ 2,1,2 } },
	{ { 2,1,2 },{ 0,1,1 },{ 0,1,0 } },

	//Vee branch
	{ { 1,2,1 },{ 2,1,2 },{ 0,0,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 0,1,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 0,1,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,0,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,0,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,1,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,1,1 } },

	{ { 1,2,1 },{ 2,1,0 },{ 1,2,0 } },
	{ { 1,2,0 },{ 2,1,1 },{ 1,2,0 } },
	{ { 1,2,0 },{ 2,1,0 },{ 1,2,1 } },
	{ { 1,2,1 },{ 2,1,1 },{ 1,2,0 } },
	{ { 1,2,1 },{ 2,1,0 },{ 1,2,1 } },
	{ { 1,2,0 },{ 2,1,1 },{ 1,2,1 } },
	{ { 1,2,1 },{ 2,1,1 },{ 1,2,1 } },

	{ { 0,0,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 0,1,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,0,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 0,1,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,1,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,0,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,1,1 },{ 2,1,2 },{ 1,2,1 } },

	{ { 1,2,1 },{ 0,1,2 },{ 0,2,1 } },
	{ { 0,2,1 },{ 1,1,2 },{ 0,2,1 } },
	{ { 0,2,1 },{ 0,1,2 },{ 1,2,1 } },
	{ { 1,2,1 },{ 1,1,2 },{ 0,2,1 } },
	{ { 1,2,1 },{ 0,1,2 },{ 1,2,1 } },
	{ { 0,2,1 },{ 1,1,2 },{ 1,2,1 } },
	{ { 1,2,1 },{ 1,1,2 },{ 1,2,1 } },

	//Diagonal branch
	{ { 2,1,0 },{ 0,1,1 },{ 1,0,2 } },
	{ { 0,1,2 },{ 1,1,0 },{ 2,0,1 } },
	{ { 2,0,1 },{ 1,1,0 },{ 0,1,2 } },
	{ { 1,0,2 },{ 0,1,1 },{ 2,1,0 } }

	};


	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	intermid.resize(imageHeight);
	pix_cp.resize(imageHeight);
	pix_bi.resize(imageHeight);
	pix_iter.resize(imageHeight);
	visited.resize(imageHeight);
	pix_out.resize(imageHeight*imageWidth*BytesPerPixel, 0);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!" << endl;
	for (int i = 0; i < imageHeight; i++) {
		intermid[i].resize(imageWidth);
		pix_cp[i].resize(imageWidth);
		pix_bi[i].resize(imageWidth);
		pix_iter[i].resize(imageWidth);
		visited[i].resize(imageWidth);
	}
	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
	}
	
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++)
		{
			if (pix[r*imageWidth + c] <= 127) {
				temp = 255;
			}
			else
			{
				temp = 0;
			}
			pix_cp[r][c] = temp;
			pix_bi[r][c] = temp;
		}
	}
	//image extension
	for (int iter_num = 0; iter_num < 45; iter_num++) {

		//判断 Stage I
		unsigned char pixel;
		for (int r = 1; r < imageHeight - 1; r++) {
			for (int c = 1; c < imageWidth - 1; c++) {
				for (int r1 = 0; r1 <= 2; r1++) {
					for (int c1 = 0; c1 <= 2; c1++) {
						window[r1][c1] = pix_cp[r + r1 - 1][c + c1 - 1] / 255;
					}
				}

				if (window[1][1] == 0) {
					intermid[r][c] = 0;
					continue;
				}
				else
				{
					bond = 2 * (window[0][1] + window[1][2] + window[1][0] + window[2][1]) + 1 * (window[0][0] + window[0][2] + window[2][0] + window[2][2]);
				}
				if (bond == 0 || bond == 11) {
					intermid[r][c] = 0;
					continue;
				}

				intermid[r][c] = maskhit(s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8, s_9, s_10, bond, window);

			}
		}
		//STAGE II 
		int flag1 = 0;
		int flag2 = 0;
		for (int i = 1; i < imageHeight - 1; i++) {
			for (int j = 1; j < imageWidth - 1; j++) {
				if (intermid[i][j] == 1) {
					for (int r = 0; r < N; r++) {
						for (int c = 0; c < N; c++) {
							window[r][c] = intermid[i + r - 1][j + c - 1];
						}
					}
				}
				else
				{
					pix_iter[i][j] = pix_cp[i][j];
					continue;
				}
				for (int i = 0; i < 27; i++) {
					if (compare(window, unc_nd[i])) {
						flag1 = 1;
						break;
					}
				}
				for (int i = 0; i < 41; i++) {
					if (dcompare(window, unc_d[i])) {
						flag2 = 1;
						break;
					}
				}
				if ((flag1 || flag2)) {
					pix_iter[i][j] = pix_cp[i][j];
					flag1 = 0;
					flag2 = 0;
					continue;
				}
				else
				{
					pix_iter[i][j] = 0;

				}
			}
		}
		pix_cp = pix_iter;
		cout << iter_num << "iteration successful!" << endl;
	}
	int number = 0;
	int number1 = 0;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_out[i*imageWidth + j] = pix_iter[i][j];
			if (pix_iter[i][j] == 255) {
				number++;
			}
		}
	}
	//修正
	int count = 0;
	for (int i = N_ck/2; i < imageHeight - N_ck/2; i++) {
		for (int j = N_ck / 2; j < imageWidth - N_ck / 2; j++) {
			if (pix_out[i*imageWidth + j] == 255) {
				for(int r = -N_ck/2;r<N_ck/2;r++){
					for (int c = -N_ck / 2; c < N_ck / 2; c++) {
						if (pix_out[(r + i)*imageWidth + (j + c)] == 255) {
							count++;
						}
					}
				}
			}
			if (count > 1) {
				pix_out[i*imageWidth + j] = 0;
			}
			count = 0;
		}
	}
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			if (pix_out[i*imageWidth + j] == 255) {
				number1++;
				index.push_back(make_pair(i, j));
			}
		}
	}
	cout<<"Total number of jigsaws is " << number1 << endl;
	jig.resize(number1);
	for (int i = 0; i < index.size(); i++) {
		q.push(index[i]);
		temp_s++;
		jig[i].push_back(index[i]);
		while (!q.empty())
		{
			row = q.front().first;
			col = q.front().second;
			visited[row][col] = 1;
			q.pop();
			//N
			if (pix_bi[row - 1][col] == 255 && visited[row - 1][col] != 1) {
				q.push(make_pair(row - 1, col));
				visited[row - 1][col] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row - 1, col));
			}
			//S
			if (pix_bi[row + 1][col] == 255 && visited[row + 1][col] != 1) {
				q.push(make_pair(row + 1, col));
				visited[row + 1][col] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row + 1, col));
			}
			//W
			if (pix_bi[row][col - 1] == 255 && visited[row][col - 1] != 1) {
				q.push(make_pair(row, col - 1));
				visited[row][col - 1] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row, col-1));
			}
			//E
			if (pix_bi[row][col + 1] == 255 && visited[row][col + 1] != 1) {
				q.push(make_pair(row, col + 1));
				visited[row][col + 1] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row, col+1));
			}
			//NE
			if (pix_bi[row-1][col + 1] == 255 && visited[row-1][col + 1] != 1) {
				q.push(make_pair(row-1, col + 1));
				visited[row-1][col + 1] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row-1, col + 1));
			}
			//NW
			if (pix_bi[row - 1][col - 1] == 255 && visited[row - 1][col - 1] != 1) {
				q.push(make_pair(row - 1, col - 1));
				visited[row - 1][col - 1] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row - 1, col - 1));
			}
			//SE
			if (pix_bi[row+1][col + 1] == 255 && visited[row+1][col + 1] != 1) {
				q.push(make_pair(row+1, col + 1));
				visited[row+1][col + 1] = 1;
				temp_s++;
				jig[i].push_back(make_pair(row+1, col + 1));
			}
			//SW
			if (pix_bi[row+1][col - 1] == 255 && visited[row+1][col - 1] != 1) {
			q.push(make_pair(row + 1, col - 1));
			visited[row + 1][col - 1] = 1;
			temp_s++;
			jig[i].push_back(make_pair(row + 1, col - 1));
		    }
		}
		size.push_back(temp_s);
		temp_s = 0;
	}
	for (int i = 0; i < size.size(); i++) {
		for (int j = i + 1; j < size.size(); j++) {
			if (abs(size[i] - size[j]) < 20 ) {
				group.push_back(make_pair(i, j));
			}
		}
	}
	sort(jig[0].begin(), jig[0].end());
	vector<pair<int,int> > shape;
	vector<pair<int, int> > std_pos;
	int wd;
	int ht;
	int wd_min;
	int ht_min;
	for (int i = 0; i < jig.size(); i++) {
		sort(jig[i].begin(), jig[i].end());
		ht = jig[i].back().first - jig[i].front().first;
		ht_min = jig[i].front().first;
		for (int j = 0; j < jig[i].size(); j++) {
			swap(jig[i][j].first, jig[i][j].second);
		}
		sort(jig[i].begin(), jig[i].end());
		wd = jig[i].back().first - jig[i].front().first;
		wd_min = jig[i].front().first;
		shape.push_back(make_pair(ht, wd));
		std_pos.push_back(make_pair(ht_min, wd_min));
		for (int j = 0; j < jig[i].size(); j++) {
			swap(jig[i][j].first, jig[i][j].second);
		}
	}

	//把各个jigsaw的像素值存进小vector
	vector<vector<vector<unsigned char> > > J;
	J.resize(shape.size());
	for ( int i = 0; i < shape.size(); i++) {
		J[i].resize(shape[i].first);
		for (int j = 0; j < shape[i].first; j++) {
			J[i][j].resize(shape[i].second);
			for (int k = 0; k < shape[i].second; k++) {
				J[i][j][k] = pix_bi[j + std_pos[i].first][k + std_pos[i].second];
			}
		}
	}
	//比
	int w_1, w_2;
	int h_1, h_2;
	int n_1, n_2;
	double similar_pct1, similar_pct2;
	double count1, count2;
	vector<double> pct;
	for (int i = 0; i < group.size();i++) {
		count1 = 0;
		count2 = 0;
		n_1 = group[i].first;
		n_2 = group[i].second;
		h_1 = shape[n_1].first;
		w_1 = shape[n_1].second;
		h_2 = shape[n_2].first;
		w_2 = shape[n_2].second;
		if ((w_1 == w_2) && (h_1 == h_2)) {
			//0度旋转
			for (int r = 0; r < h_1; r++) {
				for (int c = 0; c < w_1; c++) {
					if (J[n_1][r][c] == J[n_2][r][c]) {
						count1++;
					}
				}
			}
			//180度旋转
			if (w_1 < h_1) {
				for (int r = 0; r < h_1; r++) {
					for (int c = 0; c < w_1; c++) {
						if (J[n_1][r][c] == J[n_2][h_2 - r - 1][c]) {
							count2++;
						}
					}
				}
			}
			if (w_1 > h_1) {
				for (int r = 0; r < h_1; r++) {
					for (int c = 0; c < w_1; c++) {
						if (J[n_1][r][c] == J[n_2][r][w_2-1-c]) {
							count2++;
						}
					}
				}
			}
		}
		if ((w_1 == h_2) && (h_1 == w_2) && (w_1 != h_1)) {
			//+90度旋转

			for (int r = 0; r < h_1; r++) {
				for (int c = 0; c < w_1; c++) {
					if (J[n_1][r][c] == J[n_2][c][h_1 - r - 1]) {
						count1++;
					}
				}
			}
			//-90度旋转
			if (w_1 < h_1) {
				for (int r = 0; r < h_1; r++) {
					for (int c = 0; c < w_1; c++) {
						if (J[n_1][r][c] == J[n_2][c][r]) {
							count2++;
						}
					}
				}
			}
			if (w_1 > h_1) {
				for (int r = 0; r < h_1; r++) {
					for (int c = 0; c < w_1; c++) {
						if (J[n_1][r][c] == J[n_2][h_2-1-c][r]) {
							count2++;
						}
					}
				}
			}
		}
		similar_pct1 = count1 / (w_1*h_1);
		similar_pct2 = count2 / (w_1*h_1);
		if (similar_pct1 > similar_pct2) {
			pct.push_back(similar_pct1);
		}
		else
		{
			pct.push_back(similar_pct2);
		}

	}
	vector<pair<int, int > > same_pair;
	int sg[16];
	int pair_ct = 0;;
	for (int i = 0; i < pct.size(); i++) {
		if (pct[i] > 0.9) {
			same_pair.push_back(group[i]);
			if (sg[group[i].first] != 1) {
				sg[group[i].first] = 1;
				pair_ct++;
			}
			if (sg[group[i].second] != 1) {
				sg[group[i].second] = 1;
				pair_ct++;
			}
		}
	}
	cout << "There are " << same_pair.size() << " pairs in the board"<<endl;
	cout << "The number of unique jigsaws is " << index.size() - pair_ct << endl;
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
