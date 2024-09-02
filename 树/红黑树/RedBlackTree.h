#ifndef REDBLACKTREE
#include<iostream>
enum Color{red,black};
class Node
{
public:
    int data;
    Color color;
    Node *left,*right,*parent;
    Node(int data){
        this->data = data;
        left = right = parent = nullptr;
        color = red;        // 节点默认为红色
    }

};

class RedBlackTree
{
private:
    Node *root; // 根节点
    void fixViolation(Node *&node);
    void leftRotate(Node *&node);
    void rightRotate(Node *&node);
    void inorderHelper(Node *&node);
    void translate(Node *u,Node *v);
    void fixDelete(Node *node,Node *parent);
    Node* minnum(Node *node);
public:
    RedBlackTree():root(nullptr){};
    void insert(const int &data);
    void delNode(const int &data);
    void inorder();
};

// 插入方法
void RedBlackTree::insert(const int &data)
{
    Node *node = new Node(data);
    if(root == nullptr)
    {
        node->color = black;
        root = node;
    }
    else
    {
        Node *current = root;
        Node *parent = nullptr;

        while(current != nullptr)
        {
            parent = current;
            if(node->data < current->data)
            {
                current = current ->left;
            }
            else
            {
                current = current->right;
            }
        }

        node->parent = parent;
        if(node->data < parent->data)
        {
            parent->left = node;
        }
        else if(node->data > parent->data)
        {
            parent->right = node;
        }
        else
        {
            // 红黑树不允许有相同的值
            return;
        }
         fixViolation(node);
    }
}

// 修正节点
void RedBlackTree::fixViolation(Node *&node)
{
    Node *parent = nullptr;
    Node *grandfather = nullptr;
    // 如果节点和父节点都为红色，需要修正
    while((node != root) && node->color == red && node->parent->color == red)
    {
        parent = node->parent;
        grandfather = parent->parent;
        if(parent == grandfather->left)
        {
            // 父节点和叔叔节点都为红色
            if (grandfather->right != nullptr && grandfather->right->color == red)
            {   
                // 将爷爷节点、父亲节点、叔叔节点全变色
                grandfather->color = red;
                parent->color = black;
                grandfather->right->color = black;
                node = grandfather;
            }
            else
            {
                // 父亲节点是红色，叔叔节点是黑色
                if(node == parent->right)
                {
                    leftRotate(parent);
                    node = parent;
                    parent = node->parent;
                }
                rightRotate(grandfather);
                std::swap(parent->color,grandfather->color);
                node = parent;
            }

        }
        else
        {   
            if(grandfather->left != nullptr && grandfather->left->color == red )
            {
                grandfather->color = red;
                parent->color = black;
                grandfather->left->color = black;
                node = grandfather;
            }
            else
            {
                if(node == parent->left)
                {
                    rightRotate(parent);
                    node = parent;
                    parent = node->parent; 
                }
                leftRotate(grandfather);
                std::swap(parent->color,grandfather->color);
                node = parent;
            }
        }
        
    }
    root->color = black;
}

// 左旋
void RedBlackTree::leftRotate(Node *&node)
{
    Node *rightchildren = node->right;

    node->right = rightchildren->left;
    if(node->right != nullptr)
    {
        node->right->parent = node;
    }

    rightchildren->parent = node->parent;
    if(node == root)
    {
        root = rightchildren;
    }
    else if(node == node->parent->left)
    {
        node->parent->left = rightchildren; 
    }
    else
    {
        node->parent->right = rightchildren;
    }

    rightchildren->left = node;
    node->parent = rightchildren;
}

// 右旋
void RedBlackTree::rightRotate(Node *&node)
{
    Node *leftchildren = node->left;
    node->left = leftchildren->right;
    if(node->left != nullptr)
    {
        node->left->parent = node;
    }

    leftchildren->parent = node->parent;
    if(node == root)
    {
        root = leftchildren;
    }
    else if(node == root->left)
    {
        node->parent->left = leftchildren;
    }
    else
    {
        node->parent->right = leftchildren;
    }
    leftchildren->right = node;
    node->parent = leftchildren;    
}

