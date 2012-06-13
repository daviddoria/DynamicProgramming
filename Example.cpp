#include <iostream>
#include <vector>

#include "DynamicProgramming.h"

typedef float LabelType;

class DemoDynamicProgramming : public DynamicProgramming<LabelType>
{
private:
  float BinaryEnergy(const LabelType& labelA, const unsigned int nodeA,
                     const LabelType& labelB, const unsigned int nodeB)
  {
    if(labelA != labelB)
    {
      return fabs(labelA - labelB);
    }
    else
    {
      return 10.0f;
    }
  }

  float UnaryEnergy(const LabelType& label, const unsigned int node)
  {
    // Don't care about the middle positions
    if(node > 0 || node == this->NumberOfNodes - 1)
    {
      return 0.0f;
    }

    // Want the first position to be close to 0
    if(node == 0)
    {
      return fabs(label - 0);
    }

    // Want the last position to be close to NumberOfNodes
    if(node == NumberOfNodes - 1)
    {
      return fabs(label - NumberOfNodes);
    }

    throw std::runtime_error("Should never get here!");
  }
};

int main(int argc, char *argv[])
{

  std::vector<LabelType> labelSet = {4, 3, 5, 6, 9, 1, 2, 8 , 7};

  DemoDynamicProgramming dp;
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
