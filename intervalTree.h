#pragma once
#include <iostream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <stdexcept>

class TreeNode {
private:
	TreeNode* leftNode;
	TreeNode* rightNode;
	
	double leftB;
	double rightB;
	double midB;
	double value;
protected:
	TreeNode* NextNode;	
	TreeNode* LastNode;	

public:

	TreeNode(double leftB, double rightB, double value, TreeNode* NextNode = NULL, TreeNode* LastNode = NULL) {
		this->leftB = leftB;
		this->rightB = rightB;
		this->value = value;
		this->midB = -1;
		this->leftNode = this->rightNode = NULL;

		this->NextNode = NextNode;
		this->LastNode = LastNode;
		if(this->LastNode != NULL)
			this->LastNode->setNextNode(this);
		if (this->NextNode != NULL)
			this->NextNode->setLastNode(this);
	}
	TreeNode(TreeNode* leftNode, double rightB, double value, TreeNode* NextNode = NULL, TreeNode * LastNode = NULL) {
		this->leftB = leftNode->getLeftB();
		this->rightB = rightB;
		this->value = value < leftNode->getValue()?value:leftNode->getValue();
		this->midB = leftNode->getRightB();
		this->leftNode = leftNode;
		this->rightNode = new TreeNode(midB, rightB, value, NextNode, LastNode);

	}
	~TreeNode() {
		if (leftNode == NULL && rightNode == NULL) {
			if (NextNode != NULL)
				NextNode->setLastNode(LastNode);
			if (LastNode != NULL)
				LastNode->setNextNode(NextNode);
		}
		if (this->leftNode != NULL)
			delete this->leftNode;
		if (this->rightNode != NULL)
			delete this->rightNode;
	}
	void Slice(double B);
	void addValue(double LB, double RB, double value);
	void setNextNode(TreeNode* NextList) { 
		this->NextNode = NextList;
		//NextList->setLastNode(this);
	}
	void setLastNode(TreeNode* LastNode) { 
		this->LastNode = LastNode;
		//LastNode->setNextNode(this);
	}
	double getLeftB();
	double getRightB();
	double getValue() const;
	double getValue(double point) const;
	double getRightValue(double point) const;
	TreeNode* getInterval(double point) ;
	double getRightArea() const;
	double getLeftArea() const;
	TreeNode* getNextNode();
	TreeNode* getLastNode();
	TreeNode* releaseLeft(double B);
};

class IntervalTree {
private:
	TreeNode* root;
	TreeNode* List;
	double lastLeftB;
	double windows;
	double defualtValue;
	// 
protected:
	TreeNode* getInterval(double point);
	void Slice(double B);
	void extend(double rightB);
	void extend(double rightB, double value);
public:
	IntervalTree(double defualtValue, double lastLeftB = 0, double windows = 30) {
		this->root = new TreeNode(lastLeftB, windows, defualtValue);
		this->List = new TreeNode(-1, -1, -1);
		this->root->setNextNode(this->List);
		this->root->setLastNode(this->List);
		this->List->setNextNode(this->root);
		this->List->setLastNode(this->root);
		this->windows = windows;
		this->lastLeftB = lastLeftB;
		this->defualtValue = defualtValue;
	}
	void addValue(double LB, double RB, double value);
	double getValue(double point) const;
	void releaseLeft(double B);
	void intoNextWindows();
	void intoNextWindows(double x);
	void changeDefualtValue(double now,double Value);
	double AllocatedArea_DD(double startPoint, double targetArea);
	double AllocatedArea_DDD(double startPoint, double targetArea, double hight);
	std::string viewList()const;
};