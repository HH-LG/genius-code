// 一个简单的多元求导函数实现
#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    virtual int evaluate(vector<int> x) = 0;
    virtual Node* derivative(int id) = 0;
};

class ConstantNode : public Node {
public:
    int value;

    ConstantNode(int value) : value(value) {}

    int evaluate(vector<int> x) {
        return value;
    }

    Node* derivative(int id) {
        return new ConstantNode(0);
    }
};

class VariableNode : public Node {
public:
    int id;

    VariableNode(int id) : id(id) {}
    int evaluate(vector<int> x) {
        return x[id - 1];
    }

    Node* derivative(int id) {
        if (id == this->id) {
            return new ConstantNode(1);
        }
        return new ConstantNode(0);
    }
};

class AddNode : public Node {
public:
    Node* left;
    Node* right;

    AddNode(Node* left, Node* right) : left(left), right(right) {}

    int evaluate(vector<int> x) {
        return left->evaluate(x) + right->evaluate(x);
    }

    Node* derivative(int id) {
        return new AddNode(left->derivative(id), right->derivative(id));
    }
};

class SubtractNode : public Node {
public:
    Node* left;
    Node* right;

    SubtractNode(Node* left, Node* right) : left(left), right(right) {}

    int evaluate(vector<int> x) {
        return left->evaluate(x) - right->evaluate(x);
    }

    Node* derivative(int id) {
        return new SubtractNode(left->derivative(id), right->derivative(id));
    }
};

class MultiplyNode : public Node {
public:
    Node* left;
    Node* right;

    MultiplyNode(Node* left, Node* right) : left(left), right(right) {}

    int evaluate(vector<int> x) {
        return left->evaluate(x) * right->evaluate(x);
    }

    Node* derivative(int id) {
        return new AddNode(
            new MultiplyNode(left->derivative(id), right),
            new MultiplyNode(left, right->derivative(id))
        );
    }
};

int main() {
    stack<Node*> st;
    int id_max, m;
    string line;
    getline(cin, line);
    stringstream iss(line);
    string token;
    iss >> id_max >> m;
    getline(cin, line);
    iss = stringstream(line);
    while (iss >> token) {
        if (token == "+") {
            Node* right = st.top(); st.pop();
            Node* left = st.top(); st.pop();
            st.push(new AddNode(left, right));
        } else if (token == "-") {
            Node* right = st.top(); st.pop();
            Node* left = st.top(); st.pop();
            st.push(new SubtractNode(left, right));
        } else if (token == "*") {
            Node* right = st.top(); st.pop();
            Node* left = st.top(); st.pop();
            st.push(new MultiplyNode(left, right));
        } else if (token[0] == 'x') {
            st.push(new VariableNode(token[1] - '0'));
            if (token[1] - '0' >= id_max) {
                id_max = token[1] - '0';
            }
        } else {
            st.push(new ConstantNode(stoi(token)));
        }
    }

    Node* f = st.top();
    int var;
    vector<int> x(id_max), res;
    res.reserve(m);
    for (int i = 0; i < m; i++)
    {
        cin >> var;
        var = var;
        for (int j = 0; j < id_max; j++) {
            cin >> x[j];
        }
        res.push_back(f->derivative(var)->evaluate(x));
    }

    for (int i = 0; i < m; i++)
    {
        cout << res[i] << endl;
    }

    return 0;
}