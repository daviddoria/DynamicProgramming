#include "DynamicProgramming.h"

// STL
#include <cmath>
#include <stdexcept>

// Submodules
#include "Helpers/Helpers.h"

void DynamicProgramming::SetLabelSet(const std::vector<float>& labelSet)
{
  this->LabelSet = labelSet;
}

void DynamicProgramming::SetNumberOfNodes(const unsigned int numberOfNodes)
{
  this->NumberOfNodes = numberOfNodes;
}

float DynamicProgramming::BinaryEnergy(const float a, const float b)
{
  return fabs(a-b);
}

float DynamicProgramming::UnaryEnergy(const float a, const unsigned int position)
{
  // Don't care about the middle positions
  if(position > 0 || position == this->NumberOfNodes - 1)
  {
    return 0.0f;
  }

  // Want the first position to be close to 0
  if(position == 0)
  {
    return fabs(a-0);
  }

  // Want the last position to be close to NumberOfNodes
  if(position == NumberOfNodes - 1)
  {
    return fabs(a-NumberOfNodes);
  }

  throw std::runtime_error("Should never get here!");
}

std::vector<unsigned int> DynamicProgramming::Optimize()
{
  ComputeGrids();
  std::vector<Index> path = TracePath();

  std::vector<unsigned int> labelIds(path.size());
  for(unsigned int i = 0; i < path.size(); ++i)
  {
    labelIds[i] = path[i][1]; // Extract the label from the index (of (nodes,labels) grid)
  }

  return labelIds;
}

std::vector<DynamicProgramming::Index> DynamicProgramming::GetPossiblePredecessors(const Index& queryIndex)
{
  std::vector<Index> indexes;

  // Check if the three positions in the grid (up-left, up, and up-right) are inside the grid, and if they are, include them in
  // the possible predecessor set.
  for(int colOffset = -1; colOffset <= 1; ++colOffset)
  {
    Index potentialPossiblePredecessor;
    potentialPossiblePredecessor[0] = queryIndex[0] - 1;
    potentialPossiblePredecessor[1] = queryIndex[1] + colOffset;

    if(potentialPossiblePredecessor[0] < 0 || potentialPossiblePredecessor[0] >= this->NumberOfNodes ||
       potentialPossiblePredecessor[1] < 0 || potentialPossiblePredecessor[1] >= this->LabelSet.size())
    {
      continue;
    }
    indexes.push_back(potentialPossiblePredecessor);
  }
  return indexes;
}

void DynamicProgramming::ComputeGrids()
{
  this->CostGrid = Eigen::MatrixXf (this->NumberOfNodes, this->LabelSet.size());
  this->PredecessorGrid = IndexMatrixType(this->NumberOfNodes, this->LabelSet.size());

  // Base case: M_1(x_1) = E_u(x_1) // The cost of assigning node 1 a label named "x_1" (NOT the particular label with id=1)
  for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
    {
    this->CostGrid(0, label) = UnaryEnergy(this->LabelSet[label], 0);
    this->PredecessorGrid(0,label) = Index(-1,-1);
    }

  for(unsigned int node = 1; node < this->NumberOfNodes; ++node) // Start at 1 since 0 was done as the base case
  {
    for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
    {
      Index queryIndex;
      queryIndex[0] = node;
      queryIndex[1] = label;
      std::vector<Index> possiblePredecessors = GetPossiblePredecessors(queryIndex);
      std::vector<float> costs(possiblePredecessors.size());

      for(unsigned int i = 0; i < possiblePredecessors.size(); ++i)
      {
        costs[i] = UnaryEnergy(this->LabelSet[label], 0) + ;
      }
      unsigned int bestPredecessor = Helpers::argmin(costs);
      this->CostGrid(node, label) = costs[bestPredecessor];
      this->PredecessorGrid(node, label) = possiblePredecessors[bestPredecessor];
    }
  }
}

std::vector<Index> DynamicProgramming::TracePath()
{
  // Find min location in last row
  std::vector<float> costs(this->LabelSet.size());
  for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
  {
    costs[label] = this->CostGrid(this->NumberOfNodes - 1, label);
  }

  unsigned int startingLabel = Helpers::argmin(costs);

  //std::vector<Index> path(this->NumberOfNodes);
  std::vector<Index> path;

  Index currentIndex = this->PredecessorGrid(this->NumberOfNodes - 1, startingLabel);

  while(currentIndex[0] != -1)
  {
    path.push_back(currentIndex);
    Index currentIndex = this->PredecessorGrid(currentIndex[0], currentIndex[1]);
  }

  return path;
}
