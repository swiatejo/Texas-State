#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstrea
#include "stdlib.h"
#include "math.h"


using namespace std;


void getImage(int image[][512], int x, int y, int width, int height, int threshold);


int main(int argc, char* argv[]) {

  

  if(argc < 2)
  {
    cout << "Please enter a threshold value. 0-255. " << endl;
    return 0;
  }
    int threshold;
    istringstream(argv[1]) >> threshold;

  
  int row = 0, col = 0, numrows = 0, numcols = 0;
  ifstream infile("baboon.pgma");
  ofstream outfile("newbaboon.pgma");
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

  int picture[512][512];


  // Following lines : data
  for(row = 0; row < numrows; ++row)
    for (col = 0; col < numcols; ++col) 
	ss >>  picture[row][col];
	
  
    
    
  getImage(picture, 0, 0, 512, 512, threshold);
   
   
  int max = 0;
  for(row = 0; row < numrows; ++row)
    for(col = 0; col < numcols; ++col)
       if(max < picture[row][col])
       {
	  max = picture[row][col];
       }

  if(outfile.is_open())
  {
    outfile << "P2" << endl;
    outfile << "#compressed photo of baboon" << endl;
    outfile << numcols << "  " << numrows << endl;
    outfile << max << endl;
     for(row = 0; row < numrows; ++row) {
       for(col = 0; col < numcols; ++col) {
         outfile << picture[row][col] << "  ";
    }
    //cout << endl;
  }
  infile.close();
  outfile.close();
}
}



void getImage(int image[][512], int x, int y, int width, int height,  int threshold)
{
	
	if(width == 1 || height == 1) { return;}

	//Quadrant 1

        double sum1, avg1, var1, dif1, actvar1 = 0.0;
	for(int r = y; r < height/2; ++r){
	  for(int c = x; c < width/2; ++c){
	     sum1 += image[r][c];
             //cout << "Sum: " << sum << endl;
          }
        }  
        avg1 = sum1 / ((width/2) * (height/2));
        //cout << "Average: " << avg << endl;
        
	for(int r = y; r < height/2; ++r){
          for(int c = x; c < width/2; ++c){
              dif1 += ((image[r][c] - avg1) * (image[r][c] - avg1));
	      //cout << "Difference: " << dif << endl;
          }
        }
        var1 = dif1 / (width/2) * (height/2);
        actvar1 = sqrt(var1);
        cout << "Variance: " << actvar1 << endl; 
	cout << "Average: " << avg1 << endl;

	if(actvar1 <= threshold)
	{
	  for(int r = y; r < height/2; ++r){
            for(int c = x; c < width/2; ++c){
               image[r][c] = (int)avg1;
	    }
          }
        }
	else
	   getImage(image, x, y, width/2, height/2, threshold);

	//Quadrant 2

        double sum2, avg2, var2, dif2, actvar2 = 0.0;
        for(int r = y; r < height/2; ++r){
          for(int c = x + width/2; c < width; ++c){
             sum2 += image[r][c];
             //cout << "Sum: " << sum << endl;
          }
        }
        avg2 = sum2 / ((width/2) * (height/2));
        //cout << "Average: " << avg << endl;

        for(int r = y; r < height/2; ++r){
          for(int c = x + width/2; c < width; ++c){
              dif2 += ((image[r][c] - avg2) * (image[r][c] - avg2));
              //cout << "Difference: " << dif << endl;
          }
        }
        var2 = dif2 / ((width/2) * (height/2)) ;
        actvar2 = sqrt(var2);
        cout << "Variance: " << actvar2 << endl;
        cout << "Average: " << avg2 << endl;

        if(actvar2 <= threshold)
        {
          for(int r = y; r < height/2; ++r){
            for(int c = x + width/2; c < width; ++c){
               image[r][c] = (int)avg2;
            }
          }
        }
        else
           getImage(image, x + width/2, y,  width/2, height/2, threshold);

	//Quadrant 3

        double sum3, avg3, var3, dif3, actvar3 = 0.0;
        for(int r = y + height/2; r < height; ++r){
          for(int c = x; c < width/2; ++c){
             sum3 += image[r][c];
             //cout << "Sum: " << sum << endl;
          }
        }
        avg3 = sum3 / ((width/2) * (height/2));
        //cout << "Average: " << avg << endl;

        for(int r = y + height/2; r < height; ++r){
          for(int c = x; c < width/2; ++c){
              dif3 += ((image[r][c] - avg3) * (image[r][c] - avg3));
              //cout << "Difference: " << dif << endl;
          }
        }
        var3 = dif3 / ((width/2) * (height/2)) ;
        actvar3 = sqrt(var3);
        cout << "Variance: " << actvar3 << endl;
        cout << "Average: " << avg3 << endl;

        if(actvar3 <= threshold)
        {
          for(int r = y + height/2; r < height; ++r){
            for(int c = x; c < width/2; ++c){
               image[r][c] = (int)avg3;
            }
          }
        }
        else
           getImage(image, x, y + height/2, width/2, height/2, threshold);

	//Quadrant 4

        double sum4, avg4, var4, dif4, actvar4 = 0.0;
        for(int r = y + height/2; r < height; ++r){
          for(int c = x + width/2; c < width; ++c){
             sum4 += image[r][c];
             //cout << "Sum: " << sum << endl;
          }
        }
        avg4 = sum4 / ((width/2) * (height/2));
        //cout << "Average: " << avg << endl;

        for(int r = y + height/2; r < height; ++r){
          for(int c = x + width/2; c < width; ++c){
              dif4 += ((image[r][c] - avg4) * (image[r][c] - avg4));
              //cout << "Difference: " << dif << endl;
          }
        }
        var4 = dif4 / ((width/2) * (height/2)) ;
        actvar4 = sqrt(var4);
        cout << "Variance: " << actvar4 << endl;
        cout << "Average: " << avg4 << endl;

        if(actvar4 <= threshold)
        {
          for(int r = y + height/2; r < height; ++r){
            for(int c = x + width/2; c < width; ++c){
               image[r][c] = (int)avg4;
            }
          }
        }
        else
           getImage(image, x + width/2, y + height/2, width/2, height/2, threshold);
    
}

    
