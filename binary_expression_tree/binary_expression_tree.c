/**
****************************************************************************************************************************************
* Program title		: Chương trình tính toán giá trị biểu thức toán học "cộng, trừ, nhân, chia, lũy thừa" sử dụng cây nhị phân
* File name			: binary_expression_tree.c
* Author			: Khoa Dang Nguyen
* Date				: 09/07/2023
* Details program	:
*	Chương trình nhận vào một chuỗi là biểu thức trung tố (infix) gồm các phép toán "cộng, trừ, nhân, chia, lũy thừa"
*		- B1: xử lý chuỗi, nếu chuỗi không hợp lệ thì thông báo lỗi nhập lại, nếu hợp lệ thì format lại chuỗi để tiếp tục xử lý
*		- B2: chuyển đổi biểu thức từ trung tố (infix) sang biểu thức tiền tố (prefix)
*		- B3: khởi tạo cây biểu thức bằng cách nạp dữ liệu từ biểu thức tiền tố (prefix) vào cây nhị phân
*		- B4: duyệt cây (inorder) để tính toán kết quả
****************************************************************************************************************************************
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

//***********************************************************************
//Tree Implementation
//***********************************************************************
typedef struct tree_node
{
	char* value;
	struct tree_node* ptr_left;
	struct tree_node* ptr_right;
} TNode;

// khởi tạo node 
TNode* create_tnode(char* data)
{
	TNode* tree_node = (TNode*)malloc(sizeof(TNode));

	if (tree_node == NULL)
		return NULL;

	tree_node->value = data;
	tree_node->ptr_left = tree_node->ptr_right = NULL;

	return tree_node;
}

// function nạp phần tử vào bên trái node cha
void insert_left(TNode* root, TNode* node_left)
{
	root->ptr_left = node_left;
}

// function nạp phần tử vào bên phải node cha
void insert_right(TNode* root, TNode* node_right)
{
	root->ptr_right = node_right;
}

// function giải phóng cây nhị phân
void free_tree(TNode* root)
{
	if (root != NULL)
	{
		free_tree(root->ptr_left);
		free_tree(root->ptr_right);
		free(root);

		root = NULL;
	}
}

//***********************************************************************
//Stack Implementation
//***********************************************************************
typedef struct snode
{
	char* value;
	struct snode* ptr_next;
} SNode;

typedef struct stack
{
	SNode* top;
	int count;
} Stack;

Stack* initial_stack()
{
	Stack* stack = (Stack*)malloc(sizeof(Stack));

	if (stack == NULL)
		return NULL;

	stack->top = NULL;
	stack->count = 0;

	return stack;
}

SNode* create_snode(char* data)
{
	SNode* stack_node = (SNode*)malloc(sizeof(SNode));

	if (stack_node == NULL)
		return NULL;

	stack_node->value = data;
	stack_node->ptr_next = NULL;

	return stack_node;
}

// function lấy giá trị phần tử đỉnh stack
SNode* peek_stack(Stack* stack)
{
	SNode* stack_node = NULL;

	if (stack->top != NULL)
		stack_node = stack->top;

	return stack_node;
}

// function xóa phâng tử ra khỏi stack
char* pop_stack(Stack* stack)
{
	char* value = NULL;

	if (stack->top != NULL)
	{
		SNode* access_to_free = stack->top;
		stack->top = stack->top->ptr_next;
		value = access_to_free->value;

		free(access_to_free);

		stack->count--;
	}

	return value;
}

// function nạp phần tử vào stack
void push_stack(Stack* stack, SNode* stack_node)
{
	if (stack->top == NULL)
		stack->top = stack_node;
	else
	{
		stack_node->ptr_next = stack->top;
		stack->top = stack_node;
	}

	stack->count++;
}

// function kiểm tra stack có phần tử không
int is_stack_empty(Stack* stack)
{
	if (stack->top == NULL)
		return 1;
	return 0;
}

// function xóa stack
void clear_stack(Stack* stack)
{
	while (stack->top != NULL)
		pop_stack(stack);

	printf_s("\n Free STACK successful!");
}

// function in giá trị trong stack
void print_stack(Stack* stack)
{
	while (stack->top != NULL)
	{
		printf_s(" | %s |\n", peek_stack(stack)->value);
		pop_stack(stack);
	}

	printf_s("\n");
}

//******************************************************************************************
// Stack Tree Implementation (Stack có các phần tử là kiểu dữ liệu node tree)
//******************************************************************************************
typedef struct snode_tree
{
	TNode* tree_node;
	struct snode_tree* ptr_next;
} SNodeTree;

typedef struct stack_tree
{
	SNodeTree* top;
} StackTree;

// function khởi tạo stack
StackTree* initial_stack_tree()
{
	StackTree* stack_tree = (StackTree*)malloc(sizeof(StackTree));

	if (stack_tree == NULL)
		return NULL;

	stack_tree->top = NULL;

	return stack_tree;
}

// function tạo node có giá trị là node trong tree
SNodeTree* create_snode_tree(TNode* data)
{
	SNodeTree* stack_node_tree = (SNodeTree*)malloc(sizeof(SNodeTree));

	if (stack_node_tree == NULL)
		return NULL;

	stack_node_tree->tree_node = data;
	stack_node_tree->ptr_next = NULL;

	return stack_node_tree;
}

// function lấy phần tử đỉnh stack
SNodeTree* peek_stack_tree(StackTree* stack_tree)
{
	if (stack_tree->top != NULL)
		return stack_tree->top;
}

// function xóa phần tử đỉnh stack
SNodeTree* pop_stack_tree(StackTree* stack_tree)
{
	TNode* value = NULL;

	if (stack_tree->top != NULL)
	{
		SNodeTree* access_to_free = stack_tree->top;
		stack_tree->top = stack_tree->top->ptr_next;
		value = access_to_free->tree_node;

		free(access_to_free);
	}

	return value;
}

// function nạp phần tử vào stack
void push_stack_tree(StackTree* stack_tree, TNode* node_tree)
{
	SNodeTree* snode_tree = create_snode_tree(node_tree);

	if (stack_tree->top == NULL)
		stack_tree->top = snode_tree;
	else
	{
		snode_tree->ptr_next = stack_tree->top;
		stack_tree->top = snode_tree;
	}
}

// function kiểm tra stack có rỗng không
int is_stack_tree_empty(Stack* stack)
{
	if (stack->top == NULL)
		return 1;
	return 0;
}


//***********************************************************************
// Queue Implementation
//***********************************************************************
typedef struct qnode
{
	char* value;
	struct qnode* ptr_previous;
} QNode;

typedef struct queue
{
	QNode* tail;
	QNode* head;
	int count;
} Queue;

Queue* initial_queue()
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));

	if (queue == NULL)
		return NULL;

	queue->head = queue->tail = NULL;
	queue->count = 0;

	return queue;
}

// function tạo node
QNode* create_qnode(char* data)
{
	QNode* qnode = (QNode*)malloc(sizeof(QNode));

	if (qnode == NULL)
		return NULL;

	qnode->value = data;
	qnode->ptr_previous = NULL;

	return qnode;
}

// function nạp phần tử vào cuối queue
void enqueue(Queue* queue, QNode* qnode)
{
	if (queue->head == NULL)
		queue->head = queue->tail = qnode;
	else
	{
		queue->tail->ptr_previous = qnode;
		queue->tail = qnode;
	}

	queue->count++;
}

// function xóa phần tử đầu queue
char* dequeue(Queue* queue)
{
	char* value = NULL;

	if (queue->head != NULL)
	{
		QNode* access_to_free = queue->head;
		queue->head = queue->head->ptr_previous;
		value = access_to_free->value;

		free(access_to_free);
	}

	queue->count--;

	return value;
}

// function lấy phần tử đầu queue
QNode* peek_queue(Queue* queue)
{
	if (queue->head == NULL)
		return NULL;

	QNode* qnode = NULL;
	qnode = queue->head;

	return qnode;
}

// function kiểm tra queue có phần tử không
int is_queue_empty(Queue* queue)
{
	if (queue->head == NULL)
		return 1;
	return 0;
}

// function xóa các phần tử trong queue
void clear_queue(Queue* queue)
{
	while (queue->head != NULL)
		dequeue(queue);

	printf_s("\n Free QUEUE successful!");
}

// function in các giá trị trong queue
void print_queue(Queue* queue)
{
	while (queue->head != NULL)
	{
		char* value = dequeue(queue);
		printf_s("%s ", value);
	}
}

//***********************************************************************
//Linkedlist Implementation
//***********************************************************************
typedef struct lnode
{
	char* value;
	struct lnode* ptr_next;
} LNode;

typedef struct linkedlist
{
	LNode* head;
	LNode* tail;
	int count;
} LinkedList;

LinkedList* initial_linkedlist()
{
	LinkedList* linkedlist = (LinkedList*)malloc(sizeof(LinkedList));

	if (linkedlist == NULL)
		return NULL;

	linkedlist->head = linkedlist->tail = NULL;
	linkedlist->count = 0;

	return linkedlist;
}

LNode* create_lnode(char* data)
{
	LNode* linkedlist_node = (LNode*)malloc(sizeof(LNode));

	if (linkedlist_node != NULL)
	{
		linkedlist_node->value = data;
		linkedlist_node->ptr_next = NULL;
	}

	return linkedlist_node;
}

// function nạp phần tử vào đầu tiên linked list
void add_first(LinkedList* linkedlist, LNode* node)
{
	if (linkedlist->head == NULL)
		linkedlist->head = linkedlist->tail = node;
	else
	{
		node->ptr_next = linkedlist->head;
		linkedlist->head = node;
	}

	linkedlist->count++;
}

// function nạp phần tử vào cuối linked list
void add_last(LinkedList* linkedlist, LNode* node)
{
	if (linkedlist->head == NULL)
		linkedlist->head = linkedlist->tail = node;
	else
	{
		linkedlist->tail->ptr_next = node;
		linkedlist->tail = node;
	}

	linkedlist->count++;
}

// function xóa phần tử đầu tiên trong linked list
void remove_first(LinkedList* linkedlist)
{
	if (linkedlist->head != NULL)
	{
		LNode* access_to_free = linkedlist->head;
		linkedlist->head = linkedlist->head->ptr_next;
		free(access_to_free);
	}

	linkedlist->count--;
}

// function xóa phần tử cuối cùng trong linked list
void remove_last(LinkedList* linkedlist)
{
	if (linkedlist->head != NULL)
	{
		LNode* node_before_tail = linkedlist->head;

		for (LNode* i = linkedlist->head->ptr_next; i != linkedlist->tail; i = i->ptr_next)
			node_before_tail = i;

		LNode* access_to_free = linkedlist->tail;
		linkedlist->tail = node_before_tail;
		linkedlist->tail->ptr_next = NULL;

		free(access_to_free);
	}

	linkedlist->count--;
}

// function xóa linked list
void clear_linkedlist(LinkedList* linkedlist)
{
	LNode* access_to_delete = NULL;

	while (linkedlist->head != NULL)
	{
		access_to_delete = linkedlist->head;
		linkedlist->head = linkedlist->head->ptr_next;

		free(access_to_delete);
	}
}

// function in giá trị trong linked list
void print_linkedlist(LinkedList* linkedlist)
{
	printf_s("\n  ==> ");

	for (LNode* i = linkedlist->head; i != NULL; i = i->ptr_next)
	{
		if (is_operand(i->value))
		{
			printf_s("%s", i->value);

			if (i->ptr_next != NULL)
			{
				if (*i->ptr_next->value != '^')
					if (is_operator(*i->ptr_next->value) || is_operand(i->ptr_next->value))
						printf_s(" ");
			}
		}
		else
		{
			printf_s("%s", i->value);

			if (i->ptr_next != NULL)
			{
				if (is_closed_bracket(*i->value))
				{
					if (*i->ptr_next->value != '^')
						if (is_operator(*i->ptr_next->value))
							printf_s(" ");
				}
				else if (is_operator(*i->value))
				{
					if (*i->value != '^')
						if (is_operand(i->ptr_next->value) || is_open_bracket(*i->ptr_next->value))
							printf_s(" ");
				}
			}
		}
	}
}


//***********************************************************************
// một số hàm xử lý kiểm tra chuỗi hợp lệ.
//***********************************************************************

// function lấy độ ưu tiên giữa các operator
int get_priority(char c)
{
	int priority = 0;

	switch (c)
	{
		case '+':
		case '-':
			priority = 2;
			break;

		case '*':
		case '/':
		case '%':
		case 'x':
			priority = 3;
			break;

		case '^':
			priority = 4;
			break;

		default:
			break;
	}

	return priority;
}

// function lấy độ dài chuỗi.
int get_length(char expression[])
{
	int i = 0;
	while (expression[i++] != '\0');
	return --i;
}

// function kiểm tra kí có phải là kí tự số hay không?
int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

// function kiểm tra kí có phải là toán hạng hay không?
int is_operand(char* str)
{
	for (int i = 0; *(str + i) != '\0'; i++)
	{
		if (is_digit(*(str + i)) || *(str + i) == '.')
			continue;
		else
			return 0;
	}

	return 1;
}

// function kiểm tra kí có phải là operator hay không?
int is_operator(char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case 'x':
	case '/':
	case '%':
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

// function kiểm tra kí tự có phải là 'dấu ngoặc mở' hay không?
int is_open_bracket(char c)
{
	if (c == '(')
		return 1;
	return 0;
}

// function kiểm tra kí tự có phải là 'dấu ngoặc đóng' hay không?
int is_closed_bracket(char c)
{
	if (c == ')')
		return 1;
	return 0;
}

// function kiểm tra kí có phải là 'dấu ngoặc' không?
int is_brackets(char c)
{
	if (is_open_bracket(c) || is_closed_bracket(c))
		return 1;
	return 0;
}

// function kiểm tra kí có phải là 'khoảng trắng' không?
int is_space(char c)
{
	if (c == ' ')
		return 1;
	return 0;
}

// function trả về biểu thức đã được loại bỏ khoảng trắng.
char* get_expression_is_not_space(char expression[])
{
	// độ dài chuỗi biểu thức
	int length = get_length(expression);

	// khai báo khởi tạo số lượng khoảng trắng của biểu thức
	int number_of_spaces = 0;

	// đếm số lượng khoảng trắng của chuỗi biểu thức
	for (int i = 0; i < length; i++)
	{
		if (is_space(expression[i]))
			++number_of_spaces;
	}

	// khai báo chuỗi biểu thức đã được loại bỏ khoảng trắng
	char* expression_without_space = (char*)calloc((length + 1 - number_of_spaces), sizeof(char));
	if (expression_without_space == NULL)
		return;

	// gán ký tự cuối chuỗi '/0'
	*(expression_without_space + (length - number_of_spaces + 1)) = '\0';

	// - lặp qua từng ký tự của chuỗi biểu thức ban đầu, kiểm tra nếu:
	//		+ nếu là khoảng trắng thì bỏ qua
	//		+ ngược lại nạp vào chuỗi biểu thức không chứa khoảng trắng
	int j = 0;
	for (int i = 0; i < length; i++)
	{
		if (!is_space(expression[i]))
			*(expression_without_space + j++) = expression[i];
	}

	return expression_without_space;
}

// function kiểm tra kí tự '+' or '-' là operator bình thường hay dấu âm dương.
int is_negative_or_positive(char* before_current, char* current, char* after_current)
{
	if (is_digit(*after_current) || *after_current == '+' || *after_current == '-' || *after_current == '(')
	{
		if (before_current == NULL || is_operator(*before_current) || is_open_bracket(*before_current))
			return 1;
	}

	return 0;
}

// function trả về chuỗi từ kiểu dữ liệu kí tự (dùng để nạp vào linked list mà các phần tử là chuỗi)
char* create_string_from_character(char* character)
{
	char* string = (char*)calloc(2, sizeof(char));

	if (string != NULL)
	{
		*(string + 0) = *character;
		*(string + 1) = '\0';

		return string;
	}

	return NULL;
}

// function kiểm tra chuỗi biểu thức nhận vào có hợp lệ hay không?
int is_expression_valid(char expression[])
{
	// - các trường hợp chuỗi bị lỗi:
	//		+ biểu thức bắt đầu bằng những ký tự khác các ký tự: digit, '+' và '-', '('
	//		+ biểu thức chứa các ký tự khác các ký tự: operator, operand, brackets, '.'
	//		+ số lượng dấu ngoặc tròn không chia hết cho 2 (mở ngoặc phải có đóng ngoặc)

	int length = get_length(expression);

	// Case 1:
	// biểu thức chỉ được phép bắt đầu bằng: digit, '+' và '-', '('
	if (!is_digit(expression[0]) && expression[0] != '-' && expression[0] != '+' && !is_open_bracket(expression[0]))
		return 0;

	// Case 2:
	// biểu thức chỉ được phép chứa operator, operand, brackets, '.'
	for (int i = 0; i < length; i++)
	{
		char* current = &expression[i];

		if (!is_digit(*current) && !is_brackets(*current) && !is_operator(*current) && *current != '.')
			return 0;
	}

	// Case 3: số lượng dấu ngoặc tròn phải chia hết cho 2 (mở ngoặc phải có đóng ngoặc).
	int number_of_brackets = 0;
	for (int i = 0; i < length; i++)
	{
		char* current = &expression[i];

		if (is_brackets(*current))
			number_of_brackets++;
	}
	if (!(number_of_brackets % 2 == 0))
		return 0;


	// Case 4: kiểm tra brackets, operator, digit
	for (int i = 0; i < length; i++)
	{
		char* current = &expression[i]; // khai báo khởi tạo phần tử tại i
		char* before_current = NULL; // khai báo phần tử đằng trước phần tử i
		char* after_current = NULL; // khai báo phần tử đằng sau phần tử i

		// khởi tạo giá trị cho phần tử trước và sau i
		if (i - 1 >= 0)
			before_current = &expression[i - 1];
		if (i + 1 <= length && expression[i + 1] != '\0')
			after_current = &expression[i + 1];

		// Case 4.1: brackets:
		//	- dấu ngoặc mở 
		//		+ đằng trước: operator, open bracket 
		//		+ đằng sau  : open bracket, digit, '-' (dấu âm), '+' (dấu dương) 		
		//	- dấu ngoặc đóng 
		//		+ đằng trước: digit, closed bracket 
		//		+ đằng sau  : closed bracket, operator,  digit, '\0' (kí tự kết thúc chuỗi)
		if (is_brackets(*current))
		{
			if (is_open_bracket(*current))
			{
				if (before_current != NULL)
				{
					if (!is_operator(*before_current) && !is_open_bracket(*before_current))
						return 0;
				}
				else
					continue;

				if (after_current != NULL)
				{
					if (!is_open_bracket(*after_current) && !is_digit(*after_current) && *after_current != '-' && *after_current != '+')
						return 0;
				}
				else
					return 0;
			}
			else if (is_closed_bracket(*current))
			{
				if (before_current != NULL)
				{
					if (!is_digit(*before_current) && !is_closed_bracket(*before_current))
						return 0;
				}
				else
					return 0;

				if (after_current != NULL)
				{
					if (!is_closed_bracket(*after_current) && !is_operator(*after_current) && *after_current != '\0')
						return 0;
				}
				else
					continue;
			}
		}

		// Case 4.2: operator:
		//	- phần tử trước digit: digit, operator, open bracket, dấu chấm 
		//  - phần tử sau digit  : digit, operator, closed bracket, dấu chấm, '\0' (kí tự kết thúc chuỗi)
		else if (is_operator(*current))
		{
			if (*current != '^')
			{
				if (before_current != NULL)
				{
					if (*current == '-' || *current == '+')
					{
						if (!is_digit(*before_current) && !is_operator(*before_current) && !is_brackets(*before_current))
							return 0;
					}
					else
					{
						if (!is_digit(*before_current) && !is_closed_bracket(*before_current))
							return 0;
					}
				}
				else
				{
					if (*current == '-' || *current == '+')
						continue;
					else
						return 0;
				}

				if (after_current != NULL)
				{
					if (!is_digit(*after_current) && !is_open_bracket(*after_current) && *before_current != '.' && *before_current != '+' && *before_current != '-' && *current == '\0')
						return 0;
				}
				else
					return 0;
			}
			else
			{
				if (before_current != NULL)
				{
					if (!is_digit(*before_current) && !is_closed_bracket(*before_current))
						return 0;
				}
				else
					return 0;

				if (after_current != NULL)
				{
					if (!is_open_bracket(*after_current) && !is_digit(*after_current) && *after_current != '.' && *after_current != '-' && *after_current != '+')
						return 0;
				}
				else
					return 0;
			}
		}

		// Case 4.3: digit:
		//	- phần tử trước digit: digit, operator, open bracket, dấu chấm 
		//	- phần tử sau digit  : digit, operator, closed bracket, dấu chấm, '\0' (kí tự kết thúc chuỗi)
		else if (is_digit(*current))
		{
			if (before_current != NULL)
			{
				if (!is_digit(*before_current) && !is_operator(*before_current) && !is_open_bracket(*before_current) && *before_current != '.')
					return 0;
			}
			else
				continue;

			if (after_current != NULL)
			{
				if (!is_digit(*after_current) && !is_operator(*after_current) && !is_closed_bracket(*after_current) && *after_current != '.' && *after_current != '\0')
					return 0;
			}
			else
				continue;
		}
	}

	return 1;
}

// function lấy tất cả kí tự trong hàng đợi tạo thành chuỗi và add vào linkedlist.
void dequeue_to_create_an_string_and_add_to_linkedlist(Queue* queue, LinkedList* linkedlist)
{
	// xử lý nếu hàng đợi có phần tử
	if (!is_queue_empty(queue))
	{
		// khai báo khỏi tạo chuỗi
		char* string_number = (char*)calloc(queue->count + 1, sizeof(char));

		if (string_number != NULL)
		{
			// thêm kí tự kết thúc chuỗi.
			*(string_number + queue->count) = '\0';

			// lấy tất cả phần tử trong queue nạp vào chuỗi.
			int i = 0;
			while (!is_queue_empty(queue))
			{
				*(string_number + i) = *dequeue(queue);
				++i;
			}

			add_last(linkedlist, create_lnode(string_number)); // add vào linkedlist.
		}
	}
}

// - function nhận vào biểu thức đã được loại bỏ khoảng trắng để xử lý:
//		+ nếu chuỗi invalid => trả về NULL
//		+ ngược lại trả về linkedlist đã được format từ chuỗi nhận vào
LinkedList* regular_expression(char expression[])
{
	if (!is_expression_valid(expression))
	{
		printf_s("\n ## Syntax ERROR");
		return NULL;
	}

	char open_bracket = '(';
	char zero = '0';
	char closed_bracket = ')';

	int number_of_open_brackets = 0; // số lượng dấu ngoặc '(' đã thêm vào khi phát hiện số âm hoặc dương
	int length = get_length(expression); // lấy độ dài chuỗi biểu thức để xử lý

	Queue* queue = initial_queue(); // khai báo hàng đợi
	LinkedList* linkedlist = initial_linkedlist(); // khai báo linked list (các phần tử được nạp vào là kiểu chuỗi)

	// lặp qua các phần tử trong chuỗi để xử lý
	for (int i = 0; i < length; i++)
	{
		char* current = &expression[i]; // phần tử i
		char* before_current = NULL; // phần tử trước i
		char* after_current = NULL; // phần tử sau i

		// khởi tạo cho phần tử trước và sau i
		if (i - 1 >= 0)
			before_current = &expression[i - 1];
		if (i + 1 <= length && expression[i + 1] != '\0')
			after_current = &expression[i + 1];

		// nếu giá trị tại i là digit hoặc '.' thì nạp vào queue
		if (is_digit(*current) || *current == '.')
		{
			enqueue(queue, create_qnode(current));

			// kiểm tra nếu chỉ số i bằng độ dài chuỗi biểu thức ==> lấy toàn bộ 
			// kí tự trong queue tạo thành chuỗi và add vào linked list
			if (i == length - 1)
			{
				char* string_number = (char*)calloc(queue->count + 1, sizeof(char));

				dequeue_to_create_an_string_and_add_to_linkedlist(queue, linkedlist);

				while (number_of_open_brackets != 0)
				{
					char* closed_bracket_string = create_string_from_character(&closed_bracket);
					add_last(linkedlist, create_lnode(closed_bracket_string));
					number_of_open_brackets--;
				}
			}
		}
		else if (is_brackets(*current)) // giá trị tại i là brackets
		{
			if (is_open_bracket(*current)) // - nếu là dấu '(' thì add vào linked list.
			{
				if (after_current != NULL)
				{
					char* open_bracket_string = create_string_from_character(current);
					add_last(linkedlist, create_lnode(open_bracket_string));
				}
			}

			// nếu là dấu ')' thì lấy toàn bộ kí tự trong queue tạo thành chuỗi và 
			// add vào linked list sau đó add '(' vào linked list.
			else if (is_closed_bracket(*current))
			{
				dequeue_to_create_an_string_and_add_to_linkedlist(queue, linkedlist);

				// add current vào linked list
				char* current_string = create_string_from_character(current);
				add_last(linkedlist, create_lnode(current_string));

				// add dấu ')' tương đương số lượng dấu ')' khi phát hiện số âm hoặc dương
				while (number_of_open_brackets != 0)
				{
					char* closed_bracket_string = create_string_from_character(&closed_bracket);
					add_last(linkedlist, create_lnode(closed_bracket_string));

					number_of_open_brackets--;
				}
			}
		}

		else if (is_operator(*current))
		{
			// nếu là operator '+' hoặc '-' thì kiểm tra xem có phải là âm hay dương không?
			if (is_negative_or_positive(before_current, current, after_current) && (*current == '+' || *current == '-'))
			{
				// add '(' vào linked list
				char* open_bracket_string = create_string_from_character(&open_bracket);
				add_last(linkedlist, create_lnode(open_bracket_string));
				number_of_open_brackets++;

				// add '0' vào linked list
				char* zero_string = create_string_from_character(&zero);
				add_last(linkedlist, create_lnode(zero_string));

				// add dấu âm hoặc dương vào linked list
				char* current_string = create_string_from_character(current);
				add_last(linkedlist, create_lnode(current_string));
			}
			else // nếu là operator bình thường thì thêm vào linked list.
			{
				dequeue_to_create_an_string_and_add_to_linkedlist(queue, linkedlist);

				if (*current != '^')
				{
					// add dấu ')' tương đương số lượng dấu ')' khi phát hiện số âm hoặc dương
					while (number_of_open_brackets != 0)
					{
						char* closed_bracket_string = create_string_from_character(&closed_bracket);
						add_last(linkedlist, create_lnode(closed_bracket_string));
						number_of_open_brackets--;
					}
				}

				// add current vào linked list
				char* current_string = create_string_from_character(current);
				add_last(linkedlist, create_lnode(current_string));
			}
		}
	}

	return linkedlist;
}

//***********************************************************************
//Main Processing
//***********************************************************************

// function trả về danh sách liên kết infix đã được đảo ngược.
LinkedList* get_infix_reverse(LinkedList* infix)
{
	LinkedList* infix_reverse = initial_linkedlist(); // biểu thức đảo ngược của infix
	Stack* stack = initial_stack();

	// nạp tất cả phần tử trong biểu thức infix vào stack
	for (LNode* i = infix->head; i != NULL; i = i->ptr_next)
		push_stack(stack, create_snode(i->value));

	// xử lý cho đến khi stack rỗng
	while (!is_stack_empty(stack))
	{
		char* value = pop_stack(stack); // lấy giá trị phần tử đầu stack

		if (is_brackets(*value)) // nếu giá trị đó là dấu ngoặc
		{
			LNode* list_node; // danh sách liên kết chứa đựng các node trong cây nhị phân

			if (is_open_bracket(*value)) // nếu là dấu 'ngoặc mở' thì nạp vào linked list là ngoặc đóng
			{
				list_node = create_lnode(")");
				add_last(infix_reverse, list_node);
			}
			else if (is_closed_bracket(*value)) // nếu là dấu 'ngoặc đóng' thì nạp vào linked list là ngoặc mở
			{
				list_node = create_lnode("(");
				add_last(infix_reverse, list_node);
			}
		}
		else // ngược lại không phải dấu ngoặc thì nạp vào sau của linked list.
			add_last(infix_reverse, create_lnode(value));
	}

	return infix_reverse;
}

// function chuyển đổi biểu thức trung tố (infix) sang biểu thức tiền tố (prefix).
LinkedList* infix_to_prefix(LinkedList* infix)
{
	LinkedList* infix_reverse = get_infix_reverse(infix); // biểu thức trung tố reverse
	LinkedList* prefix = initial_linkedlist(); // biểu thức hậu tố
	Stack* stack = initial_stack();

	// lặp qua từng phần tử trong biểu thức trung tố đảo ngược để xử lý
	for (LNode* i = infix_reverse->head; i != NULL; i = i->ptr_next)
	{
		// nếu là operator thì kiểm tra độ ưu tiên giữa operator cần nạp và operator đầu stack: 
		if (is_operator(*i->value))
		{
			if (!is_stack_empty(stack)) // kiểm tra nếu stack hiện tại đang có phần tử thì mới xử lý
			{
				while (!is_stack_empty(stack)) // lặp đến khi stack rỗng
				{
					char* operator_top_stack = peek_stack(stack)->value; // lấy operator đầu stack

					// nếu operator vừa lấy có độ ưu tiên thấp hơn operator đang xử lý 
					// thì pop operator đầu stack ra và nạp vào linked list biểu thức prefix
					if (get_priority(*i->value) < get_priority(*operator_top_stack))
						add_first(prefix, create_lnode(pop_stack(stack)));
					else // nếu có độ ưu tiên cao hơn thì thoát vòng lặp.
						break;
				}
			}

			push_stack(stack, create_snode(i->value)); // nạp operator đang xử lý vào stack.
		}

		else if (is_open_bracket(*i->value)) // nếu là dấu ngoặc mở bình thường thì nạp vào stack.
			push_stack(stack, create_snode(i->value));

		else if (is_operand(i->value)) // nếu là operand thì nạp vào stack.
			add_first(prefix, create_lnode(i->value));

		// nếu là dấu ngoặc đóng thì lấy tất cả phần tử trong stack ra và nạp vào đầu linked list 
		// cho đến khi gặp dấu ngoặc mở đầu tiên.
		else if (is_closed_bracket(*i->value))
		{
			while (1)
			{
				if (!is_stack_empty(stack))
				{
					char* top_stack = pop_stack(stack);

					if (is_operator(*top_stack))
					{
						LNode* list_node = create_lnode(top_stack);
						add_first(prefix, list_node);
					}
					else if (is_open_bracket(*top_stack))
						break;
				}
			}
		}
	}

	// cuối cùng lấy tất cả phần tử còn dư trong stack add vào linked list.
	while (!is_stack_empty(stack))
		add_first(prefix, create_lnode(pop_stack(stack)));

	return prefix; // trả về con trỏ biểu thức prefix có kiểu dữ liệu linked list.
}

// function nạp từng phần tử trong linked list prefix vào cây nhị phân và trả về cây biểu thức.
TNode* create_binary_expression_tree(LinkedList* prefix)
{
	// khai báo & khởi tạo stack để chứa đựng phần tử có kiểu dữ liệu là node của cây nhị phân
	StackTree* stack = initial_stack();

	// khai báo cây nhị phân
	TNode* root = create_tnode(prefix->head->value);

	push_stack_tree(stack, root); // nạp node root của cây vào stack để bắt đầu xử lý

	// lặp qua tất cả phần tử từ biểu thức prefix nhận vào.
	for (LNode* i = prefix->head->ptr_next; i != NULL; i = i->ptr_next)
	{
		TNode* top_of_stack = peek_stack_tree(stack)->tree_node; // lấy node đầu tiên trong stack ra.

		// khai báo & khởi tạo node để nạp vào cây (dữ liệu từ biểu thức prefix)
		TNode* node_want_to_add = create_tnode(i->value);

		if (top_of_stack != NULL) // nếu stack có phần tử thì mới xử lý
		{
			// nếu node đầu stack chưa có con trái hoặc con phải thì insert vào
			if (top_of_stack->ptr_left == NULL)
				insert_left(top_of_stack, node_want_to_add);
			else if (top_of_stack->ptr_right == NULL)
				insert_right(top_of_stack, node_want_to_add);

			// nếu node ở đỉnh stack đã có con trái và con phải thì pop ra
			if (top_of_stack->ptr_right != NULL && top_of_stack->ptr_left != NULL)
				pop_stack_tree(stack);
		}

		// nếu phần tử đang xử lý là operator thì nạp thẳng vào cây
		if (is_operator(*i->value))
			push_stack_tree(stack, node_want_to_add);
	}

	return root; // trả về con trỏ - trỏ tới địa chỉ node root của cây nhị phân
}

// function sử dụng đệ quy để tính toán kết quả từ cây nhị phân
double inorder_to_calculate(TNode* root)
{
	double result = 0;

	if (is_operand(root->value))
		result += atof(root->value);
	else
	{
		double left = inorder_to_calculate(root->ptr_left);
		double right = inorder_to_calculate(root->ptr_right);

		switch (*root->value)
		{
			case '+':
				result = left + right;
				break;
			case '-':
				result = left - right;
				break;
			case '*':
				result = left * right;
				break;
			case 'x':
				result = left * right;
				break;
			case '/':
				result = left / right;
				break;
			case '^':
				result = pow(left, right);
				break;
		}
	}

	return result;
}

int main()
{
	do
	{
		LinkedList* infix = NULL;
		char expression[256];

		printf_s("\n nhap bieu thuc can tinh toan: ");
		gets_s(expression, 256);

		infix = regular_expression(get_expression_is_not_space(expression));

		if (infix == NULL || expression[0] == NULL)
		{
			int again;
			printf_s("\n\n # Continue calculator (1 - yes | 0 - no): ");
			scanf_s("%d", &again);

			if (infix == NULL)
			{
				if (again == 1)
				{
					system("cls");

					char trash;
					scanf_s("%c", &trash, 1);

					continue;
				}
				else if (again == 0)
					exit(0);
				else
					exit(0);
			}
		}

		LinkedList* prefix = infix_to_prefix(infix);
		TNode* root = create_binary_expression_tree(prefix);

		double result = inorder_to_calculate(root);

		print_linkedlist(infix);
		printf_s(" = %.3f", result);

		int again;
		printf_s("\n\n  # Continue calculate (1 - yes | 0 - no): ");
		scanf_s("%d", &again);

		if (again == 1)
		{
			system("cls");

			char trash;
			scanf_s("%c", &trash, 1);

			continue;
		}
		else if (again == 0)
		{
			clear_linkedlist(infix);
			clear_linkedlist(prefix);
			free_tree(root);

			exit(0);
		}
		else
			exit(0);
	} while (1);

	char getch = _getch();
	return 0;
}
