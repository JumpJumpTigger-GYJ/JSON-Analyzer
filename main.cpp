#include<iostream>
#include<fstream>
#include<sstream>
#include<memory>
#include<algorithm>
#include<unordered_map>
#include"json.h"
#include"parser.h"

using namespace std;
using namespace gyj::json;

class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};

class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        Node* head = nullptr, * pre = nullptr;
        dfs(root, pre, head);
        if (head) {
            pre->left = head;
            head->right = pre;
        }
        return head;
    }

    void dfs(Node* root, Node*& pre, Node*& head) {
        if (!root) {
            return;
        }
        dfs(root->left, pre, head);
        root->left = pre;
        if (pre) {
            pre->right = root;
        }
        else {
            head = root;
        }
        pre = root;
        dfs(root->right, pre, head);
    }
};

int main() {
    Node* head = new Node(4);
    head->left = new Node(2);
    head->left->left = new Node(1);
    head->left->right = new Node(3);
    head->right = new Node(5);
    Solution s;
    s.treeToDoublyList(head);

	return 0;
}