//Josh Swiatek

#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstrea
#include "stdlib.h"
#include "math.h"


using namespace std;


void getImageTwoLevels(int img[][512], int img_return[][512]);
void getImageTwelveLevels(int img[][512], int img_return[][512]);
void getErrorImage(int img[][512], int img_return[][512], int error_return[][512]);
double getDistortion(int org_img[][512], int new_img[][512]);

int main(int argc, char* argv[]) {

  
  int row, col, numcols, numrows = 0;  
  ifstream infile("baboon.pgma");
  ofstream outfile12("newbaboon12level.pgma");
  ofstream outfile2("newbaboon2level.pgma");
  ofstream errorfile2("errorbaboon2.pgma");
  ofstream errorfile12("errorbaboon12.pgma");
  stringstream ss;
  string inputLine = "";

  // First line : version
  getline(infile,inputLine);
  if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
  else cout << "Version : " << inputLine << endl;

  // Second line : comment
  getline(infile,inputLine);
  cout << "Comment : " << inputLine << endl;

  // Continue with a stringstream
  ss << infile.rdbuf();
  // Third line : size
  ss >> numcols >> numrows;
 // cout << numcols << " columns and " << numrows << " rows" << endl;
  

  //2-D Arrays for incoming image and new images
  int picture[512][512];
  int recon12_picture[512][512];
  int recon2_picture[512][512];
  int error_picture[512][512];
  int error12_picture[512][512];

  // Following lines : data
  for(row = 0; row < numrows; ++row)
    for (col = 0; col < numcols; ++col) 
	ss >>  picture[row][col];
	

  //Generate new images from original 
  getImageTwelveLevels(picture, recon12_picture);
  getImageTwoLevels(picture, recon2_picture); 
  getErrorImage(picture, recon2_picture, error_picture); 
  getErrorImage(picture, recon12_picture, error12_picture);

  cout << "Distortion - 2 Gray Levels: " << getDistortion(picture, recon2_picture) << endl;
  cout << "Distortion - 12 Gray Levels: " << getDistortion(picture, recon12_picture) << endl;


 //Create Twelve Gray Levels Baboon Image
  int max = 0;
  for(row = 0; row < numrows; ++row)
    for(col = 0; col < numcols; ++col)
       if(max < recon12_picture[row][col])
       {
	  max = recon12_picture[row][col];
       }

  if(outfile12.is_open())
  {
    outfile12 << "P2" << endl;
    outfile12 << "#compressed photo of baboon" << endl;
    outfile12 << numcols << "  " << numrows << endl;
    outfile12 << max << endl;
     for(row = 0; row < numrows; ++row) {
       for(col = 0; col < numcols; ++col) {
         outfile12 << recon12_picture[row][col] << "  ";
    }
    
  }
  outfile12.close();
}


//Create Two Gray Levels Baboon Image
int max2 = 0;
  for(row = 0; row < numrows; ++row)
    for(col = 0; col < numcols; ++col)
       if(max2 < recon2_picture[row][col])
       {
          max2 = recon2_picture[row][col];
       }

  if(outfile2.is_open())
  {
    outfile2 << "P2" << endl;
    outfile2 << "#compressed photo of baboon" << endl;
    outfile2 << numcols << "  " << numrows << endl;
    outfile2 << max2 << endl;
     for(row = 0; row < numrows; ++row) {
       for(col = 0; col < numcols; ++col) {
         outfile2 << recon2_picture[row][col] << "  ";
    }
    
  }
  
  outfile2.close();
}


//Create Error Image for Two Level Baboon Image
int max3 = 0;
  for(row = 0; row < numrows; ++row)
    for(col = 0; col < numcols; ++col)
       if(max3 < error_picture[row][col])
       {
          max3 = error_picture[row][col];
       }

  if(errorfile2.is_open())
  {
    errorfile2 << "P2" << endl;
    errorfile2 << "#compressed photo of baboon" << endl;
    errorfile2 << numcols << "  " << numrows << endl;
    errorfile2 << max3 << endl;
     for(row = 0; row < numrows; ++row) {
       for(col = 0; col < numcols; ++col) {
         errorfile2 << error_picture[row][col] << "  ";
    }
    
  }
  
  errorfile2.close();
}


//Create Error Image for Twelve Level Baboon Image
int max4 = 0;
  for(row = 0; row < numrows; ++row)
    for(col = 0; col < numcols; ++col)
       if(max4 < error12_picture[row][col])
       {
          max4 = error12_picture[row][col];
       }

  if(errorfile12.is_open())
  {
    errorfile12 << "P2" << endl;
    errorfile12 << "#compressed photo of baboon" << endl;
    errorfile12 << numcols << "  " << numrows << endl;
    errorfile12 << max4 << endl;
     for(row = 0; row < numrows; ++row) {
       for(col = 0; col < numcols; ++col) {
         errorfile12 << error12_picture[row][col] << "  ";
    }
    
  }
  infile.close();
  errorfile12.close();
}


}


