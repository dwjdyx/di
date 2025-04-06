#include<iostream>
#include<stack>
#include<queue>
#include <unordered_set> // 用于存储已存在的值
#include <climits>  // 提供 INT_MIN 和 INT_MAX
#include"BiTree.h"

using namespace std;


// 全局哈希集合，用于记录已存在的节点值
unordered_set<int> existing_values;

//创建二叉树
BiTree* BST_Tree(int min_val , int max_val ) {
	int val;
	while (true) {
		cout << "请输入整型元素(-1表示空节点):";
		cin >> val;

		if (cin.fail()) { // 检测输入是否失败
			cout << "输入无效，请重新输入！" << endl;
			cin.clear(); // 清除错误状态
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略剩余输入
			continue;
		}

		if (val == -1) { // 用 -1 表示空节点
			return nullptr;
		}

		// 检测重复值
		if (existing_values.find(val) != existing_values.end()) {
			cout << "值 " << val << " 已存在，请重新输入！" << endl;
			continue;
		}

		// 检查是否符合 BST 规则
		if (val <= min_val || val >= max_val) {
			cout << "值 " << val << " 不符合 BST 规则（应在 " << min_val << " 和 " << max_val << " 之间）" << endl;
			continue;
		}

		break; // 输入有效且不重复，退出循环
	}

	BiTree* root = new BiNode;
	if (root == nullptr) {
		return nullptr;
	}

	root->data = val;
	existing_values.insert(val); // 将新值加入集合

	cout << "构建左子树（必须 < " << val << "）" << endl;
	root->left = BST_Tree(min_val, val); // 左子树的最大值不能超过当前节点值

	cout << "构建右子树（必须 > " << val << "）" << endl;
	root->right = BST_Tree(val, max_val); // 右子树的最小值不能小于当前节点值

	return root;
}

//插入元素
BiTree* BST_insert(BiTree* root, int val, std::unordered_set<int>& existing_values) {
	// 检查重复值（可选，取决于设计需求）
	if (existing_values.find(val) != existing_values.end()) {
		cout << "值 " << val << " 已存在，插入失败！" << endl;
		return root;
	}

	if (root == nullptr) {
		BiTree* newNode = new BiNode;
		newNode->data = val;
		newNode->left = newNode->right = nullptr;
		existing_values.insert(val); // 同步更新集合
		return newNode;
	}

	if (val < root->data) {
		root->left = BST_insert(root->left, val, existing_values);
	}
	else if (val > root->data) {
		root->right = BST_insert(root->right, val, existing_values);
	}
	// val == root->data 时直接返回（或抛出异常）
	return root;
}


//删除元素
Status BST_delete(BiTree** root_ptr, int value) {
	if (*root_ptr == nullptr) {
		return FAILURE;  // 未找到节点，删除失败
	}

	BiTree* root = *root_ptr;
	if (value < root->data) {
		return BST_delete(&(root->left), value);  // 递归左子树
	}
	else if (value > root->data) {
		return BST_delete(&(root->right), value);  // 递归右子树
	}
	else {  // 找到目标节点
		// 无左子节点
		if (root->left == nullptr) {
			BiTree* temp = root->right;
			delete root;
			*root_ptr = temp;  // 更新父节点指针
			return SUCCESS;
		}
		// 无右子节点
		else if (root->right == nullptr) {
			BiTree* temp = root->left;
			delete root;
			*root_ptr = temp;  // 更新父节点指针
			return SUCCESS;
		}
		// 有左右子树
		else {
			// 找到右子树的最小节点
			BiTree* curr = root->right;
			while (curr->left != nullptr) {
				curr = curr->left;
			}
			// 用最小值覆盖当前节点
			root->data = curr->data;
			// 递归删除右子树中的最小值节点
			return BST_delete(&(root->right), curr->data);
		}
	}
}

//寻找元素
Status BST_search(BiTree* root, int target) {
	BiTree* curr = root;
	while (curr != nullptr) {
		if (target == curr->data) {
			cout << "已找到元素 " << target << endl;
			return SUCCESS;
		}
		curr = (target < curr->data) ? curr->left : curr->right;
	}
	cout << "未找到元素 " << target << endl;
	return FAILURE;
}



//先序遍历(递归)
Status BST_preorderI(BiTree* root) {
	if (root == nullptr) return FAILURE;
	cout << root->data << " ";
	Status left = BST_preorderI(root->left);
	Status right = BST_preorderI(root->right);
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//非递归
Status BST_preorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // 空树直接返回
	stack<BiTreePtr> s;
	s.push(root);  // 根节点入栈

	while (!s.empty()) {
		BiTreePtr curr = s.top();  // 取出栈顶节点
		s.pop();
		cout << curr->data << " ";  // 访问当前节点

		// 右子节点先入栈（后进先出，保证左子先处理）
		if (curr->right != nullptr) {
			s.push(curr->right);
		}
		if (curr->left != nullptr) {
			s.push(curr->left);
		}
	}
	return SUCCESS;
}

