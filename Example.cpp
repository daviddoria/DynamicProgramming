#include <iostream>
#include <vector>

#include "DynamicProgramming.h"

typedef float LabelType;
typedef unsigned int NodeType;

class DemoDynamicProgramming : public DynamicProgramming<LabelType, NodeType>
{
private:
  float BinaryEnergy(const LabelType& labelA, const NodeType& nodeA,
                     const LabelType& labelB, const NodeType& nodeB)
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

  float UnaryEnergy(const LabelType& label, const NodeType& node)
  {
    // Don't care about the middle positions
    if(node > 0 || node == this->Nodes.size() - 1)
    {
      return 0.0f;
    }

    // Want the first position to be close to 0
    if(node == 0)
    {
      return fabs(label - 0);
    }

    // Want the last position to be close to this->Nodes.size()
    if(node == this->Nodes.size() - 1)
    {
      return fabs(label - this->Nodes.size());
    }

    throw std::runtime_error("Should never get here!");
  }
};

int main(int argc, char *argv[])
{

  std::vector<LabelType> labelSet = {4, 3, 5, 6, 9, 1, 2, 8 , 7};

  DemoDynamicProgramming dp;
  dp.SetLabelSet(labelSet);
  std::vector<NodeType> nodes = {{0,1,2}};
  dp.SetNodes(nodes);
  std::vector<unsigned int> solution = dp.Optimize();

  for(unsigned int i = 0; i < solution.size(); ++i)
  {
    std::cout << labelSet[solution[i]] << " ";
  }
  std::cout << std::endl;

  return 0;
}
