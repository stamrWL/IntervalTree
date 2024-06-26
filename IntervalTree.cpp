﻿
#include "intervalTree.h"

void TreeNode::Delete() {
	if (NextNode != nullptr)
		NextNode->setLastNode(LastNode);
	if (LastNode != nullptr)
		LastNode->setNextNode(NextNode);
}

void TreeNode::Slice(double B)
{
	if (B <= leftB || B >= rightB)
	{
		return;
	}
	if (leftNode == nullptr && rightNode == nullptr)
	{
		std::shared_ptr<TreeNode> NNode = NextNode; // 叶子节点组成链表
		std::shared_ptr<TreeNode> LNode = LastNode; // 叶子节点组成链表
		LastNode = NextNode = nullptr;
		this->midB = B;

		this->leftNode = TreeNode::Create(leftB, midB, value);
		this->rightNode = TreeNode::Create(midB, rightB, value);
		this->leftNode->setLastNode(LNode);
		this->leftNode->setNextNode(this->rightNode);
		this->rightNode->setLastNode(this->leftNode);
		this->rightNode->setNextNode(NNode);
		LNode->setNextNode(leftNode);
		NNode->setLastNode(rightNode);
	}
	else
	{
		if (B <= midB)
		{
			this->leftNode->Slice(B);
		}
		else
		{
			this->rightNode->Slice(B);
		}
	}
}

void TreeNode::addValue(double LB, double RB, double AddValue)
{
	if (LB >= rightB || RB < leftB)
	{
		throw std::exception("outrange");
	}
	if (LB == leftB && RB == rightB)
	{
		this->value += AddValue;
		if (this->value < -0.001)
		{
			std::cout << "value < 0" << std::endl;
			throw std::exception("value < 0");
		}
		if (this->rightNode != nullptr)
			this->rightNode->addValue(midB, RB, AddValue);
		if (this->leftNode != nullptr)
			this->leftNode->addValue(LB, midB, AddValue);
		return;
	}
	else
	{
		if (leftNode == nullptr || rightNode == nullptr)
		{
			double MB = abs(2 * LB - leftB - rightB) > abs(2 * RB - leftB - rightB) ? RB : LB;
			this->Slice(MB);
		}
		if (RB <= midB)
		{
			this->leftNode->addValue(LB, RB, AddValue);
		}
		else if (LB >= midB)
		{
			this->rightNode->addValue(LB, RB, AddValue);
		}
		else
		{
			this->leftNode->addValue(LB, midB, AddValue);
			this->rightNode->addValue(midB, RB, AddValue);
		}
		double Lvalue = leftNode->getValue();
		double Rvalue = rightNode->getValue();
		value = Lvalue > Rvalue ? Rvalue : Lvalue;
	}
}

double TreeNode::getLeftB()
{
	return leftB;
}

double TreeNode::getRightB()
{
	return rightB;
}

double TreeNode::getValue() const
{
	return value;
}

double TreeNode::getValue(double point) const
{
	if (point < leftB || point > rightB)
	{
		return -1;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return getValue();
	}
	else
	{
		if (point < midB)
			return leftNode->getValue(point);
		else
			return rightNode->getValue(point);
	}
}


double TreeNode::getRightValue(double point) const {
	// 获取该点往右的最小值
	if (point < leftB || point > rightB)
	{
		return -1;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return getValue();
	}
	else
	{
		if (point < midB)
		{
			double Lvalue = leftNode->getValue(point);
			double Rvalue = rightNode->getValue();
			return Lvalue > Rvalue ? Rvalue : Lvalue;
		}
		else
			return rightNode->getRightValue(point);
	}


}

std::shared_ptr<TreeNode> TreeNode::getInterval(double point)
{
	if (point < leftB || point > rightB)
	{
		return nullptr;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return shared_from_this();
	}
	else
	{
		if (point <= midB)
			return leftNode->getInterval(point);
		else
			return rightNode->getInterval(point);
	}
}

double TreeNode::getRightArea() const
{
	if (this->rightNode != nullptr)
		return this->rightNode->getRightArea();
	else
		return this->value * (this->rightB - this->leftB);
}

double TreeNode::getLeftArea() const
{
	if (this->leftNode != nullptr)
		return this->leftNode->getLeftArea();
	else
		return this->value * (this->rightB - this->leftB);
}

std::shared_ptr<TreeNode> TreeNode::getNextNode()
{
	return NextNode;
}
std::shared_ptr<TreeNode> TreeNode::getLastNode()
{
	return LastNode;
}

