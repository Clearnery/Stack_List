#include "Stack.h"
#include <string>
#include <vector>
#include <gtest.h>
#include <fstream>
#include <sstream>




TEST(TStackTest, DefaultConstructor) 
{
    EXPECT_NO_THROW({
        TStack<int> stack;
        EXPECT_TRUE(stack.IsEmpty());
        EXPECT_EQ(stack.GetLen(), 0);
        });
}


TEST(TStackTest, SimplePushPop) 
{
    TStack<int> stack;


    EXPECT_NO_THROW(stack.Push(10));
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_EQ(stack.GetLen(), 1);

    EXPECT_NO_THROW(stack.Push(20));
    EXPECT_EQ(stack.GetLen(), 2);


    int value = 0;
    EXPECT_NO_THROW(value = stack.Pop());
    EXPECT_EQ(value, 20);
    EXPECT_EQ(stack.GetLen(), 1);

    EXPECT_NO_THROW(value = stack.Pop());
    EXPECT_EQ(value, 10);
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_EQ(stack.GetLen(), 0);
}

TEST(TStackTest, CopyConstructor) 
{
    TStack<int> stack1;
    stack1.Push(1);
    stack1.Push(2);
    stack1.Push(3);

    
    TStack<int> stack2(stack1);

    EXPECT_EQ(stack1.GetLen(), 3);
    EXPECT_EQ(stack2.GetLen(), 3);

    
    EXPECT_NO_THROW({
        EXPECT_EQ(stack2.Pop(), 3);
        EXPECT_EQ(stack2.Pop(), 2);
        EXPECT_EQ(stack2.Pop(), 1);
        });

    
    EXPECT_EQ(stack1.GetLen(), 3);
}


TEST(TStackTest, AssignmentOperator) 
{
    TStack<int> stack1;
    stack1.Push(10);
    stack1.Push(20);

    TStack<int> stack2;
    stack2.Push(30);


    EXPECT_NO_THROW({
        stack2 = stack1;
        });

    EXPECT_EQ(stack1.GetLen(), 2);
    EXPECT_EQ(stack2.GetLen(), 2);

    EXPECT_EQ(stack2.Pop(), 20);
    EXPECT_EQ(stack2.Pop(), 10);
    EXPECT_TRUE(stack2.IsEmpty());

    EXPECT_EQ(stack1.GetLen(), 2);
}

TEST(TStackTest, EqualityOperator) 
{
    TStack<int> stack1;
    TStack<int> stack2;

    EXPECT_TRUE(stack1 == stack2);

    stack1.Push(1);
    stack1.Push(2);
    stack1.Push(3);

    stack2.Push(1);
    stack2.Push(2);
    stack2.Push(3);

    EXPECT_TRUE(stack1 == stack2);

    stack1.Pop();
    EXPECT_FALSE(stack1 == stack2);

    stack1.Push(3);
    EXPECT_TRUE(stack1 == stack2);
}


TEST(TStackTest, InequalityOperator) 
{
    TStack<int> stack1;
    TStack<int> stack2;

    EXPECT_FALSE(stack1 != stack2);

    stack1.Push(1);
    EXPECT_TRUE(stack1 != stack2);

    stack2.Push(2);
    EXPECT_TRUE(stack1 != stack2);
}

TEST(TStackTest, MultipleElements) 
{
    TStack<int> stack;
    const int COUNT = 100;

    for (int i = 0; i < COUNT; ++i) 
    {
        EXPECT_NO_THROW(stack.Push(i));
        EXPECT_EQ(stack.GetLen(), i + 1);
    }

    for (int i = COUNT - 1; i >= 0; --i) 
    {
        EXPECT_EQ(stack.Pop(), i);
        EXPECT_EQ(stack.GetLen(), i);
    }

    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, DifferentDataTypes) 
{
    {
        TStack<std::string> stack;
        stack.Push("hello");
        stack.Push("world");

        EXPECT_EQ(stack.Pop(), "world");
        EXPECT_EQ(stack.Pop(), "hello");
    }

    {
        TStack<double> stack;
        stack.Push(3.14159);
        stack.Push(2.71828);

        EXPECT_DOUBLE_EQ(stack.Pop(), 2.71828);
        EXPECT_DOUBLE_EQ(stack.Pop(), 3.14159);
    }
}

TEST(TStackTest, SelfAssignment) 
{
    TStack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    EXPECT_NO_THROW({
        stack = stack;
        });

    EXPECT_EQ(stack.GetLen(), 3);
    EXPECT_EQ(stack.Pop(), 3);
    EXPECT_EQ(stack.Pop(), 2);
    EXPECT_EQ(stack.Pop(), 1);
}

TEST(TStackTest, ClearViaPop) 
{
    TStack<int> stack;

    for (int i = 0; i < 10; ++i) 
    {
        stack.Push(i);
    }

    while (!stack.IsEmpty()) 
    {
        EXPECT_NO_THROW(stack.Pop());
    }

    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_EQ(stack.GetLen(), 0);

    stack.Push(100);
    EXPECT_EQ(stack.Pop(), 100);
}

TEST(TStackTest, OperationChain) 
{
    TStack<int> stack;

    stack.Push(1);
    stack.Push(2);
    EXPECT_EQ(stack.Pop(), 2);
    stack.Push(3);
    EXPECT_EQ(stack.Pop(), 3);
    EXPECT_EQ(stack.Pop(), 1);
    EXPECT_TRUE(stack.IsEmpty());

    for (int i = 0; i < 5; ++i) 
    {
        stack.Push(i * 10);
    }

    EXPECT_EQ(stack.GetLen(), 5);
}

TEST(TStackTest, ReuseAfterEmpty) 
{
    TStack<int> stack;

    for (int cycle = 0; cycle < 3; ++cycle) 
    {
        for (int i = 0; i < 5; ++i) 
        {
            stack.Push(cycle * 100 + i);
        }

        for (int i = 4; i >= 0; --i) 
        {
            EXPECT_EQ(stack.Pop(), cycle * 100 + i);
        }

        EXPECT_TRUE(stack.IsEmpty());
    }
}