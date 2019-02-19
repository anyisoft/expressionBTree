// expressionBTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

struct tree {
	char data;
	struct tree *left;
	struct tree *right;
};
typedef struct tree treenode;
typedef treenode *btree;

struct stack_node {
	int data;
	struct stack_node *next;
};
typedef struct stack_node stack_list;
typedef stack_list *link;

link operator_stack = NULL;
link operand_stack  = NULL;

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

/**
 * stack data save
 */
link push(link stack, int value)
{
	link new_node;

	new_node = (link)malloc(sizeof(stack_list) );
	if (!new_node) {
		printf("allocate memory failed.\n");
		return NULL;
	}

	new_node->data = value;
	new_node->next = stack;
	stack = new_node;

	return stack;
}

/**
 * stack data restore
 */
link pop(link stack, int *value)
{
	link top;

	if (stack != NULL) {
		top = stack;
		stack = stack->next;
		*value = top->data;
		free(top);

		return stack;
	}
	else {
		*value = -1;
		return NULL;
	}
}

/**
 * make sure the stack empty or not
 */
int empty(link stack)
{
	if (stack == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * make sure the data is operator or not
 */
int isoperator(char op)
{
	switch (op) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/': return 1;
	default: return 0;
	}
}

/**
 * specify the operator priority
 */
int priority(char op)
{
	switch (op) {
	case '*':
	case '/': return 3;
	case '+':
	case '-': return 2;
	case '(': return 1;
	default: return 0;
	}
}

void inorder2preorder(const char *inorder_expression, char *preorder_expression)
{
	int pos = 0;
	int op = 0;
	int preorder_pos = 0;
	int operand1 = 0;
	int operand2 = 0;

	while (inorder_expression[pos] != '\0' && inorder_expression[pos] != '\n') {
		if (isoperator(inorder_expression[pos]) ) {
			if (empty(operator_stack) || inorder_expression[pos] == '(' ) {
				operator_stack = push(operator_stack, inorder_expression[pos]);
			}
			else {
				if (inorder_expression[pos] == ')' ) {
					while (operator_stack->data != '(' ) {
						operator_stack = pop(operator_stack, &op);
						preorder_expression[preorder_pos++] = op;

						operand_stack = pop(operand_stack, &operand2);
						operand_stack = pop(operand_stack, &operand1);
						preorder_expression[preorder_pos++] = operand1;
						preorder_expression[preorder_pos++] = operand2;
					}

					operator_stack = pop(operator_stack, &op); // pop up the ')'
				}
				else {
					while (!empty(operator_stack) && priority(inorder_expression[pos]) <= priority(operator_stack->data) ) {
						operator_stack =pop(operator_stack, &op);
						preorder_expression[preorder_pos++] = op;

						operand_stack = pop(operand_stack, &operand2);
						operand_stack = pop(operand_stack, &operand1);
						preorder_expression[preorder_pos++] = operand1;
						preorder_expression[preorder_pos++] = operand2;
					}

					operator_stack = push(operator_stack, inorder_expression[pos]);
				}
			}
		}
		else {
			operand_stack = push(operand_stack, inorder_expression[pos]);
		}

		pos++;
	}

	while (!empty(operator_stack) ) {
		operator_stack = pop(operator_stack, &op);
		preorder_expression[preorder_pos++] = op;
	}

	while (!empty(operand_stack) ) {
		operand_stack = pop(operand_stack, &op);
		preorder_expression[preorder_pos++] = op;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	btree root = NULL;
	int result;

	// 表达式二叉树结点数据
	// int data[8] = { ' ', '-', '*', '*', '5', '6', '4', '3'};
	int data[100];

	// 输入中序表达式
	char inorder_expression[100];
	char preorder_expression[100];
	printf("please input the order expression ==> ");
	gets(inorder_expression);

	// 转为前序表达式
	memset(preorder_expression, 0, sizeof(preorder_expression) );
	inorder2preorder(inorder_expression, preorder_expression);
	printf("the preorder expression is: %s", preorder_expression);

	memset(data, 0, sizeof(data) );
	data[0] = ' ';
	// strncpy((char*)&data[1], preorder_expression, strlen(preorder_expression) );
	for (int i = 0; i < strlen(preorder_expression); i++) {
		data[i+1] = preorder_expression[i];
	}

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