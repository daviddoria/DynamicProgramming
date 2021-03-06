#ifndef DynamicProgramming_hpp
#define DynamicProgramming_hpp

#include "DynamicProgramming.h"

// STL
#include <cmath>
#include <stdexcept>
#include <algorithm> // for reverse()

// Submodules
#include "Helpers/Helpers.h"

template <typename TLabel, typename TNode>
void DynamicProgramming<TLabel, TNode>::SetLabelSet(const std::vector<TLabel>& labelSet)
{
  this->LabelSet = labelSet;
}

template <typename TLabel, typename TNode>
void DynamicProgramming<TLabel, TNode>::SetNodes(const std::vector<TNode>& nodes)
{
  this->Nodes = nodes;
}

template <typename TLabel, typename TNode>
std::vector<unsigned int> DynamicProgramming<TLabel, TNode>::Optimize()
{
  std::cout << "Nodes: " << this->Nodes.size() << " labels: " << this->LabelSet.size() << std::endl;

  if(this->Nodes.size() == 0)
  {
    throw std::runtime_error("DynamicProgramming: There are 0 nodes!");
  }
  if(this->LabelSet.size() == 0)
  {
    throw std::runtime_error("DynamicProgramming: There are 0 labels!");
  }

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

template <typename TLabel, typename TNode>
void DynamicProgramming<TLabel, TNode>::ComputeGrids()
{
  this->CostGrid = Eigen::MatrixXf (this->Nodes.size(), this->LabelSet.size());
  this->PredecessorGrid = IndexMatrixType(this->Nodes.size(), this->LabelSet.size());

  // Base case: M_1(x_1) = E_u(x_1) // The cost of assigning node 1 a label named "x_1" (NOT the particular label with id=1)
  for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
    {
    this->CostGrid(0, label) = UnaryEnergy(this->LabelSet[label], this->Nodes[0]);
    this->PredecessorGrid(0,label) = Index(-1,-1); // indicates an invalid predecessor
    }

  for(unsigned int node = 1; node < this->Nodes.size(); ++node) // Start at 1 since 0 was done as the base case
  {
    std::cout << "Computing grid row " << node << " (out of " << this->Nodes.size() << ")" << std::endl;
    for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
    {
      std::vector<float> costs(this->LabelSet.size());

      for(unsigned int previousLabel = 0; previousLabel < costs.size(); ++previousLabel)
      {
        float binaryEnergy = BinaryEnergy(this->LabelSet[previousLabel], this->Nodes[node - 1],
                                          this->LabelSet[label], this->Nodes[node]);
        costs[previousLabel] =  binaryEnergy + this->CostGrid(node - 1, previousLabel);
      }
      unsigned int bestPredecessor = Helpers::argmin(costs);
      this->CostGrid(node, label) = UnaryEnergy(this->LabelSet[label], this->Nodes[node]) + costs[bestPredecessor];
      this->PredecessorGrid(node, label) = Index(node-1, bestPredecessor);
    }
  }
}

template <typename TLabel, typename TNode>
std::vector<Index> DynamicProgramming<TLabel, TNode>::TracePath()
{
  // Find min location in last row
  std::vector<float> costs(this->LabelSet.size());
  for(unsigned int label = 0; label < this->LabelSet.size(); ++label)
  {
    costs[label] = this->CostGrid(this->Nodes.size() - 1, label);
  }

  unsigned int startingLabel = Helpers::argmin(costs);

  //std::vector<Index> path(this->Nodes.size());
  std::vector<Index> path;

  Index currentIndex(this->Nodes.size() - 1, startingLabel);

  while(currentIndex[0] != -1)
  {
    path.push_back(currentIndex);
    currentIndex = this->PredecessorGrid(currentIndex[0], currentIndex[1]);
  }

  return path;
}

#endif
