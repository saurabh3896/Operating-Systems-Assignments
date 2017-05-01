#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

int main(){
  deque<string> v;                              //double-ended queue to store strings
  ifstream input_file;
  ofstream output_file;                         //fstream objects
  char filename[1024], filename_[1024];
  string get_line;                              //char buffers and string declarations
  cout << "Enter input filename : ";
  cin >> filename;
  cout << "Enter output filename : ";
  cin >> filename_;
  input_file.open(filename);                    //open file
  if(!input_file.is_open()){
    cout << "Invalid file." << endl;            //wrong file input
    return 0;
  }
  output_file.open(filename_);                  //open output file
  while(getline(input_file, get_line)){         //push strings, stored in stack-way, thus reversing
    v.push_front(get_line);
  }                                             //write the output to output file
  for(deque<string>::iterator it = v.begin();it != v.end();it++){
    output_file << *it << endl;
  }
  input_file.close();                           //close the files
  output_file.close();
  return 0;
};
