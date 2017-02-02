#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
using namespace std;


void getImage(int image[][512], int size, int threshold);



int main() {
  int row = 0, col = 0, numrows = 0, numcols = 0, gray_limit = 0;
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
  // Fourth line : Gray limit
  ss >> gray_limit;
 // cout << numcols << " columns and " << numrows << " rows" << endl;

  int picture[512][512];

  // Following lines : data
  for(row = 0; row < numrows; ++row)
    for (col = 0; col < numcols; ++col) 
	ss >>  picture[row][col];

  int threshold = 16;
  
   
  getImage(picture, numcols, threshold);


  if(outfile.is_open())
  {
    outfile << "P2" << endl;
    outfile << "#compressed photo of baboon" << endl;
    outfile << numcols << "  " << numrows << endl;
    outfile << gray_limit << endl;
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



void getImage(int image[][512], int size, int threshold)
{
	
	if(size == 1) { return;}

        int sum, avg, var, dif = 0;
	for(int r = 0; r < size/2; ++r){
	  for(int c = 0; c < size/2; ++c){
	     sum += image[r][c];
          }
        }
	avg = sum / (size * size);
        
	for(int r = 0; r < size/2; ++r){
          for(int c = 0; c < size/2; ++c){
              dif += ((image[r][c] - avg)) * ((image[r][c] - avg));
          }
        }
        var = dif / (size * size);

	if(var <= threshold)
	{
	  for(int r = 0; r < size/2; ++r){
            for(int c = 0; c < size/2; ++c){
               image[r][c] = avg;
	    }
          }
        }
	else
	   getImage(image, size/2, threshold);
    
}