std::shared_ptr<TreeNode> TreeNode::releaseLeft(double B)
{
	if (leftNode == nullptr && rightNode == nullptr)
	{
		if (rightB <= B)
		{
			return nullptr;
		}
		else
		{
			leftB = B > leftB ? B : leftB;
			return shared_from_this();
		}
	}
	else
	{
		if (B <= midB)
		{
			std::shared_ptr<TreeNode> left = nullptr;
			left = this->leftNode->releaseLeft(B);
			if (left != this->leftNode)
			{
				leftNode->Delete();
				leftNode = left;
			}
		}
		else
		{
			std::shared_ptr<TreeNode> right = nullptr;
			right = rightNode->releaseLeft(B);
			if (right != this->rightNode)
			{
				leftNode->Delete();
				rightNode->Delete();
				rightNode = right;
				leftNode = nullptr;
			}
		}
		if (leftNode == nullptr) {
			std::shared_ptr<TreeNode> r = rightNode;
			rightNode = nullptr;
			return r;
		}
		else
		{
			double Lvalue = leftNode == nullptr ? LDBL_MAX : leftNode->getValue();
			double Rvalue = rightNode == nullptr ? -1 : rightNode->getValue();
			value = Lvalue > Rvalue ? Rvalue : Lvalue;
			leftB = leftNode->getLeftB();
			return shared_from_this();
		}
	}
}

std::shared_ptr<TreeNode> IntervalTree::getInterval(double point)
{
	return root->getInterval(point);
}

void IntervalTree::Slice(double B) {
	extend(B);
	this->root->Slice(B);
}


void IntervalTree::extend(double rightB)
{
	while (this->root->getRightB() < rightB) {
		auto Last = List->getLastNode();
		root = TreeNode::Create(root, rightB, defualtValue, List, Last);
	}
}

void IntervalTree::extend(double rightB, double value)
{
	while (this->root->getRightB() < rightB) {
		auto Last = List->getLastNode();
		root = TreeNode::Create(root, rightB, value, List, Last);
	}
}

void IntervalTree::addValue(double LB, double RB, double value)
{
	double rightB = root->getRightB();
	double LeftB = root->getLeftB();
	if (rightB < RB)
	{
		intoNextWindows(RB);
	}
	if (LeftB > LB)
	{
		LB = LeftB;
	}
	this->root->addValue(LB, RB, value);
}

double IntervalTree::getValue(double point) const
{
	return this->root->getValue(point);
}


void IntervalTree::releaseLeft(double B) {
	intoNextWindows(B);
	double RealseB = int((B - lastLeftB) / windows - 1) * windows + lastLeftB;
	double LB = root->getLeftB();
	if (RealseB > LB) {
		root->releaseLeft(RealseB);
	}
}

void IntervalTree::intoNextWindows()
{
	double Right = (int((root->getRightB() - lastLeftB) / windows) + 1) * windows + lastLeftB;
	extend(Right);
}

void IntervalTree::intoNextWindows(double x)
{
	if (x > root->getRightB()) {
		double Right = (int((x - lastLeftB) / windows) + 1) * windows + lastLeftB;
		extend(Right);
	}
}

void IntervalTree::changeDefualtValue(double now, double Value) {
	intoNextWindows(now);
	//auto node = List->getLastNode();
	double RB = root->getRightB();
	double Pvalue = root->getRightValue(now);
	if (Pvalue < Value) {
		throw std::range_error("now has Task running");
	}
	addValue(now, RB, Value - defualtValue);
	defualtValue = Value;
}

double IntervalTree::AllocatedArea_DD(double startPoint, double targetArea)
{
	// 返回的值为从startPoint开始分配的高
	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double hight = Node == List ? -1 : DBL_MAX;
	// std::cout << viewList() << std::endl;
	while (Node != List && hight > 0.001 && Node != nullptr)
	{
		double RB = Node->getRightB();
		double value = Node->getValue();
		hight = hight > value ? value : hight;
		if ((RB - startPoint) * hight >= targetArea || hight < 0.001)
			break;
		Node = Node->getNextNode();
	}
	return Node == List || hight < 0.001 || Node == nullptr ? -1 : hight;
}

double IntervalTree::AllocatedArea_DDD(double startPoint, double targetArea, double hight)
{
	// 返回的值为最接近(往后)满足高为hight，面积为targetArea的区块的起点
	if (hight <= 0)
		return root->getRightB();

	intoNextWindows(startPoint);

	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double leftArea = 0;
	double ansPoint = startPoint;
	while (Node != List && Node != nullptr)
	{
		if (Node->getValue() < hight)
		{
			leftArea = 0;
			ansPoint = Node->getRightB();
		}
		else
		{
			leftArea = hight * (Node->getRightB() - ansPoint);
		}
		if (leftArea >= targetArea)
		{
			return ansPoint;
		}
		Node = Node->getNextNode();
	}
	return hight > defualtValue ? -1 : ansPoint;
}

std::string IntervalTree::viewList() const
{
	std::ostringstream stream;
	auto NextNode = List->getNextNode();
	while (NextNode != List)
	{
		stream << "(" << NextNode->getLeftB() << "," << NextNode->getRightB() << "," << NextNode->getValue() << ")";
		NextNode = NextNode->getNextNode();
	}
	return stream.str();
}


// int main() {
// 	IntervalTree A(0.4, 0, 5);
// 	A.releaseLeft(0.000001);
// 	A.releaseLeft(5.000001);
// 	A.releaseLeft(10.000001);
// 	std::cout<<A.viewList();
// 	return 0;
// }