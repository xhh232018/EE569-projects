#include<iostream>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<iterator>
#include<math.h>
#include<algorithm>
#include<stdio.h>
#define pi 3.14
using namespace std;
int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int neighbourwd;
	int searchwd;
	int sigma;
	int h;
        int N;
        unsigned char rtemp1, gtemp1, btemp1;
	vector<unsigned char> pixr_wd;
	vector<unsigned char> pixg_wd;
	vector<unsigned char> pixb_wd;
	double g_sum = 0.0;
	double delta_r, delta_g, delta_b;
	double dist_r, dist_g, dist_b, rtemp, gtemp, btemp,r_norm,b_norm,g_norm;
	double  centerwd_rpt, curr_rpt,centerwd_gpt, curr_gpt,centerwd_bpt, curr_bpt;
	rtemp = gtemp = btemp = r_norm= b_norm = g_norm = 0.0;
	vector<unsigned char> pix;
        vector<unsigned char> pixrf;
	vector<vector<vector<unsigned char> > > extimg;
        vector<vector<vector<unsigned char> > > med_ext;
	vector<vector<double> > gaussian_kn;
	vector<vector<vector<unsigned char> > > center_window;
	vector<vector<vector<double> > > weight;
	vector<double> weight_r;
	vector<double> weight_g;
	vector<double> weight_b;
	double rpix = 0.0;
	double gpix = 0.0;
	double bpix = 0.0;

	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName = argv[2];
        string inputFileName1 = argv[9];
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
			neighbourwd = atoi(argv[6]);
			searchwd = atoi(argv[7]);
			sigma = atoi(argv[8]);
			h = 10 * sigma;
                       
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
        pixrf.resize(imageWidth*imageHeight*BytesPerPixel, 0);
        pixr_wd.resize(N*N);
        pixg_wd.resize(N*N);	
	pixb_wd.resize(N*N);
	extimg.resize(imageHeight + 2 * floor(searchwd / 2));
	gaussian_kn.resize(neighbourwd);
	weight.resize(searchwd - neighbourwd + 1);
	center_window.resize(neighbourwd);
        med_ext.resize(imageHeight + 2 * floor(N / 2));
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
                
        if (!(inputFile = fopen(inputFileName1.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pixrf[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";
 
	//Step1 create Gaussian kernal
	for (int  i = 0; i < neighbourwd; i++)
	{
		gaussian_kn[i].resize(neighbourwd);
		center_window[i].resize(neighbourwd);
		for (int k = 0; k < neighbourwd; k++) {
			center_window[i][k].resize(BytesPerPixel);
		}
	}
	for (int i = 0; i <= searchwd - neighbourwd; i++)
	{
		weight[i].resize(searchwd - neighbourwd + 1);
		for (int k = 0; k <= searchwd - neighbourwd; k++) {
			weight[i][k].resize(BytesPerPixel);
		}
	}
	for (int m = 0; m < neighbourwd; m++) {
		for (int n = 0; n < neighbourwd; n++)
		{
			gaussian_kn[m][n] = (1 / (2.0*pi*sigma*sigma))*exp((-(pow((m - (neighbourwd - 1) / 2), 2) + pow((n - (neighbourwd - 1) / 2), 2))) / pow(sigma, 2));
			g_sum = g_sum + gaussian_kn[m][n];
                       
		}
	}
        //cout<<g_sum<<endl;
	double sum1_1 = 0.0;
	for (int m = 0; m < neighbourwd; m++) {
		for (int n = 0; n < neighbourwd; n++)
		{
			gaussian_kn[m][n] = (1 / g_sum)*gaussian_kn[m][n];
			sum1_1 += gaussian_kn[m][n];
		}
	}
	cout << sum1_1;
	//Step2 extend original image
	for (int i = 0; i < imageHeight + 2 * floor(searchwd / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(searchwd / 2));
		for (int j = 0; j < imageWidth + 2 * floor(searchwd / 2); j++) {
			extimg[i][j].resize(BytesPerPixel);
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r + searchwd / 2][c + searchwd / 2][0] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < searchwd / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r][c + searchwd / 2][0] = pix[(searchwd / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg[imageHeight + 2 * floor(searchwd / 2) - 1 - r][c + searchwd / 2][0] = pix[(imageHeight - 1 - searchwd / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];


		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(searchwd / 2); r++) {
		for (int c = 0; c < searchwd / 2; c++) {
			extimg[r][c][0] = extimg[r][searchwd - 1 - c][0];
			extimg[r][imageWidth + 2 * floor(searchwd / 2) - 1 - c][0] = extimg[r][imageWidth + 2 * floor(searchwd / 2) - 1 - 2 * floor(searchwd / 2) + c][0];
		}
	}//用时2s
	cout << "图像拓展完毕"<<endl;
	//step3 进行NLM处理 原有图像行->原有图像列->搜索框的行->搜索框的列->每一个小neibour 
	for (int index_rori = 0; index_rori < imageHeight; index_rori++) {
		for (int index_cori = 0; index_cori < imageWidth; index_cori++) {
		
			//在搜索框中遍历所有的neighbor框
			for (int index_rsch = 0; index_rsch <= searchwd - neighbourwd; index_rsch++) {
				for (int index_csch = 0; index_csch <= searchwd - neighbourwd; index_csch++) {
					//neighbor框内部遍历
					for (int i = 0; i < neighbourwd; i++) {
						for (int j = 0; j < neighbourwd; j++) {

							center_window[i][j][0] = extimg[searchwd / 2 - 1 + index_rori + i][searchwd / 2 - 1 + index_cori + j][0];
							
					
							delta_r = (double)(extimg[index_rori+index_rsch+i][index_cori+index_csch+j][0]-center_window[i][j][0]);
							dist_r = gaussian_kn[i][j] * pow(delta_r, 2);
							rtemp += dist_r;

						}
					}
					weight[index_rsch][index_csch][0] = exp(-rtemp / (h*h));
					r_norm += weight[index_rsch][index_csch][0];

					rpix += weight[index_rsch][index_csch][0] * extimg[index_rori + neighbourwd / 2 + index_rsch][index_cori + neighbourwd / 2 + index_csch][0];
				}
			}
		          
			
			//给原图赋值
			pix[index_rori*imageHeight*BytesPerPixel + index_cori * BytesPerPixel]  = rpix/r_norm;

			
			rpix = 0;

			r_norm = 0;
			

			rtemp = 0;
		
		}
		cout << index_rori << "行完毕"<<endl;
	}
        
        double mse_r, psnr_r;
	int val_r;
	mse_r = 0.0;
		for (int i = 0; i<imageHeight; i++)
		{
			for (int j = 0; j<imageWidth; j++)
			{

				val_r = abs((int)pixrf[i*imageWidth*BytesPerPixel + j * BytesPerPixel] - (int)pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel]);
				mse_r += pow(val_r,2);


			}
		}


	mse_r = mse_r / (imageWidth*imageHeight);
	psnr_r = 10 * log10(255 * 255 / mse_r);

	cout << psnr_r << endl;
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	return 0;
}
