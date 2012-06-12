#include <iostream>
#include <vector>

#include "DynamicProgramming.h"

int main(int argc, char *argv[])
{
  std::vector<float> labelSet = {4, 3, 5, 6, 9, 1, 2, 8 , 7};

  DynamicProgramming dp;
  dp.SetLabelSet(labelSet);
  dp.SetNumberOfNodes(3);
  std::vector<unsigned int> solution = dp.Optimize();

  for(unsigned int i = 0; i < solution.size(); ++i)
  {
    std::cout << labelSet[solution[i]] << " ";
  }
  std::cout << std::endl;

  return 0;
}
