// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>

#include <header.hpp>

TEST(UnrolledLinkedList, Push_back) {
  unrolled_linked_list<int, 5> ullist;

  for (int i = 1; i < 8; i++) ullist.push_back(i);

  EXPECT_EQ(ullist.front(), 1);
  EXPECT_EQ(ullist.back(), 7);

  EXPECT_EQ(ullist[0], ullist.front());
  EXPECT_EQ(ullist[1], 2);
  EXPECT_EQ(ullist[2], 3);
  EXPECT_EQ(ullist[ullist.size() - 1], ullist.back());
  EXPECT_EQ(ullist.getNode(), 2);

  ullist.pop_back();
  ullist.pop_back();

  EXPECT_EQ(ullist[ullist.size() - 1], 5);
  EXPECT_EQ(ullist.getNode(), 1);
}

TEST(UnrolledLinkedList, Copy) {
  unrolled_linked_list<int, 5> ullist;

  for (int i = 1; i < 8; i++) ullist.push_back(i);

  unrolled_linked_list<int, 5> ullist0 = ullist;
  unrolled_linked_list<int, 5> ullist1(ullist);

  EXPECT_TRUE(ullist == ullist0);
  EXPECT_TRUE(ullist == ullist1);
}

TEST(UnrolledLinkedList, Empty) {
  unrolled_linked_list<int, 5> ullist;

  EXPECT_TRUE(ullist.empty());

  ullist.push_back(1);
  ullist.push_back(2);
  ullist.push_back(3);

  EXPECT_FALSE(ullist.empty());
}

TEST(UnrolledLinkedList, Range) {
  unrolled_linked_list<double, 4> ullist;

  ullist.push_back(1.123);
  ullist.push_back(2.434);
  ullist.push_back(3.565);
  ullist.push_back(4.323);
  ullist.push_back(5.634);
  ullist.push_back(6.865);

  EXPECT_EQ(ullist.at(2), 3.565);
  EXPECT_THROW(ullist.at(9), std::out_of_range);
}

TEST(UnrolledLinkedList, Iterator) {
  unrolled_linked_list<double, 4> ullist;

  ullist.push_back(1.123);
  ullist.push_back(2.434);
  ullist.push_back(3.565);
  ullist.push_back(4.323);
  ullist.push_back(5.634);
  ullist.push_back(6.865);

  unrolled_linked_list<double, 4>::Iterator it1 = ullist.begin();

  EXPECT_EQ(*it1, 1.123);

  it1.advance(4);

  EXPECT_EQ(*it1, 4.323);
}

TEST(UnrolledLinkedList, Inseart) {
  unrolled_linked_list<int, 5> ullist;

  for (int i = 1; i < 8; i++) ullist.push_back(i);

  unrolled_linked_list<int, 5>::Iterator it = ullist.begin();

  it.advance(4);

  ullist.insert(it, 99);
  ullist.insert(it, 97);

  EXPECT_EQ(ullist[4], 97);
  EXPECT_EQ(ullist[5], 99);

  unrolled_linked_list<int, 5>::Iterator ptr = ullist.begin();
  ullist.erase(ptr);
  ullist.erase(ptr);
  ullist.erase(ullist.end());

  EXPECT_EQ(ullist.size(), 6);
  EXPECT_EQ(ullist[0], 3);
  EXPECT_EQ(ullist[1], 4);
  EXPECT_EQ(ullist[2], 97);
  EXPECT_EQ(ullist[3], 99);
  EXPECT_EQ(ullist[4], 5);
  EXPECT_EQ(ullist[5], 6);
}

TEST(UnrolledLinkedList, Swap) {
  unrolled_linked_list<int, 6> ullist;
  unrolled_linked_list<int, 6> ullist2;

  for (int i = 1; i < 8; i++) ullist.push_back(i);
  for (int i = 1; i < 5; i++) ullist2.push_back(i * i);

  unrolled_linked_list<int, 6> ullist3 = ullist;
  unrolled_linked_list<int, 6> ullist4 = ullist2;

  ullist.swap(ullist2);

  EXPECT_TRUE(ullist == ullist4);
  EXPECT_TRUE(ullist2 == ullist3);
}
