#include<iostream>
#include<stack>
#include<queue>
#include <unordered_set> // ���ڴ洢�Ѵ��ڵ�ֵ
#include <climits>  // �ṩ INT_MIN �� INT_MAX
#include"BiTree.h"

using namespace std;


// ȫ�ֹ�ϣ���ϣ����ڼ�¼�Ѵ��ڵĽڵ�ֵ
unordered_set<int> existing_values;

//����������
BiTree* BST_Tree(int min_val , int max_val ) {
	int val;
	while (true) {
		cout << "����������Ԫ��(-1��ʾ�սڵ�):";
		cin >> val;

		if (cin.fail()) { // ��������Ƿ�ʧ��
			cout << "������Ч�����������룡" << endl;
			cin.clear(); // �������״̬
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ����ʣ������
			continue;
		}

		if (val == -1) { // �� -1 ��ʾ�սڵ�
			return nullptr;
		}

		// ����ظ�ֵ
		if (existing_values.find(val) != existing_values.end()) {
			cout << "ֵ " << val << " �Ѵ��ڣ����������룡" << endl;
			continue;
		}

		// ����Ƿ���� BST ����
		if (val <= min_val || val >= max_val) {
			cout << "ֵ " << val << " ������ BST ����Ӧ�� " << min_val << " �� " << max_val << " ֮�䣩" << endl;
			continue;
		}

		break; // ������Ч�Ҳ��ظ����˳�ѭ��
	}

	BiTree* root = new BiNode;
	if (root == nullptr) {
		return nullptr;
	}

	root->data = val;
	existing_values.insert(val); // ����ֵ���뼯��

	cout << "���������������� < " << val << "��" << endl;
	root->left = BST_Tree(min_val, val); // �����������ֵ���ܳ�����ǰ�ڵ�ֵ

	cout << "���������������� > " << val << "��" << endl;
	root->right = BST_Tree(val, max_val); // ����������Сֵ����С�ڵ�ǰ�ڵ�ֵ

	return root;
}

//����Ԫ��
BiTree* BST_insert(BiTree* root, int val, std::unordered_set<int>& existing_values) {
	// ����ظ�ֵ����ѡ��ȡ�����������
	if (existing_values.find(val) != existing_values.end()) {
		cout << "ֵ " << val << " �Ѵ��ڣ�����ʧ�ܣ�" << endl;
		return root;
	}

	if (root == nullptr) {
		BiTree* newNode = new BiNode;
		newNode->data = val;
		newNode->left = newNode->right = nullptr;
		existing_values.insert(val); // ͬ�����¼���
		return newNode;
	}

	if (val < root->data) {
		root->left = BST_insert(root->left, val, existing_values);
	}
	else if (val > root->data) {
		root->right = BST_insert(root->right, val, existing_values);
	}
	// val == root->data ʱֱ�ӷ��أ����׳��쳣��
	return root;
}


//ɾ��Ԫ��
Status BST_delete(BiTree** root_ptr, int value) {
	if (*root_ptr == nullptr) {
		return FAILURE;  // δ�ҵ��ڵ㣬ɾ��ʧ��
	}

	BiTree* root = *root_ptr;
	if (value < root->data) {
		return BST_delete(&(root->left), value);  // �ݹ�������
	}
	else if (value > root->data) {
		return BST_delete(&(root->right), value);  // �ݹ�������
	}
	else {  // �ҵ�Ŀ��ڵ�
		// �����ӽڵ�
		if (root->left == nullptr) {
			BiTree* temp = root->right;
			delete root;
			*root_ptr = temp;  // ���¸��ڵ�ָ��
			return SUCCESS;
		}
		// �����ӽڵ�
		else if (root->right == nullptr) {
			BiTree* temp = root->left;
			delete root;
			*root_ptr = temp;  // ���¸��ڵ�ָ��
			return SUCCESS;
		}
		// ����������
		else {
			// �ҵ�����������С�ڵ�
			BiTree* curr = root->right;
			while (curr->left != nullptr) {
				curr = curr->left;
			}
			// ����Сֵ���ǵ�ǰ�ڵ�
			root->data = curr->data;
			// �ݹ�ɾ���������е���Сֵ�ڵ�
			return BST_delete(&(root->right), curr->data);
		}
	}
}

//Ѱ��Ԫ��
Status BST_search(BiTree* root, int target) {
	BiTree* curr = root;
	while (curr != nullptr) {
		if (target == curr->data) {
			cout << "���ҵ�Ԫ�� " << target << endl;
			return SUCCESS;
		}
		curr = (target < curr->data) ? curr->left : curr->right;
	}
	cout << "δ�ҵ�Ԫ�� " << target << endl;
	return FAILURE;
}



//�������(�ݹ�)
Status BST_preorderI(BiTree* root) {
	if (root == nullptr) return FAILURE;
	cout << root->data << " ";
	Status left = BST_preorderI(root->left);
	Status right = BST_preorderI(root->right);
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//�ǵݹ�
Status BST_preorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // ����ֱ�ӷ���
	stack<BiTreePtr> s;
	s.push(root);  // ���ڵ���ջ

	while (!s.empty()) {
		BiTreePtr curr = s.top();  // ȡ��ջ���ڵ�
		s.pop();
		cout << curr->data << " ";  // ���ʵ�ǰ�ڵ�

		// ���ӽڵ�����ջ������ȳ�����֤�����ȴ���
		if (curr->right != nullptr) {
			s.push(curr->right);
		}
		if (curr->left != nullptr) {
			s.push(curr->left);
		}
	}
	return SUCCESS;
}

