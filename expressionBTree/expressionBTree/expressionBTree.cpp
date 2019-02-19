// expressionBTree.cpp : �������̨Ӧ�ó������ڵ㡣
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
 * �������ʽ������
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
 * ���ʽ���������������
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
 * ���ʽ��������ǰ�����
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
 * ���ʽ�������ĺ������
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
 * ���������ʽ��ֵ
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
 * ���ʽ�������������
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

	// ���ʽ�������������
	int data[8] = { ' ', '-', '*', '*', '5', '6', '4', '3'};

	root = createbtree(data, 1);

	printf("������ʽ��");
	inorder(root);
	
	printf("\nǰ����ʽ��");
	preorder(root);

	printf("\n������ʽ��");
	postorder(root);

	result = cal(root);
	printf("\n���ʽ����ǣ�%d\n", result);

	return 0;
}