void RedBlackTree::inorderHelper(Node *&node)
{
    if(node == nullptr)
    {
        return;
    }

    inorderHelper(node->left);
    std::cout << node->data << " ";
    inorderHelper(node->right);
}
void RedBlackTree::inorder()
{
    inorderHelper(root);
    std::cout << std::endl;
}

void RedBlackTree::translate(Node* u,Node *v)
{
    if(u->parent == nullptr)
    {
        root = v;
    }
    else if(u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }

    if(v != nullptr)
    {
        v->parent = u->parent;
    }
}

void RedBlackTree::delNode(const int &data)
{
    Node *child = NULL;
    Node *parent = NULL;
    Color color;

    Node *node = root;
    while(node)
    {
        if(node->data == data)
        {
            break;
        }
        else if(node->data < data)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    // 删除节点的左右孩子都有
    if(node->left && node->right)
    {
        // 找到后继节点
        Node *replace = minnum(node->right);
        if(node->parent)
        {
            if(node == node->parent->left)
            {
                node->parent->left = replace;
            }
            else
            {
                node->parent->right = replace;
            }
        }
        else
        {
            root = replace;
        }
        child = replace->right;
        parent = replace->parent;
        color = replace->color;
        if(parent == node)
        {
            parent = replace;
        }
        else
        {
            if(child)
            {
                child->parent = parent;
            }
            parent->left = child;
            replace->right = node->right;
            node->right->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if(color == black)
        {
            fixDelete(child,parent);
        }
        delete node;
        node = nullptr;
        return;
    }
    
    if(node->left)
    {
        child = node->left;
    }
    else
    {
        child = node->right;
    }
    parent = node->parent;
    color = node->color;
    if(child)
    {
        child->parent = parent;
    }

    if(parent)
    {
        if(node == parent->left)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }
    else
    {
        root = child;
    }
    if(color == black)
    {
        fixDelete(child,parent);
    }
    delete node;
    node = nullptr;
}

void RedBlackTree::fixDelete(Node *node,Node *parent)
{
    while((!node || node->color == black) && node != root)
    {

        if(node == parent->left)
        {
            // 兄弟节点
            Node *s = parent->right;
            // 兄弟节点为红色
            if(s->color == red)
            {
                s->color = black;
                parent->color = red;
                leftRotate(parent);
                s = parent->right;
            }

            // 兄弟节点为黑，且两个孩子都为黑色
            if((s->left == nullptr || s->left->color == black) && (s->right == nullptr || s->right->color == black))
            {
                s->color = red;  // 兄弟变色
                node = parent; // 双黑上移
                parent = node->parent;
            }
            else
            {
                // 兄弟节点为黑，且兄弟右孩子为黑，左孩子为红 RL型
                if(s->right == nullptr || s->right->color == black)
                {
                    if(s->left != nullptr)
                    {
                        s->left->color = black;
                    }
                    s->color = red;
                    rightRotate(s);
                    s = parent->right;
                }

                // 兄弟节点为黑，且兄弟的右孩子为红
                s->color = parent->color;
                parent->color = black;
                if(s->right != nullptr)
                {
                    s->right->color = black;
                }
                leftRotate(parent);
                node = root; // 终止循环
            }
        }
        else
        {
            Node *s = parent->left;
            if(s->color == red)
            {
                s->color = black;
                parent->color = red;
                leftRotate(parent);
                s = node->parent->left;
            }
            // 兄弟为黑且两个孩子都为黑
            if((s->left == nullptr || s->left->color == black) && (s->right == nullptr || s->right->color == black))
            {
                s->color = red;
                node = parent;
                parent = node->parent;
            }
            else
            {
                // 兄弟为黑右孩子为红
                if(s->left == nullptr || s->left->color == black)
                {
                    if(s->right != nullptr)
                    {
                        s->right->color = black;
                    }
                    s->color = red;
                    leftRotate(s);
                    s = parent->left;
                }
                // 兄弟为黑左孩子为红
                s->color = s->parent->color;
                parent->color = black;
                if(s->left != nullptr)
                {
                    s->left->color = black;
                }
                rightRotate(parent);
                node = root;
            }
        }
       
    }
    if(node != nullptr)
    {
        node->color = black;
    }
}

Node *RedBlackTree::minnum(Node *node)
{
    while(node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

#endif