//�������
Status BST_inorderI(BiTree* root)
{
	if (root == nullptr) return FAILURE;
	Status left = BST_inorderI(root->left);
	cout << root->data << " ";
	Status right = BST_inorderI(root->right);
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//�ǵݹ�
Status BST_inorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // ����ֱ�ӷ���
	stack<BiTreePtr> s;
	BiTreePtr curr = root;				//currΪ��ǰ�ڵ�

	while (curr != nullptr || !s.empty()) {
		// ����ǰ�ڵ㼰���������ӽڵ���ջ
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->left;
		}

		// ����ջ���ڵ㣨��������ӽڵ㣩
		curr = s.top();
		s.pop();
		std::cout << curr->data << " ";

		// ת��������
		curr = curr->right;
	}
	return SUCCESS;
}

//�������
Status BST_postorderI(BiTree* root)
{
	if (root == nullptr) return FAILURE;
	Status left = BST_postorderI(root->left);
	Status right = BST_postorderI(root->right);
	cout << root->data << " ";
	return (left == SUCCESS && right == SUCCESS) ? SUCCESS : FAILURE;
}

//�ǵݹ�
Status BST_postorderR(BiTree* root)
{
	if (root == nullptr) return FAILURE;  // ����ֱ�ӷ���
	stack<BiTreePtr> s;
	BiTreePtr curr = root;				//currΪ��ǰ�ڵ�
	BiTreePtr pre = root;				//��һ���ʵĽڵ�

	while (curr != nullptr || !s.empty()) {
		// ����ǰ�ڵ㼰���������ӽڵ���ջ
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->left;
		}

		// ����ջ���ڵ㣨��������ӽڵ㣩
		curr = s.top();
		//���ջ��Ԫ�������������Ҳ�δ����
		if (curr->right && curr->right != pre)
		{
			curr = curr->right;
		}
		else {									//����Ϊ�ջ򱻷��ʹ�
			cout << curr->data << " ";			//���ʽڵ�Ԫ��
			pre = curr;							//��¼��ǰ���ʵĽڵ�
			curr = nullptr;						//��ֹ�ظ�����������
			s.pop();							//��ջ
		}
	}
	return SUCCESS;
}

//��α���
Status BST_levelOrder(BiTree* root)
{
	if (root == nullptr) return FAILURE;	//��Ϊ��ֱ���˳�
	queue<BiTreePtr> q;
	q.push(root);  // ���ڵ����

	while (!q.empty()) {
		BiTreePtr curr = q.front();  // ȡ�����׽ڵ�
		q.pop();
		std::cout << curr->data << " ";  // ���ʵ�ǰ�ڵ�

		// ���ӽڵ���ӣ�������ڣ�
		if (curr->left != nullptr) {
			q.push(curr->left);
		}
		// ���ӽڵ���ӣ�������ڣ�
		if (curr->right != nullptr) {
			q.push(curr->right);
		}
	}
	return SUCCESS;
}

// �ͷŶ������ڴ�
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
	printf("1.����						2.ɾ��\n");
	printf("3.�������					4.�������\n");
	printf("5.�������					6.��α���\n");
	printf("7.����						8.�˳�   \n");
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
		cout<<"��ѡ�����:";
		cin >> a;
		switch (a) {
		case(1): {
			int val;
			while (true) {
				cout << "��������������Ԫ��: ";
				cin >> val;

				if (cin.fail()) {  // ��������Ƿ�ʧ��
					cout << "������Ч�����������룡" << endl;
					cin.clear();  // �������״̬
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ����ʣ������
				}
				else {
					break;  // ������Ч���˳�ѭ��
				}
			}
			root = BST_insert(root, val, existing_values);
			break;
		}
		case(2): {
			int value;
			while (true) {
				cout << "��������Ҫɾ��������Ԫ��:";
				cin >> value;
				if (cin.fail()) {  // ��������Ƿ�ʧ��
					cout << "������Ч�����������룡" << endl;
					cin.clear();  // �������״̬
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ����ʣ������
				}
				else {
					break;  // ������Ч���˳�ѭ��
				}
			}
			if (BST_delete(&root, value) == SUCCESS) {
				std::cout << "�ɹ�ɾ���ڵ� " << value << std::endl;
			}
			else {
				std::cout << "δ�ҵ��ڵ� " << value << std::endl;
			}
			break;
		}
		case(3): {
			cout << "��������ݹ�" << endl;
			BST_preorderI(root);
			cout << endl << "�ǵݹ�" << endl;
			BST_preorderR(root);
			break;
		}
		case(4): {
			cout << "��������ݹ�" << endl;
			BST_inorderI(root);
			cout << endl << "�ǵݹ�" << endl;
			BST_inorderR(root);
			break;
		}
		case(5): {
			cout << "��������ݹ�" << endl;
			BST_postorderI(root);
			cout << endl << "�ǵݹ�" << endl;
			BST_postorderR(root);
			break;
		}
		case(6): {
			cout << "��α���" << endl;
			BST_levelOrder(root);
			break;
		}
		case(7): {
			int value;
			while (true) {
				cout << "��������Ҫ���ҵ�����Ԫ��:";
				cin >> value;
				if (cin.fail()) {  // ��������Ƿ�ʧ��
					cout << "������Ч�����������룡" << endl;
					cin.clear();  // �������״̬
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ����ʣ������
				}
				else {
					break;  // ������Ч���˳�ѭ��
				}
			}
			BST_search(root,value);
			break;
		}
		case(8):cout<<"�˳�\n"; break;
		default:cout<<"�����������������";
		}
	} while (a != 8);

	//�ͷſռ�
	FreeTree(root);
}