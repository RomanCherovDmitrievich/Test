#include<iostream>

#pragma pack(1)

class A
{
  public:
  int A: 1;
  void* p;
  long L;
};

/*
#pragma pack(1)

struct B
{
  int A: 2147483647;
  void* p;
  long L;
};

#pragma pack(pop)
*/
//#pragma pack(pop)


int main()
{
  std::cout << sizeof(A);
  return 0;
}