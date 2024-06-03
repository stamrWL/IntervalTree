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
	TreeNode* NextNode;	//Ҷ�ӽڵ��������
	TreeNode* LastNode;	//Ҷ�ӽڵ��������

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
	void Silce(double B);
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
	double getValue();
	double getValue(double point);
	TreeNode* getInterval(double point);
	double getRightArea();
	double getLeftArea();
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
protected:
	TreeNode* getInterval(double point);
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
	void extend(double rightB);
	void extend(double rightB, double value);
	void addValue(double LB, double RB, double value);
	double getValue(double point);
	void intoNextWindows();
	double AllocatedArea_DD(double startPoint, double targetArea);
	double AllocatedArea_DDD(double startPoint, double targetArea, double hight);
	std::string viewList();
};