double getDistortion(int org_img[][512], int new_img[][512]) 
{
	double temp = 0.0;
        double distortion = 0.0;
	for(int row = 0; row < 512; row++) {
		for(int col = 0; col < 512; col++) {
			temp += (org_img[row][col] - new_img[row][col]) * (org_img[row][col] - new_img[row][col]);
		}
	}
	distortion = temp / (512 * 512);
	return distortion;

}

//Creates error image based on pixel difference between two different images
void getErrorImage(int img[][512], int img_return[][512], int error_return[][512])
{
	for(int row = 0; row < 512; row++) {
		for(int col = 0; col < 512; col++) {
			int temp = img[row][col] - img_return[row][col];
			if (temp >= 0) {
				error_return[row][col] = temp;
			}
			else {
				error_return[row][col] = -temp;
			}
		}
	}
			 
}


//Creates new Two Gray Level Image based on quantization
void getImageTwoLevels(int img[][512], int img_return[][512])
{
     int temp_image[512][512];

     for(int row = 0; row < 512; row++) {
        for(int col = 0; col < 512; col++) {
            if(img[row][col] <= 128) {
                temp_image[row][col] = 1;
            } else if(img[row][col] > 128 && img[row][col] <= 255) {
                temp_image[row][col] = 2;
            }
	}
     }

     for(int row = 0; row < 512; row++) {
        for(int col = 0; col < 512; col++) {
           if(temp_image[row][col] == 1){
                img_return[row][col] = 64;
           } else if(temp_image[row][col] == 2) {
                img_return[row][col] = 192;
	   }
	}
     }	
}


//Creates new Twelve Gray Level Image based on quantization
void getImageTwelveLevels(int img[][512], int img_return[][512])
{
     int temp_image[512][512];

     for(int row = 0; row < 512; row++) {
	for(int col = 0; col < 512; col++) {
	    if(img[row][col] <= 21) {
		temp_image[row][col] = 1;
	    } else if(img[row][col] > 21 && img[row][col] <= 42) {
		temp_image[row][col] = 2;
	    } else if(img[row][col] > 42 && img[row][col] <= 64) {
		temp_image[row][col] = 3;
	    } else if(img[row][col] > 64 && img[row][col] <= 85) {
    		temp_image[row][col] = 4;
	    } else if(img[row][col] > 85 && img[row][col] <= 106) {
		temp_image[row][col] = 5;
     	    } else if(img[row][col] > 106 && img[row][col] <= 127) {
		temp_image[row][col] = 6;
	    } else if(img[row][col] > 127 && img[row][col] <= 150) {
		temp_image[row][col] = 7;
	    } else if(img[row][col] > 150 && img[row][col] <= 170) {
		temp_image[row][col] = 8;
	    } else if(img[row][col] > 170 && img[row][col] <= 191) {
		temp_image[row][col] = 9;
	    } else if(img[row][col] > 191 && img[row][col] <= 212) {
		temp_image[row][col] = 10;
	    } else if(img[row][col] > 212 && img[row][col] <= 234) {
		temp_image[row][col] = 11;
	    } else if(img[row][col] > 234 && img[row][col] <= 255) {
		temp_image[row][col] = 12;
	    }
	}
     }

     for(int row = 0; row < 512; row++) {
	for(int col = 0; col < 512; col++) {
	   if(temp_image[row][col] == 1){
		img_return[row][col] = 11;
	   } else if(temp_image[row][col] == 2) {
		img_return[row][col] = 32;
	   } else if(temp_image[row][col] == 3) {
		img_return[row][col] = 53;
	   } else if(temp_image[row][col] == 4) {
		img_return[row][col] = 74;
	   } else if(temp_image[row][col] == 5) {
		img_return[row][col] = 96;
	   } else if(temp_image[row][col] == 6) {
		img_return[row][col] = 117;
	   } else if(temp_image[row][col] == 7) {
		img_return[row][col] = 139;
	   } else if(temp_image[row][col] == 8) {
		img_return[row][col] = 160;
	   } else if(temp_image[row][col] == 9) {
		img_return[row][col] = 181;
	   } else if(temp_image[row][col] == 10) {
		img_return[row][col] = 202;
	   } else if(temp_image[row][col] == 11) {
		img_return[row][col] = 223;
	   } else if(temp_image[row][col] == 12) {
		img_return[row][col] = 244;
	   }
	}
    }

    /* for(int row = 0; row < 512; row++) {
	for(int col = 0; col < 512; col++) {
		cout << temp_image[row][col];
	}
	cout << endl;
     }*/

}
    
