#include <iostream>
#include <cassert>

using namespace std;

class interval {
public:

  int lo;
  int hi;

  interval()
  {
        lo = 0;
        hi = 0;
  }

  interval(int low, int high)
  {
        assert(low < high);
        assert(low <16 && low>-17);
        assert(high < 16 && high > -17);
        lo = low;
        hi = high;
  }

  void set (int low,int high)
  {
        assert(low < high);
        assert(low <16 && low>-17);
        assert(high < 16 && high > -17);
        lo = low;
        hi = high;
  }

  static interval subtract(interval in_1, interval in_2) {
        static interval top(-16,15);
        interval answer;

        int sub_lo, sub_hi;

        // Interval subtraction (low1,high1) - (low2,high2) = (low1-high2, high1-low2)
        sub_lo = in_1.lo - in_2.hi;
        sub_hi = in_1.hi - in_2.lo;

        if(sub_lo > sub_hi)
                return top;

        // wrapping it around
        if(sub_lo < -16)
                sub_lo = sub_lo + 32;
        if(sub_hi < -16)
                sub_hi = sub_hi + 32;
        if(sub_lo > 15)
                sub_lo = sub_lo - 32;
        if(sub_hi > 15)
                sub_hi = sub_hi - 32;
        if(sub_lo >= sub_hi || sub_hi > 15 || sub_lo < -16)
                return top;

        answer.set(sub_lo,sub_hi);
        return answer;

  }

  static interval bitwise_and(interval in_1, interval in_2) {

        static interval top(-16,15);
        interval answer;
        int mask = 0x0000001f;
        int sign_check = 0x00000010;
        int rev_mask = 0xffffffe0;

        int m_lo_1, m_hi_1, m_lo_2, m_hi_2;
        int lowbound, highbound;
        // considering only 5 bits 
        m_lo_1 = in_1.lo&mask;
        m_hi_1 = in_1.hi&mask;
        m_lo_2 = in_2.lo&mask;
        m_hi_2 = in_2.hi&mask;

        lowbound = ~((~m_lo_1)|(~m_lo_2));
        if(lowbound&sign_check){
                lowbound |= rev_mask;
        }
        highbound = ~((~m_hi_1)|(~m_hi_2));
        if(highbound&sign_check){
                highbound |= rev_mask;
        }
        if(lowbound >= highbound || highbound > 15 || lowbound < -16)
                return top;
        answer.set(lowbound,highbound);
        return answer;

  }

};

int main()
{
   interval output, disp;
   // Test Cases Begin
   // Subtraction
   interval i1(-16,-12), i2(4,8);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -16 && disp.hi == 15);
   i1.set(-8,1);
   i2.set(-2,9);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -16 && disp.hi == 15);
   i1.set(-15,-1);
   i2.set(-8,-3);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -12 && disp.hi == 7);
   i1.set(0,12);
   i2.set(-7,15);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -15 && disp.hi == -13);
   i1.set(0,8);
   i2.set(9,15);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -15 && disp.hi == -1);
   i1.set(14,15);
   i2.set(-16,-15);
   disp = output.subtract(i1,i2);
   assert(disp.lo == -3 && disp.hi == -1);

   //Bitwise And
   i1.set(-7,-5);
   i2.set(-2,1);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == -8 && disp.hi == 1);
   i1.set(-10,9);
   i2.set(11,15);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == 2 && disp.hi == 9);
   i1.set(-3,1);
   i2.set(-2,4);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == -4 && disp.hi == 0);
   i1.set(-8,0);
   i2.set(0,1);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == -16 && disp.hi == 15);
   i1.set(-6,0);
   i2.set(4,9);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == -16 && disp.hi == 15);
   i1.set(-6,2);
   i2.set(4,9);
   disp = output.bitwise_and(i1,i2);
   assert(disp.lo == -16 && disp.hi == 15);

   return 0;
}
