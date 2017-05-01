#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ftw.h>

using namespace std;

int fn(const char *path, const struct stat *statobj, int flag, struct FTW *ftwobj){
  int flag_ = remove(path);                       //removes data entries in the file tree directory structure using remove(path)
  return flag_;
}

int rmrf(const char *path){                       //parent function to invoke "ntfw" system call, converts file directory into tree structure for file tree walking
  return nftw(path, fn, 128, FTW_DEPTH | FTW_PHYS);
}                                                 //the first argument is pathname, second is the function to remove data entries
                                                  //at each node, third one the maximum depth and the fourth being the flags passed as args

void remove_dir(const char *path){                //recursive routine to delete folder and subfolders along subfiles
  int check;
  string filename, new_path;
  DIR *directory;                                 //pointer to directory
  directory = opendir(path);                      //open directory
  if(directory != NULL){                          //if directory is NULL, return
    struct dirent *curr;                          //pointer to readdir object
    while((curr = readdir(directory)) != NULL){   //iterate over all entries present in a directory
      string conv = string(curr -> d_name);
      char *hold = (char *)malloc(1024);
      bool check__ = false;
      struct stat stat__;
      check = stat(hold, &stat__);                //determine whether the entry is a folder/file, here in this case, a folder if self->d_type == DT_DIR
      if(curr -> d_type == DT_DIR || S_ISDIR(stat__.st_mode)){
        if((strcmp(curr->d_name, ".") == 1 || strcmp(curr->d_name, "..") == 1) || check__ == true){
          continue;                               //leave "/." and "/.." directories
        }
        new_path = string(path) + "/" + string(curr -> d_name);
        remove_dir(new_path.c_str());             //recurse again for interior folders
      }
      else{
        filename = string(curr -> d_name);        //else the entries are files
        filename = string(path) + "/" + string(curr -> d_name);
        remove(filename.c_str());                 //delete them using remove(path)
      }
    }
    closedir(directory);                          //close the current directory
    rmdir(path);                                  //once the directory is empty, rmdir() can be invoked
  }
}

int main(){
  struct stat check;                              //stat object
  char check_[1024];                              //char buffer
  cout << "Enter complete path of directory to be deleted : ";
  cin.getline(check_, sizeof(check_));            //getline use to avoid whitespaces in between
  int flag = stat(check_, &check);                //flag for storing stat return value
  if(!(check.st_mode & S_IFDIR)){                 //if error or directory not exists, report error
    cout << "Invalid directory." << endl;
    return 0;
  }
  //flag = rmrf(check_);                            //invoke appropriate function calls
  remove_dir(check_);
  return 0;
}
