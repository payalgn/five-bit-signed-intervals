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

  static interval subtract(interval in_1, interval in_2) {
        static interval top(-16,15);

        int sub_lo, sub_hi;

        // Interval subtraction (low1,high1) - (low2,high2) = (low1-high2, high1-low2)
        sub_lo = in_1.lo - in_2.hi;
        sub_hi = in_1.hi - in_2.lo;

        if(sub_lo > sub_hi)
                return top;

        // wrapping it around
        if(sub_lo < -17)
                sub_lo = sub_lo + 32;
        if(sub_hi < -17)
                sub_hi = sub_hi + 32;
        if(sub_lo >= sub_hi || sub_hi > 15 || sub_lo < -16)
                return top;

        static interval answer(sub_lo,sub_hi);
        return answer;

  }

  static interval bitwise_and(interval in_1, interval in_2) {

        static interval top(-16,15);
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
        static interval answer(lowbound,highbound);
        return answer;

  }

};

int main()
{
   interval output, disp;

   // test cases
//   interval i1(-10,9), i2(11,15);
   interval i1(-6,0), i2(1,8);

   disp = output.subtract(i1,i2);
   cout << "subtract --> " << "{" << disp.lo << "," << disp.hi << "}" << endl;
   disp = output.bitwise_and(i1,i2);
   cout << "bitwise and --> " << "{" << disp.lo << "," << disp.hi << "}" << endl;
   return 0;
}
