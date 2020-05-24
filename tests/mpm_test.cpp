#include "pitchtracker.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

std::vector<double> get_sample(std::string file){
  std::vector<double> data;
  std::ifstream infile(file);

  if(infile.fail()){
    std::cerr << "file " << file << "doesn't exist!\n";
    exit(1);
  }

  double val;
  while (infile >> val)
    data.push_back(val); 
  return data;
}

int main(){
  std::vector<double> sample_info = get_sample("/home/dave/projects/pitch-tracker/tests/A4_44100_violin.txt");
  std::cout << sample_info.size() << std::endl;
  double estimation = mpm(sample_info,44100);
  std::cout << "expected: 440.0" << "\nestimation: " << estimation << std::endl;
  return 0;
}
