#ifndef __INTERVAL_TREE_H__
#define __INTERVAL_TREE_H__
#include "linear/dynamicArray/dynamicArray.h"
#include "../functions/max.h"
#include <iostream>

class intervalTree{
    private:
    struct node{
        node* leftNode;
        node* rightNode;
        int64_t leftBound;
        int64_t rightBound;
        int64_t localMax;
        node(int64_t localMax, int64_t leftBound, int64_t rightBound, node* leftChild = nullptr, node* rightChild = nullptr) : localMax(localMax), leftNode(leftChild), rightNode(rightChild), leftBound(leftBound), rightBound(rightBound){}
        ~node(){
            delete leftNode;
            delete rightNode;
        }

        bool is_leaf(){
            return leftBound == rightBound;
        }
    };

    node* root;

    node* build(const dynamicArray<int64_t>& values, int64_t left, int64_t right){
        if(left==right){
            return new node(values[left], left, right);
        }
        int64_t middle = (left+right)/2;
        node* current = new node(0, left, right);
        current->leftNode = build(values, left, middle);
        current->rightNode = build(values, middle+1, right);
        current->localMax = max(current->leftNode->localMax, current->rightNode->localMax);
        return current;
    }

    int64_t findMaxBetween(node* current, int64_t left, int64_t right){
        if(current->rightBound < left || current->leftBound > right) return -1;
        if(left <= current->leftBound && current->rightBound <= right) return current->localMax;
        return max(findMaxBetween(current->leftNode, left, right), findMaxBetween(current->rightNode, left, right));
    }

	public:
    intervalTree(const dynamicArray<int64_t>& guardians){
        if(guardians.isEmpty()){
            root = nullptr;
            return;
        }
        root = build(guardians, 0, guardians.getLastIndex());
    }
    ~intervalTree(){delete root;}

    int64_t findMaxBetween(int64_t left, int64_t right){
        if(root==nullptr)return -1;
        if(right < left){
            int64_t temp = left;
            left = right;
            right = temp;
        }
        return findMaxBetween(root, left, right);
    }
};
#endif