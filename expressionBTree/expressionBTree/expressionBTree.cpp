// expressionBTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>

struct tree {
	char data;
	struct tree *left;
	struct tree *right;
};
typedef struct tree treenode;
typedef treenode *btree;

/**
 * 创建表达式二叉树
 */
btree createbtree(int *data, int pos)
{
	btree newnode;

	if (data[pos] == 0 || pos > 7) {
		return NULL;
	}
	else {
		newnode = (btree)malloc(sizeof(treenode) );
		newnode->data = data[pos];
		newnode->left = createbtree(data, 2*pos);
		newnode->right = createbtree(data, 2*pos+1);

		return newnode;
	}
}

/**
 * 表达式二叉树的中序输出
 */
void inorder(btree ptr)
{
	if (ptr != NULL) {
		inorder(ptr->left);
		printf("%c", ptr->data);
		inorder(ptr->right);
	}
}

/**
 * 表达式二叉树的前序输出
 */
void preorder(btree ptr)
{
	if (ptr != NULL) {
		printf("%c", ptr->data);
		preorder(ptr->left);
		preorder(ptr->right);
	}
}

/**
 * 表达式二叉树的后序输出
 */
void postorder(btree ptr)
{
	if (ptr != NULL) {
		postorder(ptr->left);
		postorder(ptr->right);
		printf("%c", ptr->data);
	}
}

/**
 * 计算二叉表达式的值
 */
int getvalue(int op, int operand1, int operand2)
{
	switch ((char)op) {
	case '*':
		return (operand1 * operand2);
	case '/':
		return (operand1 / operand2);
	case '+':
		return (operand1 + operand2);
	case '-':
		return (operand1 - operand2);
	}
}

/**
 * 表达式二叉树后序计算
 */
int cal(btree ptr)
{
	int operand1 = 0;
	int operand2 = 0;

	if (ptr->left == NULL && ptr->right == NULL) {
		return ptr->data - 48;
	}
	else {
		operand1 = cal(ptr->left);
		operand2 = cal(ptr->right);

		return getvalue(ptr->data, operand1, operand2);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	btree root = NULL;
	int result;

	// 表达式二叉树结点数据
	int data[8] = { ' ', '-', '*', '*', '5', '6', '4', '3'};

	root = createbtree(data, 1);

	printf("中序表达式：");
	inorder(root);
	
	printf("\n前序表达式：");
	preorder(root);

	printf("\n后序表达式：");
	postorder(root);

	result = cal(root);
	printf("\n表达式结果是：%d\n", result);

	return 0;
}