//中序遍历
Status BST_inorderI(BiTree* root)
{
	if (root == nullptr) return FAILURE;
	Status left = BST_inorderI(root->left);
	cout << root->data << " ";
	Status right = BST_inorderI(root->right);
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//非递归
Status BST_inorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // 空树直接返回
	stack<BiTreePtr> s;
	BiTreePtr curr = root;				//curr为当前节点

	while (curr != nullptr || !s.empty()) {
		// 将当前节点及其所有左子节点入栈
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->left;
		}

		// 访问栈顶节点（最深的左子节点）
		curr = s.top();
		s.pop();
		std::cout << curr->data << " ";

		// 转向右子树
		curr = curr->right;
	}
	return SUCCESS;
}

//后序遍历
Status BST_postorderI(BiTree* root)
{
	if (root == nullptr) return FAILURE;
	Status left = BST_postorderI(root->left);
	Status right = BST_postorderI(root->right);
	cout << root->data << " ";
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//非递归
Status BST_postorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // 空树直接返回
	stack<BiTreePtr> s;
	BiTreePtr curr = root;				//curr为当前节点
	BiTreePtr pre = root;				//上一访问的节点

	while (curr != nullptr || !s.empty()) {
		// 将当前节点及其所有左子节点入栈
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->left;
		}

		// 访问栈顶节点（最深的左子节点）
		curr = s.top();
		//如果栈顶元素有右子树，且并未访问
		if (curr->right && curr->right != pre)
		{
			curr = curr->right;
		}
		else {									//子树为空或被访问过
			cout << curr->data << " ";			//访问节点元素
			pre = curr;							//记录当前访问的节点
			curr = nullptr;						//防止重复访问左子树
			s.pop();							//出栈
		}
	}
	return SUCCESS;
}

//层次遍历
Status BST_levelOrder(BiTree* root)
{
	if (root == nullptr) return FAILURE;	//树为空直接退出
	queue<BiTreePtr> q;
	q.push(root);  // 根节点入队

	while (!q.empty()) {
		BiTreePtr curr = q.front();  // 取出队首节点
		q.pop();
		std::cout << curr->data << " ";  // 访问当前节点

		// 左子节点入队（如果存在）
		if (curr->left != nullptr) {
			q.push(curr->left);
		}
		// 右子节点入队（如果存在）
		if (curr->right != nullptr) {
			q.push(curr->right);
		}
	}
	return SUCCESS;
}

// 释放二叉树内存
void FreeTree(BiTree* root) {
	if (root == nullptr) {
		return;
	}
	FreeTree(root->left);
	FreeTree(root->right);
	delete root;
}

void Print()
{
	printf("---------------------------------------\n");
	printf("1.插入						2.删除\n");
	printf("3.先序遍历					4.中序遍历\n");
	printf("5.后序遍历					6.层次遍历\n");
	printf("7.搜索						8.退出   \n");
	printf("--------------------------------------\n");
}

int main()
{
	BiTree* root=BST_Tree();
	Print();
	int a;
	do
	{
		cout<<"\n";
		cout<<"请选择操作:";
		cin >> a;
		switch (a) {
		case(1): {
			int val;
			while (true) {
				cout << "请输入插入的整型元素: ";
				cin >> val;

				if (cin.fail()) {  // 检测输入是否失败
					cout << "输入无效，请重新输入！" << endl;
					cin.clear();  // 清除错误状态
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余输入
				}
				else {
					break;  // 输入有效，退出循环
				}
			}
			root = BST_insert(root, val, existing_values);
			break;
		}
		case(2): {
			int value;
			while (true) {
				cout << "请输入需要删除的整型元素:";
				cin >> value;
				if (cin.fail()) {  // 检测输入是否失败
					cout << "输入无效，请重新输入！" << endl;
					cin.clear();  // 清除错误状态
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余输入
				}
				else {
					break;  // 输入有效，退出循环
				}
			}
			if (BST_delete(&root, value) == SUCCESS) {
				std::cout << "成功删除节点 " << value << std::endl;
			}
			else {
				std::cout << "未找到节点 " << value << std::endl;
			}
			break;
		}
		case(3): {
			cout << "先序遍历递归" << endl;
			BST_preorderI(root);
			cout << endl << "非递归" << endl;
			BST_preorderR(root);
			break;
		}
		case(4): {
			cout << "中序遍历递归" << endl;
			BST_inorderI(root);
			cout << endl << "非递归" << endl;
			BST_inorderR(root);
			break;
		}
		case(5): {
			cout << "后序遍历递归" << endl;
			BST_postorderI(root);
			cout << endl << "非递归" << endl;
			BST_postorderR(root);
			break;
		}
		case(6): {
			cout << "层次遍历" << endl;
			BST_levelOrder(root);
			break;
		}
		case(7): {
			int value;
			while (true) {
				cout << "请输入需要查找的整型元素:";
				cin >> value;
				if (cin.fail()) {  // 检测输入是否失败
					cout << "输入无效，请重新输入！" << endl;
					cin.clear();  // 清除错误状态
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余输入
				}
				else {
					break;  // 输入有效，退出循环
				}
			}
			BST_search(root,value);
			break;
		}
		case(8):cout<<"退出\n"; break;
		default:cout<<"输入错误！请重新输入";
		}
	} while (a != 8);

	//释放空间
	FreeTree(root);
}