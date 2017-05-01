#include <iostream>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main(){
  size_t file_size;                                 //size_t variable
  struct stat buffer;                               //stat struct object to store file details
  int fp = 0;
  char input_name[1024];                            //char buffer to store input file name
  cout << "Enter file name : ";
  cin >> input_name;
  int get = stat(input_name, &buffer);              //invoke stat function to get details about the file
  if(get != 0){                                     //error if the stat function not returns 0 (-1 in case of error)
    cout << "Invalid file." << endl;
    return 0;
  }
  fp = open(input_name, O_RDWR);                    //integer based file-descriptor for lseek input
  file_size = lseek(fp, 0, SEEK_END);               //calculate file size having fd as descriptor, 0 as offset bytes, and seeking from end
  cout << "File size using \"lseek\" : " << file_size << endl;
  cout << "Blocks using \"lseek\" : " << ceil((size_t)file_size/512.0) << endl;
  file_size = buffer.st_size;                       //block size calculation in both cases (ceil(size/512.0) and st_blocks)  
  cout << "File size using \"stat\" : " << file_size << endl;
  cout << "Blocks using \"stat\" : " << buffer.st_blocks << endl;
  return 0;
}
