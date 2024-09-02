#include<iostream>
#include"RedBlackTree.h"

int main()
{
    RedBlackTree* t = new(RedBlackTree);
    t->insert(15);
    t->insert(9);
    t->insert(18);
    t->insert(6);
    t->insert(13);
    t->insert(10);
    t->insert(17);
    t->insert(27);
    t->insert(23);
    t->insert(34);
    t->insert(25);
    t->insert(37);
    t->inorder();
    t->delNode(18);
    t->delNode(25);
    t->delNode(15);
    t->delNode(6);
    t->delNode(13);
    t->delNode(37);
    t->delNode(27);
    t->delNode(17);
    t->delNode(34);
    t->delNode(9);
    t->delNode(10);
    t->delNode(23);
    t->inorder();
}