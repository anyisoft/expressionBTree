// expressionBTree.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <stdio.h>
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

/**
 * 后缀表达式建树过程
 */
btree postTree(char s[], int len)
{
	int i;
	btree p, root;

	struct stack {
		treenode *vec[256];
		int top;
	};
	struct stack q;
	q.top = -1;

	// 遍历字符串
	// 若为操作数，则生成结点并将指向该结点的指针入栈
	// 若为运算符，则生成结点并从栈中弹出两个指向操作数结点的指针作为右、左孩子，然后将新生成的结点入栈
	for (i = 0; i < len; i++) {
		if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
			p = (btree)malloc(sizeof(treenode));
			p->data = s[i];
			p->right = q.vec[q.top--];
			p->left = q.vec[q.top--];
			q.vec[++q.top] = p;
		}
		else {
			p = (btree)malloc(sizeof(treenode));
			p->data = s[i];
			p->left = NULL;
			p->right = NULL;
			q.vec[++q.top] = p;
		}
	}

	root = q.vec[q.top]; // 这一步很关键，因为该二叉树的根结点最后被保留在了栈中
	return root;
}

/**
 * 前缀表达式建树过程
 * 由于操作数在叶节点位置和前缀表达式的特点，
 * 其建树过程与后缀表达式一致，
 * 只是扫描方向相反
 */
btree preTree(char s[], int len)
{
	int i;
	btree p, root;

	struct stack {
		treenode *vec[256];
		int top;
	};
	struct stack q;
	q.top = -1;

	// 遍历字符串
	// 若为操作数，则生成结点并将指向该结点的指针入栈
	// 若为运算符，则生成结点并从栈中弹出两个指向操作数结点的指针作为左、右孩子，然后将新生成的结点入栈
	for (i = len-1; i >= 0; i--) {
		if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
			p = (btree)malloc(sizeof(treenode));
			p->data = s[i];
			p->left = q.vec[q.top--];
			p->right = q.vec[q.top--];
			q.vec[++q.top] = p;
		}
		else {
			p = (btree)malloc(sizeof(treenode));
			p->data = s[i];
			p->left = NULL;
			p->right = NULL;
			q.vec[++q.top] = p;
		}
	}

	root = q.vec[q.top]; // 这一步很关键，因为该二叉树的根结点最后被保留在了栈中
	return root;
}

/**
 * 中序表达式建树过程(递归)
 * s -- 字符串的首地址
 * i -- 子串起始位置
 * j -- 子串终止位置
 * 当 i==j 时，子串只有一个字符时，即叶子节点，创建只有一个根节点的二叉树并返回
 * 当 i!=j 时，
 *   根据运算规则(先乘除后加减)，在串中查找'+'和'-'，以最后的'+'或'-'为根(体现从左到右的原则)，将串分为两部分；
 *   如果没有'+'或'-'时，则进一步查找'*'或'/'(体现先乘除后加减)，同样以最后的运算符为根，将串分为两部分；
 *   即左子树和右子树。创建一个根节点，将找到的运算符放入。递归调用自身进入左子树的建树过程，之后同样创建右子树。
 * remark: 目前没有处理括号()
 */
btree inTree(char s[], int i, int j)
{
	// 动态生成的树节点
	btree p;
	int k, flag = 0, pos;

	if (i == j) {
		p = (btree)malloc(sizeof(treenode));
		p->data = s[i];
		p->left = NULL;
		p->right = NULL;

		return  p;
	}

	// 以下是i != j 的情况

	// 从左往右找最后一个+ 或- (先找+/-体现先乘除后加减的原则)
	for (k = i; k <= j; k++) {
		if (s[k] == '+' || s[k] == '-') {
			flag = 1;
			pos = k;
		}
	}

	if (flag == 0) {
		for (k = i; k <= j; k++) {
			if (s[k] == '*' || s[k] == '/') {
				flag = 1;
				pos = k;
			}
		}
	}

	// 若flag 不等于0，则以pos 为界将字符串分为左右两部分，分别对应表达式二叉树的左、右子树
	// 同样以最后的运算符为根，将串分为两部分，创建一个根节点，将找到的运算符放入
	if (flag != 0) {
		p = (btree)malloc(sizeof(treenode));
		p->data = s[pos];
		p->left = inTree(s, i, pos-1);
		p->right = inTree(s, pos+1, j);

		return p;
	}

	return NULL;
}

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	btree root = NULL;
	int result;
	int len;

	// 表达式二叉树结点数据
	// int data[8] = { ' ', '-', '*', '*', '5', '6', '4', '3'};
	int data[100];

	// 输入中序表达式
	char inorder_expression[100];
	// char preorder_expression[100];
	// char postorder_expression[100];
	printf("please input the inorder expression ==> ");
	gets(inorder_expression);
	// printf("please input the preorder expression ==> ");
	// gets(preorder_expression);
	// printf("please input the postorder expression ==> ");
	// gets(postorder_expression);

	// 转为前序表达式
	// memset(preorder_expression, 0, sizeof(preorder_expression) );
	// inorder2preorder(inorder_expression, preorder_expression);
	// printf("the preorder expression is: %s", preorder_expression);

	// memset(data, 0, sizeof(data) );
	// data[0] = ' ';
	// strncpy((char*)&data[1], preorder_expression, strlen(preorder_expression) );
	// for (int i = 0; i < strlen(preorder_expression); i++) {
	// 	data[i+1] = preorder_expression[i];
	// }

	// root = createbtree(data, 1);

	// len = strlen(preorder_expression);
	// root = preTree(preorder_expression, len);
	// len = strlen(postorder_expression);
	// root = postTree(postorder_expression, len);
	len = strlen(inorder_expression);
	root = inTree(inorder_expression, 0, len-1);

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