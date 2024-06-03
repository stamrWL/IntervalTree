
#include "intervalTree.h"


void TreeNode::Silce(double B)
{
	if (B <= leftB || B >= rightB)
	{
		return;
	}
	if (leftNode == NULL && rightNode == NULL)
	{
		TreeNode *NNode = NextNode; // 叶子节点组成链表
		TreeNode *LNode = LastNode; // 叶子节点组成链表
		LastNode = NextNode = NULL;
		this->midB = B;

		this->leftNode = new TreeNode(leftB, midB, value);
		this->rightNode = new TreeNode(midB, rightB, value);
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
			this->leftNode->Silce(B);
		}
		else
		{
			this->rightNode->Silce(B);
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
		if (this->rightNode != NULL)
			this->rightNode->addValue(LB, midB, value);
		if (this->leftNode != NULL)
			this->rightNode->addValue(midB, RB, value);
		return;
	}
	else
	{
		if (leftNode == NULL || rightNode == NULL)
		{
			double MB = abs(2 * LB - leftB - rightB) > abs(2 * RB - leftB - rightB) ? RB : LB;
			this->Silce(MB);
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

double TreeNode::getValue()
{
	return value;
}

double TreeNode::getValue(double point)
{
	if (point < leftB || point > rightB)
	{
		return -1;
	}
	if (leftNode == NULL || rightNode == NULL)
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

TreeNode *TreeNode::getInterval(double point)
{
	if (point < leftB || point > rightB)
	{
		return NULL;
	}
	if (leftNode == NULL || rightNode == NULL)
	{
		return this;
	}
	else
	{
		if (point <= midB)
			return leftNode->getInterval(point);
		else
			return rightNode->getInterval(point);
	}
}

double TreeNode::getRightArea()
{
	if (this->rightNode != NULL)
		return this->rightNode->getRightArea();
	else
		return this->value * (this->rightB - this->leftB);
}

double TreeNode::getLeftArea()
{
	if (this->leftNode != NULL)
		return this->leftNode->getLeftArea();
	else
		return this->value * (this->rightB - this->leftB);
}

TreeNode *TreeNode::getNextNode()
{
	return NextNode;
}
TreeNode *TreeNode::getLastNode()
{
	return LastNode;
}

TreeNode *TreeNode::releaseLeft(double B)
{
	if (leftNode == NULL && rightNode == NULL)
	{
		if (rightB <= B)
		{
			return NULL;
		}
		else
		{
			leftB = B > leftB ? B : leftB;
			return this;
		}
	}
	else
	{
		if (B <= midB)
		{
			TreeNode *left = NULL;
			left = this->leftNode->releaseLeft(B);
			if (left != this->leftNode)
			{
				delete leftNode;
				leftNode = left;
			}
		}
		else
		{
			TreeNode *right = NULL;
			right = rightNode->releaseLeft(B);
			if (right != this->rightNode)
			{
				delete leftNode;
				delete rightNode;
				rightNode = right;
				leftNode = NULL;
			}
		}
		if (leftNode == NULL)
			return rightNode;
		else
		{
			double Lvalue = leftNode == NULL ? LDBL_MAX : leftNode->getValue();
			double Rvalue = rightNode == NULL ? -1 : rightNode->getValue();
			value = Lvalue > Rvalue ? Rvalue : Lvalue;
			return this;
		}
	}
}

void IntervalTree::extend(double rightB)
{
	double nowRB = this->root->getRightB();
	if (rightB <= nowRB)
	{
		return;
	}
	auto Last = List->getLastNode();
	root = new TreeNode(root, rightB, defualtValue, List, Last);
}

void IntervalTree::extend(double rightB, double value)
{
	double nowRB = this->root->getRightB();
	if (rightB <= nowRB)
	{
		return;
	}
	auto Last = List->getLastNode();
	root = new TreeNode(root, rightB, value, List, Last);
}

void IntervalTree::addValue(double LB, double RB, double value)
{
	double rightB = root->getRightB();
	double LeftB = root->getLeftB();
	if (rightB < RB)
	{
		extend(RB);
	}
	if (LeftB > LB)
	{
		LB = LeftB;
	}
	this->root->addValue(LB, RB, value);
}

double IntervalTree::getValue(double point)
{
	return this->root->getValue(point);
}

void IntervalTree::intoNextWindows()
{
	
	extend(root->getRightB() + windows);

}

double IntervalTree::AllocatedArea_DD(double startPoint, double targetArea)
{
	// 返回的值为从startPoint开始分配的高
	TreeNode *Node = getInterval(startPoint);
	double hight = Node == List ? -1 : DBL_MAX;
	// std::cout << viewList() << std::endl;
	while (Node != List && hight > 0.001)
	{
		double RB = Node->getRightB();
		double value = Node->getValue();
		hight = hight > value ? value : hight;
		if ((RB - startPoint) * hight >= targetArea || hight < 0.001)
			break;
		Node = Node->getNextNode();
	}
	return Node == List || hight < 0.001 ? -1 : hight;
}

double IntervalTree::AllocatedArea_DDD(double startPoint, double targetArea, double hight)
{
	// 返回的值为最接近(往后)满足高为hight，面积为targetArea的区块的起点
	if (hight <= 0)
		return root->getRightB();
	while(startPoint > root->getRightB()) {
		intoNextWindows();
	}
	TreeNode *Node = getInterval(startPoint);
	double leftArea = 0;
	double ansPoint = startPoint;
	while (Node != List && Node != NULL)
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
	return root->getRightB();
}

TreeNode *IntervalTree::getInterval(double point)
{
	return root->getInterval(point);
}

std::string IntervalTree::viewList()
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
