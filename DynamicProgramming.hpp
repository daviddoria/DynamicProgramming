#ifndef DynamicProgramming_hpp
#define DynamicProgramming_hpp

#include "DynamicProgramming.h"

// STL
#include <cmath>
#include <stdexcept>
#include <algorithm> // for reverse()

// Submodules
#include "Helpers/Helpers.h"

template <typename TLabel>
void DynamicProgramming<TLabel>::SetLabelSet(const std::vector<TLabel>& labelSet)
{
  this->LabelSet = labelSet;
}

template <typename TLabel>
void DynamicProgramming<TLabel>::SetNumberOfNodes(const unsigned int numberOfNodes)
{
  this->NumberOfNodes = numberOfNodes;
}

template <typename TLabel>
std::vector<unsigned int> DynamicProgramming<TLabel>::Optimize()
{
  std::cout << "Nodes: " << this->NumberOfNodes << " labels: " << this->LabelSet.size() << std::endl;

  ComputeGrids();
  std::vector<Index> path = TracePath();

  std::vector<unsigned int> labelIds(path.size());
  for(unsigned int i = 0; i < path.size(); ++i)
  {
    labelIds[i] = path[i][1]; // Extract the label from the index (of (nodes,labels) grid)
  }

  std::reverse (labelIds.begin( ), labelIds.end( ) );
  return labelIds;
}

template <typename TLabel>
void DynamicProgramming<TLabel>::ComputeGrids()
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
      std::vector<float> costs(this->LabelSet.size());

      for(unsigned int previousLabel = 0; previousLabel < costs.size(); ++previousLabel)
      {
        float binaryEnergy = BinaryEnergy(this->LabelSet[previousLabel], previousLabel,
                                          this->LabelSet[label], label);
        costs[previousLabel] =  binaryEnergy + this->CostGrid(node - 1, previousLabel);
      }
      unsigned int bestPredecessor = Helpers::argmin(costs);
      this->CostGrid(node, label) = UnaryEnergy(this->LabelSet[label], node) + costs[bestPredecessor];
      this->PredecessorGrid(node, label) = Index(node-1, bestPredecessor);
    }
  }
}

template <typename TLabel>
std::vector<Index> DynamicProgramming<TLabel>::TracePath()
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

  Index currentIndex(this->NumberOfNodes - 1, startingLabel);

  while(currentIndex[0] != -1)
  {
    path.push_back(currentIndex);
    currentIndex = this->PredecessorGrid(currentIndex[0], currentIndex[1]);
  }

  return path;
}

#endif
